#ifndef _CLASSIFIER_H_
#define _CLASSIFIER_H_

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "../Segmentor/segmentor.hpp"
#include <map>
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/highgui.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/ml/ml.hpp>
#include <boost/filesystem.hpp>
#include <time.h>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <boost/algorithm/string.hpp>
#include "classifier.hpp"
#include "../Segmentor/segmentor.hpp"
#include <boost/lambda/bind.hpp>
#include <memory>
#include <map>

using namespace std;
using namespace cv;


class Classifier{
private:
  cv::Ptr<FeatureDetector> detector;
  cv::Ptr<DescriptorExtractor > extractor;
  cv::Ptr<DescriptorMatcher > matcher;
  Segmentor * myseg;
  void print2Dvector(std::vector<std::vector<std::string> > print);
  void save2Dvector(std::vector<std::vector<std::string> > print, int seed);
  void load2Dvector(std::vector<std::vector<std::string> > &print,std::string file_path);
public:
  // Constructor
  Classifier();
  // Destructor
  ~Classifier();
  // Main functionality of the class
  
  void getFeatures(cv::Mat img,cv::Mat mask, cv::Mat &res, BOWImgDescriptorExtractor * bowide);
  int getWords(std::string folderpath,int seed, bool verbose = false);
  int makeFileList(std::string folderpath, int seed);
  int kFoldFileList(std::string folderpath, int seed);
  void checkFolders(std::string folderpath);
  void extractTrainingData(std::string filepath, std::map<std::string,cv::Mat>& classes_training_data, cv::Mat vocabulary, int seed, bool verbose = false);
  std::string trainSVM(std::string vocab_path, std::string train_path, int seed, bool verbose = false);
  std::string trainSVMParams(std::string vocab_path, std::string train_path, int seed, bool verbose = false);
  void testSVM(std::string seed_path, std::string vocab_path, std::string svm_path, int seed, bool verbose = false);
  void getHist(cv::Mat src, cv::Mat &res, cv::Mat mask, bool verbose = false);
  void getHistPyramid(cv::Mat src, cv::Mat &res, cv::Mat mask, bool verbose = false);
  void getmoments(cv::Mat binmask, double hu[7]);
  void pcaImage(cv::Mat img, cv::Mat eigenvalues, cv::Mat eigenvectors, cv::Mat &res);
};

#endif
