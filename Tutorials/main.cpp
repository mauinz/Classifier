#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <sstream>


int main(int argc, char** argv){
  IplImage* image = cvLoadImage(argv[1]);
  cvNamedWindow("viewer");
  cvShowImage("viewer", image);
  cvWaitKey(0);
  cvReleaseImage(&image);
  cvDestroyWindow("viewer");
}
