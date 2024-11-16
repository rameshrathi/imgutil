//
// Created by Ramesh Kumar on 12/11/24.
//

#include "include/cimg.h"

void whiteBalance(cv::Mat& img) {
    cv::cvtColor(img, img, cv::COLOR_BGR2RGB);
}

void cimg::ImageSharper::perform(cv::Mat& img) {
    // Create a 3x3 Laplacian kernel
    const cv::Mat kernel = (cv::Mat_<char>(3,3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);

    // Apply the kernel to the image
    filter2D(img, img, -1, kernel);
}

void cimg::ColorEnhancer::perform(cv::Mat& img) {
    // Convert to HSV color space
    cv::Mat hsv;
    cvtColor(img, hsv, cv::COLOR_BGR2HSV);

    // Adjust hue, saturation, and value (example)
    constexpr double hue_shift = 20;
    constexpr double sat_scale = 1.2;
    constexpr double val_scale = 1.1;

    hsv.forEach<cv::Vec3b>([&](cv::Vec3b& pixel, const int* position) {
        pixel[0] = cv::saturate_cast<uchar>(pixel[0] + hue_shift);
        pixel[1] = cv::saturate_cast<uchar>(pixel[1] * sat_scale);
        pixel[2] = cv::saturate_cast<uchar>(pixel[2] * val_scale);
    });
    // Convert back to BGR
    cvtColor(hsv, img, cv::COLOR_HSV2BGR);
}

void cimg::GaussianBlur::perform(cv::Mat& img) {
    cv::GaussianBlur(img, img, cv::Size(5, 5), 1.5);  // Kernel size = 5x5, Sigma = 1.5
}

void cimg::MedianBlur::perform(cv::Mat& img) {
    cv::medianBlur(img, img, 5);  // Kernel size = 5
}

void cimg::MorphOperation::perform(cv::Mat& img) {

    const int morph_size = 2;  // kernel size

    // Structuring element (kernel) creation
    cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT,
                        cv::Size(2 * morph_size + 1, 2 * morph_size + 1),
                        cv::Point(morph_size, morph_size));
    cv::morphologyEx(img, img, morphType, element);
}

void cimg::HistogramEqualization::perform(cv::Mat& img) {
    // Calculate the histogram
    // Convert to grayscale
    cv::Mat gray;
    cvtColor(img, gray, cv::COLOR_BGR2GRAY);

    // Calculate the histogram
    cv::MatND hist;
    int histSize = 256;
    float range[] = { 0, 256 };
    const float* histRange = { range };
    cv::calcHist(&gray, 1, {0}, cv::Mat(), hist, 1,
        &histSize, &histRange, true, false);

    // Equalize the histogram
    cv::equalizeHist(gray, gray);
}