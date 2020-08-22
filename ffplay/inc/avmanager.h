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
    int32_t initCamera();
    int32_t startCamera();
    int32_t stopCamera();



    int32_t initRtmp(QString url);
    int32_t startRtmp();
    int32_t stopRtmp();

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
