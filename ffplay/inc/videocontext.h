#ifndef VIDEOCONTEXT_H
#define VIDEOCONTEXT_H
#include <QDebug>
#include <QImage>

extern "C"
{
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libswscale/swscale.h>
    #include <libavutil/imgutils.h>
};

class MainWin{
public:
    virtual void setVideoImage(QImage img) = 0;
};
//// 延时函数
//void delay(int msec)
//{

//}

class VideoContext
{
public:
    VideoContext(MainWin* main_win);
    ~VideoContext();

    //MainCallBack

public:
    bool init(QString url);
    int findStreamIndex();
    bool openCodeCtx();
    void setImage();

    void releaseAV();


private:
    AVFormatContext* pFormatCtx_;  // 存储音视频封装格式中包含的信息
    AVCodecContext*  pCodecCtx_;   // 视频流编码结构
    AVCodec*         pCodec_;        // 视频解码器
    AVFrame*         pFrame_;
    AVFrame*         pFrameYUV_;
    AVPacket*        pPacket_;
    int              videoindex_;   //视频帧索引，初始化为-1

    unsigned char *out_buffer_;
    struct SwsContext *img_convert_ctx_; // 主要用于视频图像的转换


private:
    QString         streamUrl_;
    MainWin*        main_win_;

};

#endif // VIDEOCONTEXT_H
