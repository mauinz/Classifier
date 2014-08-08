#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <sstream>
#include "segmentor.hpp"
#include "SLIC/slic.h"

using namespace std;
using namespace cv;

const string winName = "Image";
const Scalar RED = Scalar(0,0,255);
const Scalar BLUE = Scalar(255,0,0);
const int iter = 1;

Segmentor::Segmentor(){}
Segmentor::~Segmentor(){}

//=======================================================================================
void Segmentor::getBinMask( const Mat& comMask, Mat& binMask ){
//=======================================================================================
  if( comMask.empty() || comMask.type()!=CV_8UC1 ){
    CV_Error( CV_StsBadArg, "comMask is empty or has incorrect type (not CV_8UC1)" );
  }
  if( binMask.empty() || binMask.rows!=comMask.rows || binMask.cols!=comMask.cols ){
    binMask.create( comMask.size(), CV_8UC1 );
  }
  binMask = comMask & 1;
}
//=======================================================================================
void changeMask( Mat& mask, vector<Point> bgdPixels, vector<Point> fgdPixels )
//=======================================================================================
{
    vector<Point>::const_iterator it = bgdPixels.begin(), itEnd = bgdPixels.end();
    for( ; it != itEnd; ++it )
        mask.at<uchar>(*it) = GC_BGD;
    it = fgdPixels.begin(), itEnd = fgdPixels.end();
    for( ; it != itEnd; ++it )
        mask.at<uchar>(*it) = GC_FGD;
}
//=======================================================================================
void Segmentor::showImage( Mat& _img, Mat& _mask){
//=======================================================================================
  Mat res;
  Mat binMask;
  if( _mask.empty() ){
    _img.copyTo( res );
  }
  else{
    getBinMask( _mask, binMask );
    _img.copyTo( res, binMask );
  }
  imshow( winName, res );
}
//=======================================================================================
void Segmentor::changeImage( Mat& _img, Mat& _mask, Mat& _res){
//=======================================================================================
  Mat binMask;
  if( _mask.empty() ){
    _img.copyTo( _res );
  }
  else{
    getBinMask( _mask, binMask );
    _img.copyTo( _res, binMask );
  }
}

// filename = path to image file
// _res = image after segmentation
//=======================================================================================
int Segmentor::segment(std::string filename, Mat& _res){
//=======================================================================================
  Mat image, mask, bgdModel, fgdModel;
  Rect rect;
  
  // Error handling of inputs

  if(filename.empty()){
    return 1;
  }
  
  image = imread(filename,1);
  if(image.empty()){
    return 1;
  }

  // Show image to user and wait for click
  // imshow(winName, image);
  // waitKey();

  // Perform grabcut on
  int max_x = image.cols, max_y = image.rows;
  rect = Rect(Point(max_x*(.05),max_y*(.05)), Point(max_x*(.95),max_y*(.95)));
  grabCut(image, mask,rect,bgdModel,fgdModel,iter,GC_INIT_WITH_RECT);
  
  changeImage(image, mask, _res);
  // waitKey();
  return 0;
}

//=======================================================================================
void Segmentor::getMask(cv::Mat& image, cv::Mat& mask){
//=======================================================================================
  Mat comMask, bgdModel, fgdModel, res;
  Rect rect;
  vector<Point> fgdPxls, bgdPxls;
  
  const int dx[8] = {-1, -1, 0, 1, 1,  1,  0, -1};
  const int dy[8] = { 0,  1, 1, 1, 0, -1, -1, -1};

  // Show image to user and wait for click
  // imshow(winName, image);
  // waitKey();

  // Perform grabcut on
  int max_x = image.cols, max_y = image.rows;
  for(int i = 0; i < 32; i++){
    for(int j = 0; j < 32; j++){
      Point p1((int)(max_x*i/32),(int)(max_y*j/32));
      bgdPxls.push_back(p1);
    } 
  }
  
  rect = Rect(Point(max_x*(.05),max_y*(.05)), Point(max_x*(.95),max_y*(.95)));
  grabCut(image, comMask,rect,bgdModel,fgdModel,1,GC_INIT_WITH_RECT);
  changeMask( comMask, bgdPxls, fgdPxls );
  grabCut(image, comMask, rect, bgdModel, fgdModel, 2, GC_INIT_WITH_MASK );
  
  vector<Point>::const_iterator it = bgdPxls.begin(), itEnd = bgdPxls.end();
  int x,y;
  for( ; it != itEnd; ++it ){
    for(int k = 0; k < 8; k++){
      x = it->x + dx[k];
      y = it->y + dy[k];
      if(x > 0 && x <max_x && y > 0 && y <max_y && 
	 (comMask.at<uchar>(y,x) == GC_BGD || 
	  comMask.at<uchar>(y,x) == GC_PR_BGD)){
	break;
      }
      else if(k == 7){
	comMask.at<uchar>(*it) = GC_FGD;
      }
    }
  }

  getBinMask(comMask,mask);
  changeImage(image, mask, res);
  imshow(winName,res);
  imwrite("final.jpg",res);
  waitKey(0);
  
  //std::cout <<"GC_BGD:"<< GC_BGD << std::endl;
  //changeImage(image, mask, _res);
  //waitKey();
  //return 0;
}

