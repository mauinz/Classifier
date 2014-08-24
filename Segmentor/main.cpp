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
  //WHOLE MAIN FUNCTION JUST FOR TESTING, NOT TO BE RUN AS ./main

  /*
  Segmentor *mySeg = new Segmentor;
  cv::Mat res,image,crap;
  std::map<int,cv::Mat> spixels;
  mySeg->segment("image.tif",res);
  image = cv::imread("image.tif");
  
  IplImage tmp = res;
  IplImage *iplimage = &tmp;
  IplImage *lab_image = cvCloneImage(iplimage);
  cvCvtColor(iplimage, lab_image, CV_BGR2Lab);
  int w = iplimage->width, h = iplimage->height;
  int no_super_pixs = 400;
  int weight = 20;
  double step = sqrt((w * h) / (double) no_super_pixs);

  
  Slic slic;
  
  slic.generate_superpixels(lab_image, step, weight);
  slic.create_connectivity(lab_image);
  //slic.print_clusters(lab_image);
  slic.label_pixels(lab_image,spixels);
  //cv::imshow(winName, iplimage);
  CvScalar mean = slic.get_mean(image,spixels[1]);
  std::cout << mean.val[0] << std::endl;
  std::cout << mean.val[1] << std::endl;
  std::cout << mean.val[2] << std::endl;
  cv::Mat con = slic.get_contour(spixels[1]);

  for(int i = 0; i < 36; i++){
    if(i%6 == 0){
      std::cout << std::endl;
    }
    std::cout << con.at<int>(0,i) << " ";
  }
  cv::Mat con2 = slic.get_contour(spixels[302]);

  for(int i = 0; i < 36; i++){
    if(i%6 == 0){
      std::cout << std::endl;
    }
    std::cout << con2.at<int>(0,i) << " ";
  }
  //cv::imshow(winName, image);
  cv::waitKey();
  //cv::imshow(winName, res);
  //cv::waitKey();
  
  //slic.display_contours(iplimage, CV_RGB(255,0,0));
  //cvShowImage("result", iplimage);
  //cvWaitKey(0);
  
  delete mySeg;
  return 0;
  */
  /*
  Segmentor *mySeg = new Segmentor;
  cv::Mat img = cv::imread("../Grabcut/binmask.jpg",0), mask;
  imshow(winName,img);
  cv::waitKey(0);
  mySeg->getMask(img,mask);
  imshow(winName,mask);
  cout << img.at<uchar>(0, 0) <<std::endl;
  cout << img.at<uchar>(200, 200) << std::endl;
  
  cv::waitKey(0);
  */
  //std::string seed_path = "/home/matthew/Documents/classifier/Classifier/Seeds/test_seed_1";
  //std::vector<std::vector<string> > test_images;  
  Segmentor *mySeg = new Segmentor;
  
  /*
  load2Dvector(test_images, seed_path);

  for(unsigned int i = 0; i < test_images.size(); i++) {
    if(test_images[i][2] == "test"){
      std::string save_path = "/home/matthew/Dropbox/Computer Science/Classifier/Thesis/images/Segmented/"
	+ test_images[i][1] + ".jpg";
      cv::Mat img = cv::imread(test_images[i][0]), res, res_img;
      mySeg->getMask(img,res);
      mySeg->changeImage(img, res, res_img);
      cv::imwrite(save_path, res_img);
      std::cout << "Writing: " << save_path << std::endl;
    }
  }
  ********/


      cv::Mat img = cv::imread("google.jpg"), res, res_img;
      mySeg->getMask(img,res);
      mySeg->changeImage(img, res, res_img);
      cv::imwrite("google_segmented.jpg", res_img);
      
    
  

  delete mySeg;
}
