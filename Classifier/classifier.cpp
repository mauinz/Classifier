#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <iostream>
#include <sstream>
#include "classifier.hpp"

using namespace std;
using namespace cv;

Classifier::Classifier(){}
Classifier::~Classifier(){}

int Classifier::getSIFT(const char* argv, cv::Mat& _res){
    const cv::Mat input = cv::imread(argv, 0);

    cv::SiftFeatureDetector detector;
    std::vector<cv::KeyPoint> keypoints;
    detector.detect(input, keypoints);

    // Add results to image and save.
    cv::drawKeypoints(input, keypoints, _res);

    return 0;
}
