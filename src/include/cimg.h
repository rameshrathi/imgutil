// -------------- cl-img.h -----------
#ifndef CIMG_HPP
#define CIMG_HPP

#include <opencv2/opencv.hpp>

namespace cimg {
    class ImageProcessor {
        public:
        virtual ~ImageProcessor() = default;
        virtual cv::Mat perform(const cv::Mat& img) = 0;
    };
    class ImageSharper final : public ImageProcessor {
        public:
        cv::Mat perform(const cv::Mat &img) override;
        ~ImageSharper() override = default;
    };
    class ColorEnhancer final : public ImageProcessor {
        public:
        cv::Mat perform(const cv::Mat &img) override;
        ~ColorEnhancer() override = default;
    };

    // ***********  WORKER *****************
    enum WorkerState {
        waiting,
        in_progress,
        completed,
        failed,
    };

    // Begin
    class Worker {
        std::string filePath;
        WorkerState state = waiting;
        ImageProcessor * processor;
        cv::Mat result;

    public:
        // constructor
        explicit Worker(const std::string & path, ImageProcessor & en) {
            filePath = path;
            processor =  & en;
        }

        // copy constructor
        Worker(const Worker & w) {
            filePath = w.filePath;
            state = w.state;
            processor = w.processor;
        }

        // move constructor
        Worker(Worker && w)  noexcept {
            filePath = w.filePath;
            state = w.state;
            processor = w.processor;
            w.filePath = "";
        }

        cv::Mat & getResult() {
            return result;
        }

        WorkerState & getState() {
            return state;
        }

        // start worker
        void start();

        // stop worker
        void stop();
    };

}   // End of namespace 'cimg'

namespace resources {
    const std::string IMAGES_FOLDER = "/Users/ramesh/Downloads/images/";
    const std::string VIDEOS_FOLDER = "/Users/ramesh/Downloads/videos/";
    const std::string CASCADE_FACE = "/opt/homebrew/opt/opencv/share/opencv4/haarcascades/haarcascade_frontalface_default.xml";
    const std::string CASCADE_FULL_BODY = "/opt/homebrew/opt/opencv/share/opencv4/haarcascades/haarcascade_fullbody.xml";
}  // resources

#endif // CIMG_HPP
