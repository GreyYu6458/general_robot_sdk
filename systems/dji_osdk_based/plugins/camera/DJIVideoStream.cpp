#include "DJIVideoStream.hpp"
#include <dji_vehicle.hpp>
#include <opencv2/opencv.hpp>

extern "C"
{
    #include <libavformat/avformat.h>
    #include <libavcodec/avcodec.h>
    #include <libavutil/imgutils.h>
    #include <libswscale/swscale.h>
}

class RTMPClient
{
public:
    ~RTMPClient()
    {
        av_write_trailer(ofmt_ctx);

        if(frame)
            av_frame_free(&frame);

        if(swsctx)
            sws_freeContext(swsctx);
        
        if(out_codec_ctx)
            avcodec_close(out_codec_ctx);
        
        if(ofmt_ctx)
        {
            avio_close(ofmt_ctx->pb);
            avformat_free_context(ofmt_ctx);
        }
    }

    void push(cv::Mat& image)
    {
        const int stride[] = {static_cast<int>(image.step[0])};

        sws_scale(swsctx, &image.data, stride, 0, image.rows, frame->data, frame->linesize);

        frame->pts += av_rescale_q(1, out_codec_ctx->time_base, out_stream->time_base);

        write_frame();
    }

    void initFFmpeg(const std::string& name)
    {
        outputServer = "rtmp://139.224.133.41:1935/live/" + name;

        #if LIBAVCODEC_VERSION_INT < AV_VERSION_INT(58, 9, 100)
          av_register_all();
        #endif
          avformat_network_init();

        initialize_avformat_context();

        initialize_io_context();

        out_codec       = avcodec_find_encoder(AV_CODEC_ID_H264);

        out_stream      = avformat_new_stream(ofmt_ctx, out_codec);

        out_codec_ctx   = avcodec_alloc_context3(out_codec);

        set_codec_params();

        initialize_codec_stream();

        out_stream->codecpar->extradata = out_codec_ctx->extradata;

        out_stream->codecpar->extradata_size = out_codec_ctx->extradata_size;

        av_dump_format(ofmt_ctx, 0, outputServer.c_str(), 1);

        swsctx = initialize_sample_scaler();

        frame = allocate_frame_buffer();

        int ret = avformat_write_header(ofmt_ctx, nullptr);
    }

    void deinitFFMPEG()
    {
        av_write_trailer(ofmt_ctx);

        if(frame)
            av_frame_free(&frame);

        if(swsctx)
            sws_freeContext(swsctx);
        
        if(out_codec_ctx)
            avcodec_close(out_codec_ctx);
        
        if(ofmt_ctx)
        {
            avio_close(ofmt_ctx->pb);
            avformat_free_context(ofmt_ctx);
        }
    }

    void createIO(const std::string& url)
    {
        outputServer = url;
    }

private:
    
    void write_frame()
    {
      AVPacket pkt = {0};
      av_new_packet(&pkt, 0);

      int ret = avcodec_send_frame(out_codec_ctx, frame);
      if (ret < 0)
      {
        std::cout << "Error sending frame to codec context!" << std::endl;
      }

      ret = avcodec_receive_packet(out_codec_ctx, &pkt);
      if (ret < 0)
      {
        std::cout << "Error receiving packet from codec context!" << std::endl;
      }

      av_interleaved_write_frame(ofmt_ctx, &pkt);
      av_packet_unref(&pkt);
    }


    void initialize_avformat_context()
    {
        int ret = avformat_alloc_output_context2(&ofmt_ctx, nullptr, "flv", nullptr);
        if (ret < 0)
        {
          std::cout << "Could not allocate output format context!" << std::endl;
        }
    }

    void initialize_io_context()
    {
        if (!(ofmt_ctx->oformat->flags & AVFMT_NOFILE))
        {
            int ret = avio_open2(&ofmt_ctx->pb, outputServer.c_str(), AVIO_FLAG_WRITE, nullptr, nullptr);
            if (ret < 0)
            {
              std::cout << "Could not open output IO context!" << std::endl;
            }
        }
    }

    void set_codec_params()
    {
        const AVRational dst_fps = {fps, 1};

        out_codec_ctx->codec_tag = 0;
        out_codec_ctx->codec_id = AV_CODEC_ID_H264;
        out_codec_ctx->codec_type = AVMEDIA_TYPE_VIDEO;
        out_codec_ctx->width = width;
        out_codec_ctx->height = height;
        out_codec_ctx->gop_size = 12;
        out_codec_ctx->pix_fmt = AV_PIX_FMT_YUV420P;
        out_codec_ctx->framerate = dst_fps;
        out_codec_ctx->time_base = av_inv_q(dst_fps);
        out_codec_ctx->bit_rate = bitrate;

        if (ofmt_ctx->oformat->flags & AVFMT_GLOBALHEADER)
        {
          out_codec_ctx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
        }
    }

    void initialize_codec_stream()
    {
        int ret = avcodec_parameters_from_context(out_stream->codecpar, out_codec_ctx);
        if (ret < 0)
        {
            std::cout << "Could not initialize stream codec parameters!" << std::endl;
        }

        AVDictionary *codec_options = nullptr;
        av_dict_set(&codec_options, "profile", profile.c_str(), 0);
        av_dict_set(&codec_options, "preset", "superfast", 0);
        av_dict_set(&codec_options, "tune", "zerolatency", 0);

        // open video encoder
        ret = avcodec_open2(out_codec_ctx, out_codec, &codec_options);
        if (ret < 0)
        {
            std::cout << "Could not open video encoder!" << std::endl;
        }
    }

    SwsContext *initialize_sample_scaler()
    {
        SwsContext *swsctx = sws_getContext(1280, 960, AV_PIX_FMT_BGR24, width, height, out_codec_ctx->pix_fmt, SWS_BICUBIC, nullptr, nullptr, nullptr);
        if (!swsctx)
        {
          std::cout << "Could not initialize sample scaler!" << std::endl;
        }
        return swsctx;
    }

    AVFrame *allocate_frame_buffer()
    {
        AVFrame *frame = av_frame_alloc();
        auto av_buffer = av_malloc(av_image_get_buffer_size(out_codec_ctx->pix_fmt, width, height, 1));
        av_image_fill_arrays(frame->data, frame->linesize, (uint8_t *)av_buffer, out_codec_ctx->pix_fmt, width, height, 1);
        frame->width = width;
        frame->height = height;
        frame->format = static_cast<int>(out_codec_ctx->pix_fmt);

        return frame;
    }

    AVFormatContext *ofmt_ctx = nullptr;
    AVCodec         *out_codec = nullptr;
    AVStream        *out_stream = nullptr;
    AVCodecContext  *out_codec_ctx = nullptr;

    AVFrame         *frame = nullptr;
    SwsContext      *swsctx = nullptr;

    std::string outputServer {"rtmp://139.224.133.41:1935/live/opencv2rtmp"};
    static const std::string profile;

    double  width{960};
    double  height{720};
    int     bitrate{300000};
    int     fps{15};
};

const std::string RTMPClient::profile = "high444";

/**
 * @brief Camera Control Plugin
 */
class DJIVideoStream::Impl
{	
    friend class DJIVideoStream;
public:

    Impl(const std::shared_ptr<DJIVehicleSystem>& system)
    : _system(system)
    {
        _rtmp_client = std::make_unique<RTMPClient>();
    }

    static void rtmpSend(CameraRGBImage img, void* userData)
    {
        Impl* controller = (Impl*)(userData);

        cv::Mat mat(img.height, img.width, CV_8UC3, img.rawData.data(), img.width*3);

        cvtColor(mat, mat, cv::COLOR_RGB2BGR);

        controller->_rtmp_client->push(mat);
    }

    bool startStream()
    {
        _rtmp_client->initFFmpeg(_system->uniqueCode());

        _system->vehicle()->advancedSensing->startFPVCameraStream(
            &Impl::rtmpSend,
            this
        );
        return true;
    }

    bool stopStream()
    {
        _system->vehicle()->advancedSensing->stopFPVCameraStream();

        _rtmp_client->deinitFFMPEG();
        return true;
    }

private:
    std::unique_ptr<RTMPClient> _rtmp_client;

    std::shared_ptr<DJIVehicleSystem> _system;
};


// IMPL
DJIVideoStream::DJIVideoStream(const std::shared_ptr<DJIVehicleSystem>& system)
    :   ::rsdk::camera::VideoStreamInterface(system), 
        DJIPluginBase(system), 
        _impl (new Impl(system))
{
}

DJIVideoStream::~DJIVideoStream()
{
    delete _impl;
}

bool DJIVideoStream::start()
{
    return _impl->startStream();
}

bool DJIVideoStream::isStarted()
{
    return _impl->stopStream();
}

DJIVehicleModels DJIVideoStream::supportModel()
{
    return DJIVehicleModels::MODEL_ALL;
}

void DJIVideoStream::exec()
{
    start();
}
