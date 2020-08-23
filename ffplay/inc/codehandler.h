#ifndef CODEHANDLER_H
#define CODEHANDLER_H
extern "C"
{
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libswscale/swscale.h>
    #include <libavutil/imgutils.h>

    #include "libavdevice/avdevice.h"
    #include "libavutil/time.h"
};

class CodeInterface{
public:
    enum StreamState{
        NONE,
        INITSTREAM,
        STREAMING,
    };

    CodeInterface():codec_context_(nullptr), codec_(nullptr), frame_(nullptr),state_(NONE){}
    virtual ~CodeInterface() {}

    virtual int32_t init(AVFormatContext* format_ctx) = 0;
    virtual int32_t openCodeContxt(AVFormatContext* format_ctx) = 0;
    virtual int32_t decode() = 0;
    virtual int32_t encode() = 0;

    virtual void    setStreamState(StreamState state) { state_ = state;}
    virtual StreamState getStreamState() {return state_;}


public:
    AVCodecContext  *codec_context_;
    AVCodec         *codec_;
    AVFrame         *frame_;
    StreamState      state_;
};

#endif // CODEHANDLER_H
