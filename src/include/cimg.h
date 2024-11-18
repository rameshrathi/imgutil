// -------------- cl-img.h -----------
#ifndef CIMG_HPP
#define CIMG_HPP

#include <opencv2/opencv.hpp>

namespace cimg {
    class ImageProcessor {
        public:
        virtual ~ImageProcessor() = default;
        virtual void perform(cv::Mat& img) = 0;
    };
    class ImageSharper final : public ImageProcessor {
        public:
        ~ImageSharper() override = default;
        void perform(cv::Mat &img) override;
    };
    class ColorEnhancer final : public ImageProcessor {
        public:
        ~ColorEnhancer() override = default;
        void perform(cv::Mat &img) override;
    };
    class GaussianBlur final : public ImageProcessor {
        public:
        ~GaussianBlur() override = default;
        void perform(cv::Mat &img) override;
    };
    class MedianBlur final : public ImageProcessor {
    public:
        ~MedianBlur() override = default;
        void perform(cv::Mat &img) override;
    };

    class LaplacianSharing final : public ImageProcessor {
        public:
        ~LaplacianSharing() override = default;
        void perform(cv::Mat &img) override;
    };

    enum MorphOperationType {
        ERODE    = 0, //!< see #erode
        DILATE   = 1, //!< see #dilate
        OPEN     = 2, //!< an opening operation
        CLOSE    = 3, //!< a closing operation
        GRADIENT = 4, //!< a morphological gradient
        TOPHAT   = 5, //!< "top hat"
        BLACKHAT = 6, //!< "black hat"
        HITMISS
    };
    class MorphOperation final : public ImageProcessor {
        MorphOperationType morphType;
        public:
        explicit MorphOperation(const MorphOperationType mType) : morphType(mType) {}
        ~MorphOperation() override = default;
        void perform(cv::Mat &img) override;
    };

    class HistogramEqualization final : public ImageProcessor {
        public:
        ~HistogramEqualization() override = default;
        void perform(cv::Mat &img) override;
    };

    class CannyEdgeDetector final : public ImageProcessor {
        public:
        ~CannyEdgeDetector() override = default;
        void perform(cv::Mat &img) override;
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
        ImageProcessor *processor;
        cv::Mat result;

    public:
        // constructor
        explicit Worker(const std::string & path, ImageProcessor & en) {
            filePath = path;
            processor =  & en;
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
