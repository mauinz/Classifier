#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/nonfree.hpp>
#include <boost/filesystem.hpp>
#include <time.h>
#include <string>
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

int Classifier::getWords(const char * folderpath){
  cv::initModule_nonfree();
  //Set up components
  cv::SiftDescriptorExtractor extractor;
  cv::SiftFeatureDetector detector(500);
  std::vector<cv::KeyPoint> keypoints;
  cv::Mat training_descriptors(1,extractor.descriptorSize(),extractor.descriptorType());
  cv::Mat descriptors;

  //get file names and containing folder
  std::cout << "Opening files and extracting features" << std::endl;
  for ( boost::filesystem::recursive_directory_iterator end, dir(folderpath); 
       dir != end; ++dir ) {
    if(boost::filesystem::is_regular_file(*dir) && boost::filesystem::extension(*dir)== ".tif" ){
      std::cout << "Reading: " << dir->path().string() << std::endl;
      Mat img = imread(dir->path().string());
      std::cout << "Detecting" << std::endl;
      detector.detect(img, keypoints);
      std::cout << "Extracting" << std::endl;
      extractor.compute(img,keypoints,descriptors);
      training_descriptors.push_back(descriptors);
    }
  }
  
  cout << "Total descriptors: " << training_descriptors.rows << endl;

  cv::BOWKMeansTrainer bowtrainer(1000); //num clusters
  bowtrainer.add(training_descriptors);
  std::cout << "clustering features" << std::endl;
  cv::Mat vocabulary = bowtrainer.cluster();
  
  std::cout << "Saving vocabulary" << std::endl;
  std::string save_name = "Vocabulary_";

  time_t     now = time(0);
  struct tm  tstruct;
  char       buf[80];
  tstruct = *localtime(&now);
  strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
  save_name += buf;
  save_name += ".yml";
  std::cout << "Saving vocabulary as: " << save_name << std::endl;
  FileStorage fs1(save_name, FileStorage::WRITE);
  fs1 << "vocabulary" << vocabulary;
  fs1.release();
  return 0;
}
