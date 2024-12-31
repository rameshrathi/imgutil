#include <iostream>
#include <future>

#include "src/include/stream.hpp"

const std::string TAPO_URL = "rtsp:/tapo_front:tapo_front@192.168.29.71/stream1";
const std::string VIDEOS_FOLDER = "/Users/ramesh/Downloads/resources/videos/";
const std::string IMAGES_FOLDER = "/Users/ramesh/Downloads/resources/images/";
const std::string CLASSES_PATH = "/Users/ramesh/Downloads/resources/classes.txt";

int main(const int argc, char *argv[]) {
    std::cout << "Starting worker..." << std::endl;
    // show_live_video();

    // std::string videoFile = VIDEOS_FOLDER + "walking.mp4";
    // detect_object_in_video(CLASSES_PATH, videoFile);


    start_stream(TAPO_URL.c_str());

    std::cout << "Worker is finished..." << std::endl;
    return 0;
}
