#include "classifier.hpp"
#include "../Segmentor/segmentor.hpp"
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>



int main(int argc,  char** argv){
  
  std::string img_folder = "/home/matthew/Documents/Data/UK-Leps.images";
  std::string seed_file = "/home/matthew/Documents/classifier/Classifier/Seeds/test_seed_1";
  std::string sf2 = "/home/matthew/Documents/classifier/Classifier/Seeds/test_seed_2";
  std::string sf3 = "/home/matthew/Documents/classifier/Classifier/Seeds/test_seed_3";
  std::string sf4 = "/home/matthew/Documents/classifier/Classifier/Seeds/test_seed_4";
  std::string sf5 = "/home/matthew/Documents/classifier/Classifier/Seeds/test_seed_5";
  std::string vocab_file = "/home/matthew/Documents/classifier/Classifier/Vocabularies/Vocabulary_1_2014-07-16.18:03:34.yml";
  std::string svm_file =  "/home/matthew/Documents/classifier/Classifier/SVMS_12014-08-01.12:33:55";
  std::string test_image = "/home/matthew/Documents/classifier/Classifier/image.tif";
  std::string test_image_2 = "/home/matthew/Desktop/1913939_253854910507_7156307_n.jpg";
  bool verbose = false;
  int seed = 1;
  
  Classifier* myclas = new Classifier;

  if(argc == 1){
    return 0;
  }
  
  if(argc > 2){
    if(((std::string)"verbose").compare(argv[2]) == 0){
      verbose = true;
    }
  }
  
  // getWords() TEST
  if(((std::string)"getWords").compare(argv[1]) == 0){
    myclas->getWords(seed_file, seed, verbose);
  }
  
  // makeFileList() TEST
  else if(((std::string)"makeFileList").compare(argv[1]) == 0){
    myclas->makeFileList(img_folder,1);
    myclas->makeFileList(img_folder,2);
    myclas->makeFileList(img_folder,3);
    myclas->makeFileList(img_folder,4);
    myclas->makeFileList(img_folder,5);
  }

  //checkFolders() TEST
  else if(((std::string)"checkFolders").compare(argv[1]) == 0){
    myclas->checkFolders(img_folder);
  }
  
  // Setting up SVM TEST
  else if(((std::string)"trainSVM").compare(argv[1]) == 0){
    myclas->trainSVM(vocab_file,seed_file,seed);
  }
  
  // Testing SVM results
  else if(((std::string)"testSVM").compare(argv[1]) == 0){
    myclas->testSVM(seed_file,vocab_file,svm_file,seed);
  }
  
  // Run FULL TEST AND SAVE RESULTS
  else if(((std::string)"trainSVM").compare(argv[1]) == 0){
    
    verbose = true;
    std::string svm_1 = myclas->trainSVM(vocab_file,seed_file,1,verbose);
    std::string svm_2 = myclas->trainSVM(vocab_file,sf2,2,verbose);
    std::string svm_3 = myclas->trainSVM(vocab_file,sf3,3,verbose);
    std::string svm_4 = myclas->trainSVM(vocab_file,sf4,4,verbose);
    std::string svm_5 = myclas->trainSVM(vocab_file,sf5,5,verbose);

    myclas->testSVM(seed_file,vocab_file,svm_file,seed,verbose);
    myclas->testSVM(sf2,vocab_file,svm_file,2,verbose);
    myclas->testSVM(sf3,vocab_file,svm_file,3,verbose);
    myclas->testSVM(sf4,vocab_file,svm_file,4,verbose);
    myclas->testSVM(sf5,vocab_file,svm_file,5,verbose);
  }

  delete myclas;
  return 0;
}
