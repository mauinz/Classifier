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
#include <iomanip>
#include "classifier.hpp"
#include <boost/lambda/bind.hpp>

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

int Classifier::makeFileList(const char * folderpath){

  std::vector<std::string> folderlist;
  std::vector<std::vector<string> > image_split;
  int count, folder_count= -1;
  srand (2);
  for ( boost::filesystem::recursive_directory_iterator end, dir(folderpath); 
	dir != end; ++dir ) {
    if(!boost::filesystem::is_regular_file(*dir)){
      folderlist.push_back(dir->path().string());
    }
  }
  
  int test_count[folderlist.size()];
  
  for(unsigned int i = 0; i < folderlist.size(); i++){
    count = 0;
    for( boost::filesystem::recursive_directory_iterator end, dir(folderlist[i]); dir!= end; dir++){
      count++;
    }
    //std::cout << folderlist[i] << ": " << count << std::endl;
    test_count[i] = rand() % count;
    //std::cout << test_count[i] << std::endl;
  }
  
  for ( boost::filesystem::recursive_directory_iterator end, dir(folderpath); 
	dir != end; ++dir) {
    if(!boost::filesystem::is_regular_file(*dir)){
      folder_count++;
      count = 0;
    }
    if(boost::filesystem::is_regular_file(*dir)){
      std::vector<string> tmp;
      tmp.push_back(dir->path().string());
      tmp.push_back(dir->path().parent_path().filename().string());
      if(test_count[folder_count] == count){
	tmp.push_back("test");
      }
      else{
	tmp.push_back("train");
      }
      image_split.push_back(tmp);
      count++;
      //std::cout <<test_count[folder_count] << ":"<< count << std::endl;
    }
  }
  
  print2Dvector(image_split);
  
  return 0;
  
}

void Classifier::checkFolders(const char * folderpath){
  bool need_checking = false;
  for ( boost::filesystem::recursive_directory_iterator end, dir(folderpath); 
       dir != end; ++dir ) {
    if(boost::filesystem::is_regular_file(*dir) &&!(boost::filesystem::extension(*dir)== ".tif") ){
      std::cout << "Please check file: " << *dir << std::endl;
      need_checking = true;
    }
  }
  if(!need_checking){
    std::cout << "All files are as expected" << std::endl;
  }
}

void Classifier::print2Dvector(std::vector<std::vector<string> > print){
  for(unsigned int i = 0; i < print.size();i++){
    for(unsigned int j = 0; j < print[i].size(); j++){
      std::cout << std::setw(10) << print[i][j];
    }
    std::cout << std::endl;
  }

}
