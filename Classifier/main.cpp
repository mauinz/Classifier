#include "classifier.hpp"
#include "../Segmentor/segmentor.hpp"
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>



int main(int argc,  char** argv){
  
  std::string img_folder = "/home/matthew/Documents/Data/UK-Leps.images";
  std::string seed_file = "/home/matthew/Documents/classifier/Classifier/Seeds/test_seed_1";
  std::string sf1 = "/home/matthew/Documents/classifier/Classifier/Seeds/test_seed_1";
  std::string sf2 = "/home/matthew/Documents/classifier/Classifier/Seeds/test_seed_2";
  std::string sf3 = "/home/matthew/Documents/classifier/Classifier/Seeds/test_seed_3";
  std::string sf4 = "/home/matthew/Documents/classifier/Classifier/Seeds/test_seed_4";
  std::string sf5 = "/home/matthew/Documents/classifier/Classifier/Seeds/test_seed_5";
  std::string vocab_file = "/home/matthew/Documents/classifier/Classifier/Vocabularies/Vocabulary_1_2014-07-16.18:03:34.yml";
  std::string vf1 = "/home/matthew/Documents/classifier/Classifier/Vocabularies/Vocabulary_1_2014-08-05.21:34:18.yml";
  std::string vf2 = "/home/matthew/Documents/classifier/Classifier/Vocabularies/Vocabulary_2_2014-08-05.22:33:08.yml";
  std::string vf3 = "/home/matthew/Documents/classifier/Classifier/Vocabularies/Vocabulary_3_2014-08-05.23:32:26.yml";
  std::string vf4 = "/home/matthew/Documents/classifier/Classifier/Vocabularies/Vocabulary_4_2014-08-06.00:33:06.yml";
  std::string vf5 = "/home/matthew/Documents/classifier/Classifier/Vocabularies/Vocabulary_5_2014-08-06.01:32:53.yml";
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
    myclas->getWords(sf1, 1, verbose);
    myclas->getWords(sf2, 2, verbose);
    myclas->getWords(sf3, 3, verbose);
    myclas->getWords(sf4, 4, verbose);
    myclas->getWords(sf5, 5, verbose);
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

  else if(((std::string)"trainParams").compare(argv[1]) == 0){
    std::string svm = myclas->trainSVMParams(vf1,sf1,1,verbose);
    myclas->testSVM(sf1,vf1,svm,1,verbose);
  }
  
  // Testing SVM results
  else if(((std::string)"testSVM").compare(argv[1]) == 0){
    myclas->testSVM(seed_file,vocab_file,svm_file,seed);
  }
  
  // Run FULL TEST AND SAVE RESULTS
  else if(((std::string)"FULLParams").compare(argv[1]) == 0){

    verbose = true;
    std::string svm_1 = myclas->trainSVMParams(vf1,sf1,1,verbose);
    myclas->testSVM(sf1,vf1,svm_1,1,verbose);

    std::string svm_2 = myclas->trainSVMParams(vf2,sf2,2,verbose);
    myclas->testSVM(sf2,vf2,svm_2,2,verbose);

    std::string svm_3 = myclas->trainSVMParams(vf3,sf3,3,verbose);
    myclas->testSVM(sf2,vf3,svm_3,3,verbose);

    std::string svm_4 = myclas->trainSVMParams(vf4,sf4,4,verbose);
    myclas->testSVM(sf2,vf4,svm_4,4,verbose);

    std::string svm_5 = myclas->trainSVMParams(vf5,sf5,5,verbose);
    myclas->testSVM(sf2,vf5,svm_5,5,verbose);
  }

    

  delete myclas;
  return 0;
}
