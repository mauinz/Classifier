#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <sstream>
using namespace std;
using namespace cv;

const string winName = "Image";
const Scalar RED = Scalar(0,0,255);
const Scalar BLUE = Scalar(255,0,0);
const int iter = 7;

void getBinMask( const Mat& comMask, Mat& binMask ){
  if( comMask.empty() || comMask.type()!=CV_8UC1 ){
    CV_Error( CV_StsBadArg, "comMask is empty or has incorrect type (not CV_8UC1)" );
  }
  if( binMask.empty() || binMask.rows!=comMask.rows || binMask.cols!=comMask.cols ){
    binMask.create( comMask.size(), CV_8UC1 );
  }
  binMask = comMask & 1;
}
void showImage( Mat& _img, Mat& _mask)
{
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

int main(int argc, char** argv){
  
  vector<Point> fgdPxls, bgdPxls;
  Mat image, mask, bgdModel, fgdModel;
  Rect rect;
  
  // Error handling of inputs
  if(argc == 1){
    return 1;
  }
  string filename = argv[1];
  if(filename.empty()){
    return 1;
  }
  
  image = imread(filename,1);
  if(image.empty()){
    return 1;
  }

  // Show image to user and wait for click
  imshow(winName, image);
  waitKey();

  // Perform grabcut on
  rect = Rect(Point(10,10), Point(630,470));
  grabCut(image, mask,rect,bgdModel,fgdModel,iter,GC_INIT_WITH_RECT);

  showImage(image, mask);
  waitKey();
  
}
