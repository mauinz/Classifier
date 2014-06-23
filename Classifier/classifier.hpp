#ifndef _CLASSIFIER_H_
#define _CLASSIFIER_H_

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class Classifier{
private:

public:
  // Constructor
  Classifier();
  // Destructor
  ~Classifier();
  // Main functionality of the class
  int getSIFT(const char* argv, cv::Mat& _res);
};

#endif
