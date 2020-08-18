#include "videocontext.h"

VideoContext::VideoContext(MainWin* main_win)
    :pFormatCtx_(nullptr),pCodecCtx_(nullptr),pCodec_(nullptr),
      pFrame_(nullptr),pFrameYUV_(nullptr),pPacket_(nullptr),videoindex_(-1),
      out_buffer_(nullptr),img_convert_ctx_(nullptr),main_win_(main_win)
{
}

VideoContext::~VideoContext()
{
    pFormatCtx_ = nullptr;
    pCodecCtx_ = nullptr;
    pCodec_ = nullptr;
    pFrame_ = nullptr;
    pFrameYUV_ = nullptr;
    pPacket_ = nullptr;
    out_buffer_ = nullptr;
    img_convert_ctx_ = nullptr;

    delete pFormatCtx_;
    delete pCodecCtx_;
    delete pCodec_;
    delete pFrame_;
    delete pFrameYUV_;
    delete pPacket_;
    delete out_buffer_;
    delete img_convert_ctx_;
}


bool VideoContext::init(QString url)
{
    streamUrl_ = url;
    pFormatCtx_ = avformat_alloc_context();

    // 打开音视频流
    if(avformat_open_input(&pFormatCtx_,url.toStdString().c_str(),nullptr,nullptr)!=0){
        qDebug() << ("Couldn't open input stream.\n");
        return false;
    }

    // 获取音视频流数据信息
    if(avformat_find_stream_info(pFormatCtx_,nullptr)<0){
        qDebug() << ("Couldn't find stream information.\n");
        return false;
    }

    pFrame_= av_frame_alloc();
    pFrameYUV_= av_frame_alloc();
}

int VideoContext::findStreamIndex()
{
    // 查找视频流的起始索引位置（nb_streams表示视音频流的个数）
    for(int i=0; i<pFormatCtx_->nb_streams; i++)
        if(pFormatCtx_->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO){
            videoindex_=i;
            break;
        }
    if(videoindex_==-1){
        qDebug() << ("Didn't find a video stream.\n");
        return -1;
    }

    return videoindex_;
}

bool VideoContext::openCodeCtx()
{
    // 查找视频解码器
    pCodecCtx_=pFormatCtx_->streams[videoindex_]->codec; // 获取视频流编码结构
    pCodec_=avcodec_find_decoder(pCodecCtx_->codec_id);
    if(pCodec_== nullptr){
        qDebug() << ("Codec not found.\n");
        return false;
    }

    // 打开解码器
    if(avcodec_open2(pCodecCtx_, pCodec_,nullptr)<0){
        qDebug() << ("Could not open codec.\n");
        return false;
    }

    // 打印视频信息
    qDebug() << ("--------------- File Information ----------------\n");
    av_dump_format(pFormatCtx_, 0, streamUrl_.toStdString().c_str(), 0); // 此函数打印输入或输出的详细信息
    qDebug() << ("-------------------------------------------------\n");
}


void VideoContext::setImage()
{
    int ret, got_picture;

    // 创建帧结构，此函数仅分配基本结构空间，图像数据空间需通过av_malloc分配
    pFrame_ = av_frame_alloc();
    pFrameYUV_ = av_frame_alloc();

    // 创建动态内存,创建存储图像数据的空间（av_image_get_buffer_size获取一帧图像需要的大小）
    out_buffer_ = (unsigned char *)av_malloc((size_t)av_image_get_buffer_size(AV_PIX_FMT_RGB32, pCodecCtx_->width, pCodecCtx_->height, 1));
    // 存储一帧像素数据缓冲区
    av_image_fill_arrays(pFrameYUV_->data, pFrameYUV_->linesize, out_buffer_,
        AV_PIX_FMT_RGB32, pCodecCtx_->width, pCodecCtx_->height, 1);
    pPacket_ = (AVPacket *)av_malloc(sizeof(AVPacket));

    // 初始化img_convert_ctx结构
   img_convert_ctx_ = sws_getContext(pCodecCtx_->width, pCodecCtx_->height, pCodecCtx_->pix_fmt,
       pCodecCtx_->width, pCodecCtx_->height, AV_PIX_FMT_RGB32, SWS_BICUBIC, nullptr, nullptr, nullptr);

   // av_read_frame读取一帧未解码的数据
   while (av_read_frame(pFormatCtx_, pPacket_) >= 0)
   {
       // 如果是视频数据
       if (pPacket_->stream_index == videoindex_)
       {
           // 解码一帧视频数据
           ret = avcodec_decode_video2(pCodecCtx_, pFrame_, &got_picture, pPacket_);
           if (ret < 0)
           {
               qDebug() << ("Decode Error.\n");
               return;
           }
           if (got_picture)
           {
               sws_scale(img_convert_ctx_, (const unsigned char* const*)pFrame_->data, pFrame_->linesize, 0, pCodecCtx_->height,
                                   pFrameYUV_->data, pFrameYUV_->linesize);
               QImage img((uchar*)pFrameYUV_->data[0],pCodecCtx_->width,pCodecCtx_->height,QImage::Format_RGB32);
               main_win_->setVideoImage(img);
           }
       }
       av_free_packet(pPacket_);
   }

}

void VideoContext::releaseAV()
{
    /*** （四）最后要释放申请的内存空间 ***/
       sws_freeContext(img_convert_ctx_); // 释放一个SwsContext
       av_frame_free(&pFrameYUV_);
       av_frame_free(&pFrame_);
       avcodec_close(pCodecCtx_);
       avformat_close_input(&pFormatCtx_);
}

