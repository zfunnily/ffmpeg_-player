#ifndef DEVICEINFO_H
#define DEVICEINFO_H

#include "codehandler.h"
#include <QDebug>
//#include <QList>
//#include <QtMultimedia/QCamera>
//#include <QtMultimediaWidgets/QCameraViewfinder>
//#include<QtMultimedia/QCameraImageCapture>

class DeviceInfo
{
public:
    DeviceInfo();
    ~DeviceInfo();

public:
    //Show Device
    void show_dshow_device();

    //Show Dshow Device Option
    void show_dshow_device_option();

    //Show VFW Device
    void show_vfw_device();

    //Show AVFoundation Device
    void show_avfoundation_device(AVFormatContext*   format_ctx);
private:
    AVFormatContext*   format_ctx_;
    AVDictionary* options_;
    AVInputFormat* iformat_;
};

#endif // DEVICEINFO_H
