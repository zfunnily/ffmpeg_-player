#include "avmanager.h"

AVManager::AVManager()
{
    camera_format_ctx_ = avformat_alloc_context();
    rtmp_format_ctx_ = avformat_alloc_context();

    device_ = new DeviceInfo();

    camera_video_ = new VideoContext();
    rtmp_video_   = new VideoContext();
    // 转发摄像头信号
    QObject::connect(rtmp_video_, SIGNAL(sign_setVideoImage(QImage)), this, SIGNAL(sign_setVideoImage(QImage)));
    // 转发拉流信号
    QObject::connect(camera_video_, SIGNAL(sign_setVideoImage(QImage)), this, SIGNAL(sign_setCameraImage(QImage)));

}

AVManager::~AVManager()
{

    delete camera_video_;
    delete rtmp_video_;
    delete device_;
    delete rtmp_format_ctx_;
    delete camera_format_ctx_;
    device_ = nullptr;
}

int32_t AVManager::startCamera()
{
    //打开设备
    if (device_->show_avfoundation_device(camera_format_ctx_) != 0)
    {
        qDebug() << "open device fail!";
        return -1;
    }
    if (camera_video_->init(camera_format_ctx_) != 0)
    {
        qDebug() << "camera_video init fail!";
        return -1;
    }
    //camera_video_->setImage(camera_format_ctx_);
    return 0;
}

int32_t AVManager::stopCamera()
{
    camera_video_->setImage(camera_format_ctx_);
    return 0;

}

int32_t AVManager::startRtmp(QString url)
{
    // 打开音视频流
    if(avformat_open_input(&rtmp_format_ctx_,url.toStdString().c_str(),nullptr,nullptr)!=0){
        qDebug() << ("Couldn't open input stream.\n");
        return -1;
    }

    if (rtmp_video_->init(rtmp_format_ctx_) != 0)
    {
        qDebug() << "rtmp_video init fail!";
        return -1;
    }
    rtmp_video_->setImage(rtmp_format_ctx_);
    return 0;
}

int32_t AVManager::stopRtmp()
{
    rtmp_video_->releaseAV();
    return 0;
}

int32_t AVManager::pushCamera()
{
    int videoindex = camera_video_->getVideoIndex();

    AVFormatContext* ofmt_ctx = avformat_alloc_context();
    AVOutputFormat* ofmt =nullptr;
    const char* out_path = "rtmp://106.54.84.32:1935/live/test01";
    avformat_alloc_output_context2(&ofmt_ctx,NULL,"flv",out_path);
    //output encode initialize
    AVCodec* pCodec  = avcodec_find_encoder(AV_CODEC_ID_VP8);
    if (!pCodec)
    {
        qDebug() << ("Can not find encoder! (没有找到合适的编码器！)\n");
        return -1;
    }

    // 初始化编码器
    AVCodecContext* pCodecCtx = avcodec_alloc_context3(pCodec);
    pCodecCtx->pix_fmt = AV_PIX_FMT_YUV420P;
    pCodecCtx->width = camera_format_ctx_->streams[camera_video_->getVideoIndex()]->codec->width;
    pCodecCtx->height = camera_format_ctx_->streams[camera_video_->getVideoIndex()]->codec->height;
    pCodecCtx->time_base.num = 1;
    pCodecCtx->time_base.den = 25;
    pCodecCtx->bit_rate=40000;
    pCodecCtx->gop_size=250;

    ofmt = ofmt_ctx->oformat;
    if (ofmt_ctx->oformat->flags & AVFMT_GLOBALHEADER)
    {
        pCodecCtx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
    }
    pCodecCtx->qmin = 10;
    pCodecCtx->qmax = 51;
    pCodecCtx->max_b_frames = 3;

    AVDictionary * param = nullptr;
//    av_dict_set(&param,"preset", "fast", 0);//Set the given entry in *pm, overwriting an existing entry(覆盖现有输入).
//    av_dict_set(&param,"tune","zerolatency",0);

    if (avcodec_open2(pCodecCtx,pCodec,&param) < 0) {
        qDebug() << ("Failed to open encoder! (编码器打开失败！)\n");
        return -1;
    }

    //将新流添加到媒体文件。
    AVStream* video_st = avformat_new_stream(ofmt_ctx,pCodec);
    if (video_st == nullptr)
    {
        return -1;
    }

    video_st->codec = pCodecCtx;
    video_st->time_base.num = 1;
    video_st->time_base.den = 25;

    // 创建并初始化AVIOContext以访问url指示的资源。
    if (avio_open(&ofmt_ctx->pb,out_path,AVIO_FLAG_READ_WRITE) < 0)
    {
        qDebug() << ("Failed to open output file! (输出文件打开失败！)\n");
        return -1;
    }

    int ret;
    av_dump_format(ofmt_ctx, 0, out_path, 1);
    ret = avformat_write_header(ofmt_ctx,nullptr);
    if (ret < 0)
    {
        qDebug() << ( "Error occurred when opening output URL\n");
        return -1;
    }

    //start push
    AVPacket pkt;
    int frame_index=0;
    int64_t start_time=av_gettime();
    while(1) {
        AVStream *in_stream, *out_stream;
        //Get an AVPacket
        ret = av_read_frame(camera_format_ctx_, &pkt);
        if (ret < 0)
            break;
        //FIX：No PTS (Example: Raw H.264)
        //Simple Write PTS
        if(pkt.pts==AV_NOPTS_VALUE){
            //Write PTS
            AVRational time_base1=camera_format_ctx_->streams[videoindex]->time_base;
            //Duration between 2 frames (us)
            int64_t calc_duration=(double)AV_TIME_BASE/av_q2d(camera_format_ctx_->streams[videoindex]->r_frame_rate);
            //Parameters
            pkt.pts=(double)(frame_index*calc_duration)/(double)(av_q2d(time_base1)*AV_TIME_BASE);
            pkt.dts=pkt.pts;
            pkt.duration=(double)calc_duration/(double)(av_q2d(time_base1)*AV_TIME_BASE);
        }
        //Important:Delay
        if(pkt.stream_index==videoindex){
            AVRational time_base=camera_format_ctx_->streams[videoindex]->time_base;
            AVRational time_base_q={1,AV_TIME_BASE};
            int64_t pts_time = av_rescale_q(pkt.dts, time_base, time_base_q);
            int64_t now_time = av_gettime() - start_time;
            if (pts_time > now_time)
                av_usleep(pts_time - now_time);
        }in_stream  = camera_format_ctx_->streams[pkt.stream_index];
        out_stream = ofmt_ctx->streams[pkt.stream_index];
        /* copy packet */
        //Convert PTS/DTS
        pkt.pts = av_rescale_q_rnd(pkt.pts, in_stream->time_base, out_stream->time_base, (AVRounding)(AV_ROUND_NEAR_INF|AV_ROUND_PASS_MINMAX));
        pkt.dts = av_rescale_q_rnd(pkt.dts, in_stream->time_base, out_stream->time_base, (AVRounding)(AV_ROUND_NEAR_INF|AV_ROUND_PASS_MINMAX));
        pkt.duration = av_rescale_q(pkt.duration, in_stream->time_base, out_stream->time_base);
        pkt.pos = -1;
        //Print to Screen
        if(pkt.stream_index==videoindex){
            printf("Send %8d video frames to output URL\n",frame_index);
            frame_index++;
        }
        //ret = av_write_frame(ofmt_ctx, &pkt);
        ret = av_interleaved_write_frame(ofmt_ctx, &pkt);

        if (ret < 0) {
            printf( "Error muxing packet\n");
            break;
        }

        av_free_packet(&pkt);

    }
    //Write file trailer
    av_write_trailer(ofmt_ctx);
}

