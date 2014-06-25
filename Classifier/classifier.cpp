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
  cv::SurfDescriptorExtractor extractor;
  cv::SurfFeatureDetector detector(500);
  std::vector<cv::KeyPoint> keypoints;
  cv::Mat training_descriptors(1,extractor.descriptorSize(),extractor.descriptorType());
  std::cout << extractor.descriptorType() << std::endl;
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

  //Save descriptors
  std::cout << "Saving Descriptors" << std::endl;
  std::string save_des = "Descriptors_";
  time_t     now = time(0);
  struct tm  tstruct;
  char       buf[80];
  tstruct = *localtime(&now);
  strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
  save_des += buf;
  save_des += ".yml";
  std::cout << "Saving descriptors as: " << save_des << std::endl;
  FileStorage fs1(save_des, FileStorage::WRITE);
  fs1 << "training_descriptors" << training_descriptors;
  fs1.release();

  // Find vocabulary
  cv::BOWKMeansTrainer bowtrainer(1000); //num clusters
  bowtrainer.add(training_descriptors);
  std::cout << "clustering features" << std::endl;
  cv::Mat vocabulary = bowtrainer.cluster();
  
  // Save vocabulary
  std::cout << "Saving vocabulary" << std::endl;
  std::string save_voc = "Vocabulary_";
  save_voc += buf;
  save_voc += ".yml";
  std::cout << "Saving vocabulary as: " << save_voc << std::endl;
  FileStorage fs2(save_voc, FileStorage::WRITE);
  fs2 << "vocabulary" << vocabulary;
  fs2.release();
  return 0;
}
