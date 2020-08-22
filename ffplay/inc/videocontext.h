#ifndef VIDEOCONTEXT_H
#define VIDEOCONTEXT_H
#include <QDebug>
#include <QImage>

#include "codehandler.h"
#include "deviceinfo.h"

class VideoContext : public QObject,public CodeInterface
{
    Q_OBJECT
public:
    VideoContext();
    virtual ~VideoContext();

public:
    virtual int32_t init(AVFormatContext* format_ctx);
    virtual int32_t openCodeContxt(AVFormatContext* format_ctx);
    virtual int32_t decode() { return 0;}
    virtual int32_t encode() {return 0;}

    void releaseAV();

public:
    void getDevice();
public slots :
    void setImage(AVFormatContext* format_ctx);

public: signals:
// 图像信号
    void sign_setVideoImage(QImage img);

private:
    AVFormatContext* format_ctx_;  // 存储音视频封装格式中包含的信息
    AVFrame*         frame_yuv_;
    int              video_index_;   //视频帧索引，初始化为-1

    unsigned char *out_buffer_;
    struct SwsContext *img_convert_ctx_; // 主要用于视频图像的转换

private:
    QString         stream_url_;
    DeviceInfo*     device_;

};

#endif // VIDEOCONTEXT_H
