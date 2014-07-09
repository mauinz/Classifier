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
  slic.label_pixels(lab_image,crap);
  //cv::imshow(winName, image);
  //cv::waitKey();
  //cv::imshow(winName, res);
  //cv::waitKey();
  
  //slic.display_contours(iplimage, CV_RGB(255,0,0));
  //cvShowImage("result", iplimage);
  //cvWaitKey(0);
  
  delete mySeg;
  return 0;

}
