#ifndef VIDEOCONTEXT_H
#define VIDEOCONTEXT_H
#include <QDebug>
#include <QImage>

#include "codehandler.h"

class MainWin{
public:
    virtual void setVideoImage(QImage img) = 0;
};

class VideoContext : public CodeInterface
{
public:
    VideoContext(MainWin* main_win);
    virtual ~VideoContext();

    //MainCallBack

public:
    virtual int32_t init(AVFormatContext* format_ctx);
    virtual int32_t openCodeContxt(AVFormatContext* format_ctx);
    virtual int32_t decode() { return 0;}
    virtual int32_t encode() {return 0;}

    void setImage(AVFormatContext* format_ctx);
    void releaseAV();


private:
    AVFormatContext* format_ctx_;  // 存储音视频封装格式中包含的信息
    AVFrame*         frame_yuv_;
    int              video_index_;   //视频帧索引，初始化为-1

    unsigned char *out_buffer_;
    struct SwsContext *img_convert_ctx_; // 主要用于视频图像的转换


private:
    QString         stream_url_;
    MainWin*        main_win_;

};

#endif // VIDEOCONTEXT_H
