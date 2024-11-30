//
// Created by Ramesh Kumar on 08/11/24.
//

#include <filesystem>
#include <iostream>
#include "include/cimg.h"

void cimg::Worker::start() {
    // run task on background thread
    if (filePath.empty()) {
        state = failed;
        return;
    }
    cv::Mat img = cv::imread(filePath, cv::IMREAD_COLOR);
    if (img.empty()) {
        std::cout << "Could not read file: " << filePath << std::endl;
        return;
    }
    // Task started
    std::cout << "Task started: " << filePath << std::endl;

    Input input = Input(img);
    processor(input);
    result = img;

    if (result.empty()) {
        state = failed;
        std::cout << "Task failed: " << filePath << std::endl;
    }
    else {
        state = completed;
        std::cout << "Task completed: " << filePath << std::endl;
    }
}

void cimg::Worker::stop() {
    // run task on background thread
    state = failed;
    std::cout << "Task stopped: " << filePath << std::endl;
}

//  =============================================================
//  Image processors
//  =============================================================

namespace cimg
{
    void whiteBalance(Input & input) {
        cv::cvtColor(input.image, input.image, cv::COLOR_BGR2RGB);
    }

    void imageSharper(Input & input) {
        // Sharper kernel
        const cv::Mat kernel = (cv::Mat_<char>(3,3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);
        // Apply the kernel to the image
        filter2D(input.image, input.image, -1, kernel);
    }

    void colorEnhancer(Input & input) {
        cv::Mat hsv;
        cvtColor(input.image, hsv, cv::COLOR_BGR2HSV);

        // Adjust hue, saturation, and value (example)
        constexpr double hue_shift = 10;
        constexpr double sat_scale = 1.1;
        constexpr double val_scale = 1.1;

        hsv.forEach<cv::Vec3b>([&](cv::Vec3b& pixel, const int* position) {
            pixel[0] = cv::saturate_cast<uchar>(pixel[0] + hue_shift);
            pixel[1] = cv::saturate_cast<uchar>(pixel[1] * sat_scale);
            pixel[2] = cv::saturate_cast<uchar>(pixel[2] * val_scale);
        });
        
        // Convert back to BGR
        cvtColor(hsv, input.image, cv::COLOR_HSV2BGR);
    }

    void gaussianBlur(Input & input) {
        // Kernel size = 5x5, Sigma = 1.5
        cv::GaussianBlur(input.image, input.image, cv::Size(5, 5), 1.5);
    }

    void medianBlur(Input & input) {
        cv::medianBlur(input.image, input.image, 5);  // Kernel size = 5
    }

    void edgeDetector(Input & input) {
        cv::Mat result;
        cv::Canny(input.image, result, 100, 200, 3);
        input.image = std::move(result);
    }

    void laplacianSharping(Input & input) {
        const cv::Mat kernel = (cv::Mat_<char>(3, 3) << 0, 1, 0, 1, -4, 1, 0, 1, 0);
        cv::Mat sharpened_img;
        filter2D(input.image, sharpened_img, -1, kernel);
        input.image = std::move(sharpened_img);
    }

    void histogramEqualization(Input & input) {
        // Calculate the histogram
        cv::Mat gray;
        cvtColor(input.image, gray, cv::COLOR_BGR2GRAY);

        // Calculate the histogram
        cv::MatND hist;
        constexpr int histSize = 256;
        float range[] = { 0, 256 };
        const float* histRange = { range };

        int channels = 0;
        int *addr_channel = & channels;
    
        cv::calcHist(&gray, 1, addr_channel, cv::Mat(),
            hist, 1, &histSize, &histRange, true, false);
        // Equalize the histogram
        cv::equalizeHist(gray, gray);
    }

    void morphOperations(Input & input) {
        const int morph_size = 2;  // kernel size
        // Structuring element (kernel) creation
        cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT,
                            cv::Size(2 * morph_size + 1, 2 * morph_size + 1),
                            cv::Point(morph_size, morph_size));
        
        auto morphType = cv::MorphTypes::MORPH_OPEN;
        cv::morphologyEx(input.image, input.image, morphType, element);
    }

} //namespace cimg