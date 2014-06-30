#include "classifier.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

int main(){

  //SIFT TEST
  /*
  Classifier* myclas = new Classifier;
  cv::Mat res;
  myclas->getSIFT("image.tif", res);

  cv::namedWindow( "Display window", cv::WINDOW_AUTOSIZE );
  cv::imshow("Display window", res);
  cv::waitKey();
  delete myclas;
  return 0;
  */
  
  // getVocab() TEST
  /*
  Classifier* myclas = new Classifier;
  //for testing purposes use:
  //"/home/matthew/Documents/Data/UK-Leps.images/Boloria-selene/"
  myclas->getWords("/home/matthew/Documents/Data/UK-Leps.images");
  delete myclas;
  return 0;
  */
  // makeFileList() TEST
  /*
  Classifier* myclas = new Classifier;
  myclas->makeFileList("/home/matthew/Documents/Data/UK-Leps.images");
  delete myclas;
  return 0;
  */
  //checkFolders() TEST
  /*
  Classifier* myclas = new Classifier;
  myclas->checkFolders("/home/matthew/Documents/Data/UK-Leps.images");
  delete myclas;
  return 0;
  */
  
  // Setting up SVM TEST
  
  Classifier* myclas = new Classifier;
  myclas->trainSVM("/home/matthew/Documents/classifier/Classifier/Vocabulary_2014-06-30.14:58:15.yml",
		   "/home/matthew/Documents/classifier/Classifier/test_seed_1");
  delete myclas;
  return 0;
  
}
