#include "videocontext.h"

VideoContext::VideoContext()
    : frame_yuv_(nullptr),video_index_(-1),
      out_buffer_(nullptr),img_convert_ctx_(nullptr)
{
    device_ = new DeviceInfo();
}

VideoContext::~VideoContext()
{
    releaseAV();

    codec_context_ = nullptr;
    codec_ = nullptr;
    frame_ = nullptr;
    frame_yuv_ = nullptr;
    out_buffer_ = nullptr;
    img_convert_ctx_ = nullptr;

    delete codec_context_;
    delete codec_;
    delete frame_;
    delete frame_yuv_;
    delete out_buffer_;
    delete img_convert_ctx_;
}

int32_t VideoContext::openCodeContxt(AVFormatContext* format_ctx)
{    
    // 获取视频流数据信息
    if (avformat_find_stream_info(format_ctx,nullptr) < 0)
    {
        qDebug() << "无法获取流信息！";
        return -1;
    }

    video_index_ = -1;
    // 查找视频流的起始索引位置（nb_streams表示视音频流的个数）
    for(int i=0; i<format_ctx->nb_streams; i++)
        if(format_ctx->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO){
            video_index_=i;
            break;
        }
    if(video_index_==-1){
        qDebug() << ("Didn't find a video stream.\n");
        return -1;
    }

    // 查找视频解码器
    codec_context_ = format_ctx->streams[video_index_]->codec; // 获取视频流编码结构
    codec_=avcodec_find_decoder(codec_context_->codec_id);
    if(codec_== nullptr){
        qDebug() << ("Codec not found.\n");
        return false;
    }

    // 打开解码器
    if(avcodec_open2(codec_context_, codec_,nullptr)<0){
        qDebug() << ("Could not open codec.\n");
        return false;
    }

    // 打印视频信息
//    qDebug() << ("--------------- File Information ----------------\n");
//    av_dump_format(format_ctx, 0, stream_url_.toStdString().c_str(), 0); // 此函数打印输入或输出的详细信息
//    qDebug() << ("-------------------------------------------------\n");
}

int32_t VideoContext::init(AVFormatContext* format_ctx)
{
    if (openCodeContxt(format_ctx) != 0)
    {
        qDebug() << ("faile open CodeContxt!!");
        return -1;
    }
    frame_= av_frame_alloc();
    frame_yuv_= av_frame_alloc();

    setStreamState(INITSTREAM);
    return 0;
}


void VideoContext::setImage(AVFormatContext* format_ctx)
{
    int ret, got_picture;

    // 创建帧结构，此函数仅分配基本结构空间，图像数据空间需通过av_malloc分配
    frame_ = av_frame_alloc();
    frame_yuv_ = av_frame_alloc();

    // 创建动态内存,创建存储图像数据的空间（av_image_get_buffer_size获取一帧图像需要的大小）
    out_buffer_ = (unsigned char *)av_malloc((size_t)av_image_get_buffer_size(AV_PIX_FMT_RGB32, codec_context_->width, codec_context_->height, 1));
    // 存储一帧像素数据缓冲区
    av_image_fill_arrays(frame_yuv_->data, frame_yuv_->linesize, out_buffer_,
        AV_PIX_FMT_RGB32, codec_context_->width, codec_context_->height, 1);

    AVPacket*  packet = (AVPacket *)av_malloc(sizeof(AVPacket));

    // 初始化img_convert_ctx结构
   img_convert_ctx_ = sws_getContext(codec_context_->width, codec_context_->height, codec_context_->pix_fmt,
       codec_context_->width, codec_context_->height, AV_PIX_FMT_RGB32, SWS_BICUBIC, nullptr, nullptr, nullptr);

   setStreamState(STREAMING);//改变状态

   // av_read_frame读取一帧未解码的数据
   while (av_read_frame(format_ctx, packet) >= 0 && getStreamState() == STREAMING)
   {
       // 如果是视频数据
       if (packet->stream_index == video_index_)
       {
           // 解码一帧视频数据
           ret = avcodec_decode_video2(codec_context_, frame_, &got_picture, packet);
           if (ret < 0)
           {
               qDebug() << ("Decode Error.\n");
               return;
           }
           if (got_picture)
           {
               sws_scale(img_convert_ctx_, (const unsigned char* const*)frame_->data, frame_->linesize, 0, frame_->height,
                                   frame_yuv_->data, frame_yuv_->linesize);
               QImage img((uchar*)frame_yuv_->data[0],codec_context_->width,codec_context_->height,QImage::Format_RGB32);
               emit sign_setVideoImage(img);
           }
       }
       av_free_packet(packet);
   }

}

void VideoContext::releaseAV()
{
    /*** （四）最后要释放申请的内存空间 ***/
       sws_freeContext(img_convert_ctx_); // 释放一个SwsContext
       av_frame_free(&frame_yuv_);
       av_frame_free(&frame_);
       avcodec_close(codec_context_);
       //avformat_close_input(&format_ctx_);
       setStreamState(NONE);//改变状态
}


void VideoContext::getDevice()
{
    device_->show_dshow_device();
    device_->show_dshow_device_option();
    device_->show_vfw_device();
    //device_->show_avfoundation_device();
}
