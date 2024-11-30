
#include <opencv2/opencv.hpp>

#include "include/detect.h"
#include "include/helpers.h"

using namespace cv;

//Color definitons and Constant assigments
//Renk ve Sabit atamalarý
const std::vector<cv::Scalar> colors = { cv::Scalar(255, 255, 0), cv::Scalar(0, 255, 0), cv::Scalar(0, 255, 255), cv::Scalar(255, 0, 0) };

void detect_video(cv::Mat& image, cv::dnn::Net& net, std::vector<Detection>& output, const std::vector<std::string>& className) {

    cv::Mat blob;

    auto input_image = format_yolov5(image);

    cv::dnn::blobFromImage(input_image, blob, 1. / 255., cv::Size(INPUT_WIDTH, INPUT_HEIGHT), cv::Scalar(), true, false);
    net.setInput(blob);
    std::vector<cv::Mat> outputs;
    net.forward(outputs, net.getUnconnectedOutLayersNames());

    float x_factor = input_image.cols / INPUT_WIDTH;
    float y_factor = input_image.rows / INPUT_HEIGHT;

    float* data = (float*)outputs[0].data;

    const int dimensions = 85;
    const int rows = 25200;

    std::vector<int> class_ids;
    std::vector<float> confidences;
    std::vector<cv::Rect> boxes;

    for (int i = 0; i < rows; ++i) {

        float confidence = data[4];
        if (confidence >= CONFIDENCE_THRESHOLD) {

            float* classes_scores = data + 5;
            cv::Mat scores(1, className.size(), CV_32FC1, classes_scores);
            cv::Point class_id;
            double max_class_score;
            minMaxLoc(scores, 0, &max_class_score, 0, &class_id);
            if (max_class_score > SCORE_THRESHOLD) {

                confidences.push_back(confidence);

                class_ids.push_back(class_id.x);

                float x = data[0];
                float y = data[1];
                float w = data[2];
                float h = data[3];
                // Kapsayýcý kutunun x, y koordinatlarý; yükseklik ve geniþliði
                // Width, height and x,y coordinates of bounding box

                int left = int((x - 0.5 * w) * x_factor);
                int top = int((y - 0.5 * h) * y_factor);
                int width = int(w * x_factor);
                int height = int(h * y_factor);
                boxes.push_back(cv::Rect(left, top, width, height));
            }

        }
        data += 85;

    }

    std::vector<int> nms_result;
    cv::dnn::NMSBoxes(boxes, confidences, SCORE_THRESHOLD, NMS_THRESHOLD, nms_result);
    for (int i = 0; i < nms_result.size(); i++) {
        int idx = nms_result[i];
        Detection result;
        result.class_id = class_ids[idx];
        result.confidence = confidences[idx];
        result.box = boxes[idx];
        output.push_back(result);
    }
}

int detect_object_in_video(const std::string & classPath, const std::string & videoUrl)
{
    // Loads the list of class names using load_class_list()
    std::vector<std::string> class_list = helper::load_class_list(classPath);

    // Opens a video file for processing.
    cv::Mat frame;
    cv::VideoCapture capture(videoUrl);
    if (!capture.isOpened())
    {
        std::cerr << "Error while opening video media\n";
        return -1;
    }

    // Use CUDA if existing
    bool is_cuda = false;

    // Loading YOLOv5 model using load_net()
    cv::dnn::Net net;
    load_net(net, is_cuda);

    auto start = std::chrono::high_resolution_clock::now();
    int frame_count = 0;
    float fps = -1;
    int total_frames = 0;

    // Calculate the new dimensions while preserving the aspect ratio
    int targetWidth = 800; // Adjust the desired width

    while (true)
    {
        capture.read(frame);
        if (frame.empty())
        {
            std::cout << "Media finished\n";
            break;
        }

        std::vector<Detection> output;
        detect_video(frame, net, output, class_list);

        frame_count++;
        total_frames++;

        int detections = output.size();

        for (int i = 0; i < detections; ++i)
        {
            auto detection = output[i];
            auto box = detection.box;
            auto classId = detection.class_id;
            const auto color = colors[classId % colors.size()];
            cv::rectangle(frame, box, color, 3);

            cv::rectangle(frame, cv::Point(box.x, box.y - 20), cv::Point(box.x + box.width, box.y), color, cv::FILLED);
            cv::putText(frame, class_list[classId].c_str(), cv::Point(box.x, box.y - 5), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0));
        }

        if (frame_count >= 30)
        {
            auto end = std::chrono::high_resolution_clock::now();
            fps = frame_count * 1000.0 / std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

            frame_count = 0;
            start = std::chrono::high_resolution_clock::now();
        }

        if (fps > 0)
        {
            std::ostringstream fps_label;
            fps_label << std::fixed << std::setprecision(2);
            fps_label << "FPS: " << fps;
            std::string fps_label_str = fps_label.str();

            cv::putText(frame, fps_label_str.c_str(), cv::Point(10, 25), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 255), 2);
        }

        // Resize the frame with it's new shape as it's original ratio
        int targetHeight = static_cast<int>(frame.rows * static_cast<float>(targetWidth) / frame.cols);
        cv::Size newSize(targetWidth, targetHeight);
        cv::Mat resized_frame;
        cv::resize(frame, resized_frame, newSize);

        cv::imshow("output", resized_frame);

        int key = cv::waitKey(1);
        if (key == 27) // ESC to exit
        {
            break;
        }
    }
    return 0;
}

int show_live_video() {
    
    // Camera Feed URL => rtsp:/tapo_front:tapo_front@192.168.29.71/stream1
    // Video capture

    std::string rtsp_url = "rtsp:/tapo_front:tapo_front@192.168.29.71/stream1";
    VideoCapture cap(rtsp_url);

    if (!cap.isOpened()) {
        std::cerr << "Error opening video stream" << std::endl;
        return -1;
    }

    while (true) {
        Mat frame;
        cap >> frame;

        if (frame.empty()) {
            std::cerr << "Error capturing frame" << std::endl;
            break;
        }

        imshow("RTSP Stream", frame);

        // Exit if 'q' key is pressed
        if (waitKey(1) == 'q') {
            break;
        }
    }

    cap.release();
    destroyAllWindows();

    return 0;
}