#include "deviceinfo.h"
//Q_DECLARE_METATYPE(QCameraInfo)

DeviceInfo::DeviceInfo()
{
    avdevice_register_all();
}

DeviceInfo::~DeviceInfo()
{
}

void DeviceInfo::show_dshow_device()
{
    AVFormatContext *pFormatCtx = avformat_alloc_context();
    AVDictionary* options = nullptr;
    av_dict_set(&options,"list_devices","true",0);
    AVInputFormat *iformat = av_find_input_format("dshow");
    qDebug() << ("Device Info=============\n");
    avformat_open_input(&pFormatCtx,"list",iformat,&options);
    qDebug() <<("========================\n");
}

void DeviceInfo::show_dshow_device_option()
{
    AVFormatContext *pFormatCtx = avformat_alloc_context();
    AVDictionary* options = nullptr;
    av_dict_set(&options,"list_options","true",0);
    AVInputFormat *iformat = av_find_input_format("dshow");
    qDebug() <<("========Device Option Info======\n");
    avformat_open_input(&pFormatCtx,"list",iformat,&options);
    qDebug() <<("================================\n");

}

void DeviceInfo::show_vfw_device()
{
    AVFormatContext *pFormatCtx = avformat_alloc_context();
    AVInputFormat *iformat = av_find_input_format("vfwcap");
    qDebug() <<("========VFW Device Info======\n");
    avformat_open_input(&pFormatCtx,"list",iformat,nullptr);
    qDebug() <<("=============================\n");
}

int32_t DeviceInfo::show_avfoundation_device(AVFormatContext*   format_ctx)
{
   AVDictionary* options = nullptr;
   av_dict_set(&options,"list_devices","true",0);
   AVInputFormat *iformat = av_find_input_format("avfoundation");
   qDebug() <<("==AVFoundation Device Info===\n");
   //avformat_open_input(&format_ctx,"0",iformat,&options);
   if (avformat_open_input(&format_ctx,"1",iformat,nullptr) != 0)
   {
        qDebug() << "Couldn't open input stream. \n";
        qDebug() <<("=============================\n");
        return -1;
   }
   qDebug() <<("=============================\n");
   return 0;


}

