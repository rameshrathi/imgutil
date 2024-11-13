//
// Created by Ramesh Kumar on 12/11/24.
//

#include "include/cimg.h"

cv::Mat cimg::ImageSharper::perform(const cv::Mat& img) {
    cv::Mat sharp;

    // Create a 3x3 Laplacian kernel
    const cv::Mat kernel = (cv::Mat_<char>(3,3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);

    // Apply the kernel to the image
    filter2D(img, sharp, -1, kernel);
    return sharp;
}

cv::Mat cimg::ColorEnhancer::perform(const cv::Mat& img) {
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
    cv::Mat corrected_img;
    cvtColor(hsv, corrected_img, cv::COLOR_HSV2BGR);

    return corrected_img;
}
