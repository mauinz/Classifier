#ifndef _SEGMENTOR_H_
#define _SEGMENTOR_H_

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class Segmentor{
private:
  void getBinMask( const cv::Mat& comMask, cv::Mat& binMask );
  void changeImage( cv::Mat& _img, cv::Mat& _mask, cv::Mat& _res);
  void showImage( cv::Mat& _img, cv::Mat& _mask);
public:
  // Constructor
  Segmentor();
  // Destructor
  ~Segmentor();
  // Main functionality of the class
  int segment(char* argv, cv::Mat& res);
  int getSIFT(char* argv, cv::Mat& _res);
};

#endif
