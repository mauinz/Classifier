#include "classifier.hpp"
#include "../Segmentor/segmentor.hpp"
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>



int main(){
  
  std::string seed_file = "/home/matthew/Documents/classifier/Classifier/Seeds/test_seed_1";
  std::string vocab_file = "/home/matthew/Documents/classifier/Classifier/Vocabularies/Vocabulary_1_2014-07-16.18:03:34.yml";
  std::string svm_file =  "/home/matthew/Documents/classifier/Classifier/SVMS_12014-07-22.17:25:13";
  std::string test_image = "/home/matthew/Documents/classifier/Classifier/image.tif";
  std::string test_image_2 = "/home/matthew/Desktop/1913939_253854910507_7156307_n.jpg";
  int seed = 1;
  

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
  /*
  Classifier* myclas = new Classifier;
  //for testing purposes use:
  //"/home/matthew/Documents/Data/UK-Leps.images/Boloria-selene/"
  //myclas->getWords("/home/matthew/Documents/Data/UK-Leps.images");
  myclas->getWords("/home/matthew/Documents/classifier/Classifier/Seeds/test_seed_1", 1, true);
  delete myclas;
  return 0;
  */


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
  
  Classifier* myclas = new Classifier;
  myclas->trainSVM(vocab_file,seed_file,seed);
  delete myclas;
  return 0;
  


  // Testing SVM results
  /*
  Classifier* myclas = new Classifier;
  myclas->testSVM("/home/matthew/Documents/classifier/Classifier/Seeds/test_seed_1",
		  "/home/matthew/Documents/classifier/Classifier/Vocabularies/Vocabulary_1_2014-07-16.18:03:34.yml",
		  "/home/matthew/Documents/classifier/Classifier/SVMS_12014-07-22.17:25:13",1);
  delete myclas;
  return 0;
  */

  // Testing histogram
  
  /*
  cv::Mat src,res;
  src = cv::imread(test_image);
  Classifier* myclas = new Classifier;
  Segmentor* myseg = new Segmentor;
  myclas->getHist(src,res,myseg,true);
  std::cout << "rows:" << res.rows << std::endl;
  std::cout << "cols:" << res.cols << std::endl;
  delete myclas;
  delete myseg;
  return 0;
  */
}
