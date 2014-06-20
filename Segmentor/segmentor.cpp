#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <sstream>
#include "segmentor.hpp"

using namespace std;
using namespace cv;

const string winName = "Image";
const Scalar RED = Scalar(0,0,255);
const Scalar BLUE = Scalar(255,0,0);
const int iter = 7;

Segmentor::Segmentor(){}
Segmentor::~Segmentor(){}


void Segmentor::getBinMask( const Mat& comMask, Mat& binMask ){
  if( comMask.empty() || comMask.type()!=CV_8UC1 ){
    CV_Error( CV_StsBadArg, "comMask is empty or has incorrect type (not CV_8UC1)" );
  }
  if( binMask.empty() || binMask.rows!=comMask.rows || binMask.cols!=comMask.cols ){
    binMask.create( comMask.size(), CV_8UC1 );
  }
  binMask = comMask & 1;
}
void Segmentor::showImage( Mat& _img, Mat& _mask){
  Mat res;
  Mat binMask;
  if( _mask.empty() ){
    _img.copyTo( res );
  }
  else{
    getBinMask( _mask, binMask );
    _img.copyTo( res, binMask );
  }
  imshow( winName, res );
}

void Segmentor::changeImage( Mat& _img, Mat& _mask, Mat& _res){
  Mat binMask;
  if( _mask.empty() ){
    _img.copyTo( _res );
  }
  else{
    getBinMask( _mask, binMask );
    _img.copyTo( _res, binMask );
  }
}

int Segmentor::segment(char* argv, Mat& _res){
  
  Mat image, mask, bgdModel, fgdModel;
  Rect rect;
  
  // Error handling of inputs
  string filename = argv;
  if(filename.empty()){
    return 1;
  }
  
  image = imread(filename,1);
  if(image.empty()){
    return 1;
  }

  // Show image to user and wait for click
  // imshow(winName, image);
  // waitKey();

  // Perform grabcut on
  rect = Rect(Point(10,10), Point(630,470));
  grabCut(image, mask,rect,bgdModel,fgdModel,iter,GC_INIT_WITH_RECT);

  changeImage(image, mask, _res);
  // waitKey();
  return 0;
}
/*
int Segmentor::getSIFT(char* argv, cv::Mat& _res){
    const cv::Mat input = cv::imread(argv, 0);
    initModule_nonfree();

    cv::SiftFeatureDetector detector;
    std::vector<cv::KeyPoint> keypoints;
    detector.detect(input, keypoints);

    // Add results to image and save.
    cv::drawKeypoints(input, keypoints, _res);

    return 0;
}*/
