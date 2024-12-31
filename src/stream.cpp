// ====================================================
// Import C api from FFmpeg
// ====================================================

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
}

#include <opencv2/opencv.hpp>
#include <iostream>
#include <filesystem> // C++17 for directory management

namespace fs = std::filesystem;

#include "include/stream.hpp"

int start_stream(const char * url) {

    // Initialize FFmpeg
    // av_register_all();
    avformat_network_init();

    // Video properties
    int width = 640, height = 480, fps = 30;

    // Create an OpenCV video capture source
    cv::VideoCapture cap(url); // Use webcam
    if (!cap.isOpened()) {
        std::cerr << "Error: Cannot open webcam!" << std::endl;
        return -1;
    }

    cap.set(cv::CAP_PROP_FRAME_WIDTH, width);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, height);
    cap.set(cv::CAP_PROP_FPS, fps);

    // Create DASH output directory
    const std::string outputDir = "/Users/ramesh/Desktop/temp/";
    if (!fs::exists(outputDir)) {
        fs::create_directory(outputDir);
    }

    // FFmpeg Output format context (MPEG-DASH)
    AVFormatContext* formatCtx = nullptr;
    avformat_alloc_output_context2(&formatCtx, nullptr, "dash", (outputDir + "/output.mpd").c_str());
    if (!formatCtx) {
        std::cerr << "Error: Cannot allocate output format context!" << std::endl;
        return -1;
    }

    // Video codec
    const AVCodec* codec = avcodec_find_encoder(AV_CODEC_ID_H264);
    if (!codec) {
        std::cerr << "Error: H.264 codec not found!" << std::endl;
        return -1;
    }

    AVStream* stream = avformat_new_stream(formatCtx, codec);
    if (!stream) {
        std::cerr << "Error: Cannot create new stream!" << std::endl;
        return -1;
    }

    AVCodecContext* codecCtx = avcodec_alloc_context3(codec);
    codecCtx->codec_id = AV_CODEC_ID_H264;
    codecCtx->width = width;
    codecCtx->height = height;
    codecCtx->time_base = AVRational{1, fps};
    codecCtx->pix_fmt = AV_PIX_FMT_YUV420P;
    codecCtx->bit_rate = 400000;
    codecCtx->gop_size = 12;
    codecCtx->max_b_frames = 2;

    if (formatCtx->oformat->flags & AVFMT_GLOBALHEADER)
        codecCtx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;

    if (avcodec_open2(codecCtx, codec, nullptr) < 0) {
        std::cerr << "Error: Cannot open codec!" << std::endl;
        return -1;
    }

    stream->time_base = AVRational{1, fps};
    stream->codecpar->codec_id = AV_CODEC_ID_H264;
    stream->codecpar->codec_type = AVMEDIA_TYPE_VIDEO;
    stream->codecpar->width = width;
    stream->codecpar->height = height;
    stream->codecpar->format = codecCtx->pix_fmt;

    // Set DASH muxer options
    av_dict_set(&formatCtx->metadata, "hls_playlist", "1", 0); // Needed for DASH
    av_dict_set(&formatCtx->metadata, "dash_segment_type", "mp4", 0);
    av_dict_set(&formatCtx->metadata, "dash_init_seg_name", "init-stream$RepresentationID$.mp4", 0);
    av_dict_set(&formatCtx->metadata, "dash_segment_name", "chunk-stream$RepresentationID$-$Number%.m4s", 0);

    // Open output file
    if (!(formatCtx->oformat->flags & AVFMT_NOFILE)) {
        if (avio_open(&formatCtx->pb, (outputDir + "output.mpd").c_str(), AVIO_FLAG_WRITE) < 0) {
            std::cerr << "Error: Cannot open output file!" << std::endl;
            return -1;
        }
    }

    // Write file header
    if (avformat_write_header(formatCtx, nullptr) < 0) {
        std::cerr << "Error: Cannot write file header!" << std::endl;
        return -1;
    }

    // Frame allocation
    AVFrame* frame = av_frame_alloc();
    frame->format = codecCtx->pix_fmt;
    frame->width = width;
    frame->height = height;
    av_image_alloc(frame->data, frame->linesize, width, height, codecCtx->pix_fmt, 32);

    SwsContext* swsCtx = sws_getContext(
        width, height, AV_PIX_FMT_BGR24, width, height, AV_PIX_FMT_YUV420P,
        SWS_BILINEAR, nullptr, nullptr, nullptr);

    // Packet for encoded data
    AVPacket pkt;
    av_init_packet(&pkt);
    pkt.data = nullptr;
    pkt.size = 0;

    // Starting writing frames to the output file which is output.mdp
    int frameCounter = 0;
    while (true) {
        // Capture a frame
        cv::Mat bgrFrame;
        cap >> bgrFrame;
        if (bgrFrame.empty()) break;

        // Convert OpenCV Mat to YUV format
        const uint8_t* srcSlice[1] = {bgrFrame.data};
        int srcStride[1] = {static_cast<int>(bgrFrame.step)};
        sws_scale(swsCtx, srcSlice, srcStride, 0, height, frame->data, frame->linesize);

        frame->pts = frameCounter++;

        // Encode frame
        int ret = avcodec_send_frame(codecCtx, frame);
        if (ret < 0) {
            std::cerr << "Error: Cannot send frame for encoding!" << std::endl;
            break;
        }

        while (ret >= 0) {
            ret = avcodec_receive_packet(codecCtx, &pkt);
            if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) break;
            if (ret < 0) {
                std::cerr << "Error: Encoding failed!" << std::endl;
                break;
            }

            av_packet_rescale_ts(&pkt, codecCtx->time_base, stream->time_base);
            pkt.stream_index = stream->index;

            // Write packet to output
            av_interleaved_write_frame(formatCtx, &pkt);
            av_packet_unref(&pkt);
        }

        // // Display locally (optional)
        // cv::imshow("Filtered Video", bgrFrame);
        // if (cv::waitKey(1) == 27) break; // Exit on ESC
    }

    // Write trailer and cleanup
    av_write_trailer(formatCtx);
    avcodec_free_context(&codecCtx);
    av_frame_free(&frame);
    avio_closep(&formatCtx->pb);
    avformat_free_context(formatCtx);

    cap.release();
    cv::destroyAllWindows();

    // Serve files on localhost using a Python HTTP server
    std::cout << "Run this command to serve DASH content locally:\n";
    std::cout << "    python3 -m http.server --directory dash_output 8000\n";
    std::cout << "Access the DASH stream at: http://localhost:8000/output.mpd\n";

    return 0;
}
