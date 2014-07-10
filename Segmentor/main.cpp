#include "segmentor.hpp"
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <string>
#include "SLIC/slic.h"

const std::string winName = "Image";

int main(){
  
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

}
