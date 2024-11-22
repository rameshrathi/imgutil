#include <iostream>
#include <future>

#include "src/include/cimg.h"
#include "src/include/utils.h"

void startWorkers(const std::string & folderPath) {
    // Workers
    std::vector<cimg::Worker> workers;
    if (workers.empty()) {
        // create workers
        for (const std::vector<std::string> files = utils::load_dir_images(folderPath);
            const std::string& file : files) {
                auto sharper = cimg::WhiteBalance();
                const auto worker = cimg::Worker(file, sharper);
                workers.push_back(worker);
            }
    }
    // run workers
    for (cimg::Worker & w : workers) {
        w.start();
        if (w.getState() == cimg::WorkerState::completed) {
            cv::Mat & result = w.getResult();
            imshow("Result", result);
            cv::waitKey(3000);
        }
    }
}

// ======================================
int main(const int argc, char *argv[]) {
    std::cout << "Starting worker..." << std::endl;
	
	if (argc < 2) {
		std::cout << "!Error! - Requires image folder path in arguments" << std::endl;
		return 1;
	}

    const std::string folderPath = argv[1];
    startWorkers(folderPath);

    std::cout << "Worker is finished..." << std::endl;
    return 0;
}
