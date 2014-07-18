#include "augmentor.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

int main(){
  Augmentor * myAug = new Augmentor;

  myAug->augImage("image.tif");

  delete myAug;
  return 0;
}

