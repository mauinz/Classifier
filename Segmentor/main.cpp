#include "segmentor.hpp"
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <boost/algorithm/string.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include "SLIC/slic.h"
const std::string winName = "Image";


//=======================================================================================
void load2Dvector(std::vector<std::vector<string> > &print,std::string file_path){
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


int main(){

  std::string seed_path = "/home/matthew/Documents/classifier/Classifier/Seeds/test_seed_1";
  std::vector<std::vector<string> > test_images;  
  Segmentor *mySeg = new Segmentor;
  
  
  load2Dvector(test_images, seed_path);

  for(unsigned int i = 0; i < test_images.size(); i++) {
    if(test_images[i][2] == "test"){
      std::string save_path = "/home/matthew/Documents/classifier/website/images/"
	+ test_images[i][1] + ".jpg";
      cv::Mat img = cv::imread(test_images[i][0]), res, res_img;
      
      cv::imwrite(save_path, img);
      std::cout << "Writing: " << save_path << std::endl;
    }
  }
  
  delete mySeg;
}
