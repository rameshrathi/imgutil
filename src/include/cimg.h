// -------------- cl-img.h -----------
#ifndef CIMG_HPP
#define CIMG_HPP

#include <vector>

namespace cimg {
    // Begin
    class Worker {
        std::string folderPath;
        std::vector<std::string> fileNames = {};
    public:
        explicit Worker(const char *path) : folderPath(path) {}
    };

    // == starting the worker ==
    void start_worker(Worker &worker);

    // == stop the worker ==
    void stop_worker(Worker &worker);

    // End of namespace 'cimg'
}
// -------------------
#endif
