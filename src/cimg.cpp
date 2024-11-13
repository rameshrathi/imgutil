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
    const cv::Mat img = cv::imread(filePath, cv::IMREAD_COLOR);
    if (img.empty()) {
        std::cout << "Could not read file: " << filePath << std::endl;
        return;
    }
    // Task started
    std::cout << "Task started: " << filePath << std::endl;

    // perform the processor task
    result = processor->perform(img);

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
