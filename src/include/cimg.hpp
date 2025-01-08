// -------------- cl-img.h -----------
#ifndef CIMG_HPP
#define CIMG_HPP

#include <opencv2/opencv.hpp>

namespace cimg {
    struct Input {
        cv::Mat image;
        Input(cv::Mat img) : image(img) {}
    };
    using IMAGE_PROCESSOR = void (*) (Input & input);

    // *********  Processors ***************

    void whiteBalance(Input & input);
    void imageSharper(Input & input);
    void colorEnhancer(Input & input);
    void gaussianBlur(Input & input);
    void medianBlur(Input & input);
    void edgeDetector(Input & input);
    void laplacianSharping(Input & input);
    void histogramEqualization(Input & input);

    struct MorphInput {
        cv::Mat image;
        cv::MorphTypes morphType;
    };
    void morphOperations(Input & input);

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
        IMAGE_PROCESSOR processor;
        cv::Mat result;

    public:
        // constructor
        explicit Worker(const std::string _path, IMAGE_PROCESSOR _processor) {
            filePath = _path;
            processor =  _processor;
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
