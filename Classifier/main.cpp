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
  
  // getWords() TEST
  
  Classifier* myclas = new Classifier;
  //for testing purposes use:
  //"/home/matthew/Documents/Data/UK-Leps.images/Boloria-selene/"
  //myclas->getWords("/home/matthew/Documents/Data/UK-Leps.images");
  myclas->getWords("/home/matthew/Documents/classifier/Classifier/Seeds/test_seed_1", 1, true);
  delete myclas;
  return 0;
  


  // makeFileList() TEST
  /*
  Classifier* myclas = new Classifier;
  myclas->makeFileList("/home/matthew/Documents/Data/UK-Leps.images",1);
  myclas->makeFileList("/home/matthew/Documents/Data/UK-Leps.images",2);
  myclas->makeFileList("/home/matthew/Documents/Data/UK-Leps.images",3);
  myclas->makeFileList("/home/matthew/Documents/Data/UK-Leps.images",4);
  myclas->makeFileList("/home/matthew/Documents/Data/UK-Leps.images",5);
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
  /*
  Classifier* myclas = new Classifier;
  myclas->trainSVM("/home/matthew/Documents/classifier/Classifier/Vocabularies/Vocabulary_1_2014-07-15.14:39:52.yml",
		   "/home/matthew/Documents/classifier/Classifier/Seeds/test_seed_1", 1);
  delete myclas;
  return 0;
  */


  // Testing SVM results
  /*
  Classifier* myclas = new Classifier;
  myclas->testSVM("/home/matthew/Documents/classifier/Classifier/Seeds/test_seed_1",
		  "/home/matthew/Documents/classifier/Classifier/Vocabularies/Vocabulary_2014-07-03.16:10:49.yml",
		  "/home/matthew/Documents/classifier/Classifier/SVMS");
  delete myclas;
  return 0;
  */
}
