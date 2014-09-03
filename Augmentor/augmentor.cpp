#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/nonfree.hpp>
#include <opencv2/ml/ml.hpp>
#include <opencv/highgui.h>
#include <boost/filesystem.hpp>
#include <time.h>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <boost/algorithm/string.hpp>
#include "augmentor.hpp"
#include "../Segmentor/segmentor.hpp"
#include <boost/lambda/bind.hpp>
#include <memory>
#include <map>

using namespace std;
using namespace cv;

Augmentor::Augmentor(){}
Augmentor::~Augmentor(){}

template <typename T>
//=======================================================================================
std::string to_string(T value){
//=======================================================================================
  std::ostringstream os ;
  os << value ;
  return os.str() ;
}
//=======================================================================================
void Augmentor::print2Dvector(std::vector<std::vector<string> > print){
//=======================================================================================
  for(unsigned int i = 0; i < print.size();i++){
    for(unsigned int j = 0; j < print[i].size(); j++){
      std::cout << std::setw(10) << print[i][j];
    }
    std::cout << std::endl;
  }
}
//=======================================================================================
void Augmentor::save2Dvector(std::vector<std::vector<string> > print, int seed){
//=======================================================================================
  ofstream myfile;
  std::string save_name = "Seeds/test_seed_";
  save_name += to_string(seed);
  myfile.open(save_name.c_str());

  for(unsigned int i = 0; i < print.size();i++){
    for(unsigned int j = 0; j < print[i].size(); j++){
      myfile << print[i][j];
      if(j != 2){
	myfile << ",";
      }
    }
    myfile << std::endl;
  }
  myfile.close();
}
//=======================================================================================
void Augmentor::load2Dvector(std::vector<std::vector<string> > &print,std::string file_path){
//=======================================================================================
  std::ifstream myfile;
  
  myfile.open(file_path.c_str());
  for( std::string line; getline( myfile, line ); ){
    vector<std::string> tmp_line;
    boost::split(tmp_line,line, boost::is_any_of(","));
    print.push_back(tmp_line);
  }
 
  myfile.close();
}

void Augmentor::collectData(std::string file_path, cv::Mat &res){
  Segmentor * mySeg = new Segmentor;
  std::vector<std::vector<string> > seed_data;
  cv::Mat pixel_matrix = cv::Mat(0,3, CV_8U);
  load2Dvector(seed_data, file_path);

  //For each image, collect pixrl values
  for(unsigned int i = 0; i < seed_data.size(); i++){
    if(seed_data[i][2] == "train"){
      
      std::cout << "Reading: " << seed_data[i][0] << std::endl;
      Mat img = imread(seed_data[i][0]), mask;
      std::cout << "Detecting" << std::endl;
      mySeg->getMask(img, mask);
      for(int i = 0; i < img.cols; i++){
	for(int j = 0; j < img.rows; j++){
	  if((int)mask.at<uchar>(j,i) != 0){
	    
	    cv::Mat tmp = (cv::Mat_<uchar>(1,3) << img.at<cv::Vec3b>(j,i)[0], img.at<cv::Vec3b>(j,i)[0], img.at<cv::Vec3b>(j,i)[0]);
	   
	    pixel_matrix.push_back(tmp);
	  }
	}	
      }
    }
  }
  pixel_matrix.convertTo(res,CV_32F);
  
  delete mySeg;
}
