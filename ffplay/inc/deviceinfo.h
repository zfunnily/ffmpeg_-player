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
    int32_t show_avfoundation_device(AVFormatContext*   format_ctx);
};

#endif // DEVICEINFO_H
