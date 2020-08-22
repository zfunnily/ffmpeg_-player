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
    delete device_;
    device_ = nullptr;
}

int32_t AVManager::initCamera()
{

    //打开设备
    device_->show_avfoundation_device(camera_format_ctx_);
    camera_video_->init(camera_format_ctx_);
    return 0;
}


int32_t AVManager::startCamera()
{
    camera_video_->setImage(camera_format_ctx_);
    return 0;
}

int32_t AVManager::stopCamera()
{
    camera_video_->setImage(camera_format_ctx_);
    return 0;

}

int32_t AVManager::initRtmp(QString url)
{
    QObject::connect(rtmp_video_, SIGNAL(sign_setVideoImage(QImage img)), this, SLOT(slot_setVideoImage(QImage img)));

    // 打开音视频流
    if(avformat_open_input(&rtmp_format_ctx_,url.toStdString().c_str(),nullptr,nullptr)!=0){
        qDebug() << ("Couldn't open input stream.\n");
        return -1;
    }

    rtmp_video_->init(rtmp_format_ctx_);
    return 0;
}

int32_t AVManager::startRtmp()
{
    rtmp_video_->setImage(rtmp_format_ctx_);
    return 0;
}

int32_t AVManager::stopRtmp()
{
    rtmp_video_->releaseAV();
    return 0;
}
