#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <sstream>
using namespace std;
using namespace cv;

int main(int argc, char** argv){
  Point anchor = Point(-1,-1),minloc,maxloc;
  Mat image = imread(argv[1]);
  Mat grey, thres, morph, kernel, surebg, dist_transform, surefg, unknown;
  double minval, maxval;
  //Convert to grey scale
  cvtColor(image, grey,COLOR_BGR2GRAY);
  threshold(grey,thres,0,255,THRESH_BINARY_INV + THRESH_OTSU);
  morphologyEx(thres,morph,MORPH_OPEN,kernel, anchor, 2);
  dilate(morph,surebg,kernel,anchor,3);
  distanceTransform(morph,dist_transform,CV_DIST_L2,5);
  
  minMaxLoc(dist_transform,&minval,&maxval,&minloc,&maxloc);
  threshold(dist_transform,surefg,0.7*maxval,255,0);
  surefg.convertTo(surefg,surebg.type());
  unknown = surebg - surefg;
  
  //make markers
  //int markers = connectedComponents(surefg);
  //markers++;
  imshow("Image", image);
  waitKey();
  imshow("Image", grey);
  waitKey();
  imshow("Image",thres);
  waitKey();
  imshow("Image",morph);
  waitKey();
  imshow("Surebg",surebg);
  waitKey();
  imshow("dist",dist_transform);
  waitKey();
  imshow("surefg",surefg);
  waitKey();
  imshow("unknown",unknown);
  waitKey();
  
}
