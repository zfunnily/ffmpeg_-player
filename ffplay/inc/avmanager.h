#ifndef AVMANAGER_H
#define AVMANAGER_H

#include "videocontext.h"

class AVManager : public QObject
{
    Q_OBJECT
public:
    AVManager();
    ~AVManager();

public:
    // 摄像头捕获
    int32_t startCamera();
    int32_t stopCamera();

    // 拉流
    int32_t startRtmp(QString url);
    int32_t stopRtmp();

    // 推流
    int32_t pushCamera();

public : signals:
    void sign_setCameraImage(QImage img);
// 拉流信号
    void sign_setVideoImage(QImage img);

private:
    AVFormatContext* camera_format_ctx_;
    AVFormatContext* rtmp_format_ctx_;

    VideoContext* rtmp_video_;
    VideoContext* camera_video_;

    DeviceInfo* device_;
};

#endif // AVMANAGER_H
