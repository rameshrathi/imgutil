
#include <opencv2/opencv.hpp>

const float INPUT_WIDTH = 640.0;
const float INPUT_HEIGHT = 640.0;
const float SCORE_THRESHOLD = 0.2;
const float NMS_THRESHOLD = 0.4; 
const float CONFIDENCE_THRESHOLD = 0.4;

struct Detection {

    // class Id
    int class_id;

    // confidence
    float confidence;

    // box
    cv::Rect box;
};

void load_net(cv::dnn::Net& net, bool is_cuda);
cv::Mat format_yolov5(const cv::Mat& source);

