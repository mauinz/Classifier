#ifndef _AUGMENTOR_H_
#define _AUGMENTOR_H_

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <map>
#include <string>

class Augmentor{
private:

public:
  // Constructor
  Augmentor();
  // Destructor
  ~Augmentor();
  // Main functionality of the class
  void print2Dvector(std::vector<std::vector<std::string> > print);
  void save2Dvector(std::vector<std::vector<std::string> > print, int seed);
  void load2Dvector(std::vector<std::vector<std::string> > &print,std::string file_path);
  void augImage(std::string image_path);
  cv::Mat asRowMatrix(const std::vector<cv::Mat>& src, int rtype, double alpha, double beta );
  cv::Mat norm_0_255(const cv::Mat& src);

};

#endif