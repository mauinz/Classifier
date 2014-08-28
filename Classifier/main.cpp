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
  std::string svm_1 = "/home/matthew/Documents/classifier/Classifier/SVMS_1_2014-08-20.00:34:18";
  std::string svm_2 = "/home/matthew/Documents/classifier/Classifier/SVMS_2_2014-08-20.02:20:15";
  std::string svm_3 = "/home/matthew/Documents/classifier/Classifier/SVMS_3_2014-08-20.04:05:58";
  std::string svm_4 = "/home/matthew/Documents/classifier/Classifier/SVMS_4_2014-08-20.05:51:52";
  std::string svm_5 = "/home/matthew/Documents/classifier/Classifier/SVMS_5_2014-08-20.07:38:21";
  std::string vf1 = "/home/matthew/Documents/classifier/Classifier/Vocabularies/Vocabulary_1_2014-08-19.18:28:23.yml";
  std::string vf2 = "/home/matthew/Documents/classifier/Classifier/Vocabularies/Vocabulary_2_2014-08-19.19:35:40.yml";
  std::string vf3 = "/home/matthew/Documents/classifier/Classifier/Vocabularies/Vocabulary_3_2014-08-19.20:42:59.yml";
  std::string vf4 = "/home/matthew/Documents/classifier/Classifier/Vocabularies/Vocabulary_4_2014-08-19.21:50:30.yml";
  std::string vf5 = "/home/matthew/Documents/classifier/Classifier/Vocabularies/Vocabulary_5_2014-08-19.22:58:06.yml";
  std::string svm_file =  "/home/matthew/Documents/classifier/Classifier/SVMS_12014-08-01.12:33:55";
  std::string test_image = "/home/matthew/Documents/classifier/Classifier/image.tif";
  std::string test_image_2 = "/home/matthew/Desktop/1913939_253854910507_7156307_n.jpg";
  
  std::string seedfile[10];
  seedfile[0] = "/home/matthew/Documents/classifier/Classifier/Seeds/test_seed_0";
  seedfile[1] = "/home/matthew/Documents/classifier/Classifier/Seeds/test_seed_1";
  seedfile[2] = "/home/matthew/Documents/classifier/Classifier/Seeds/test_seed_2";
  seedfile[3] = "/home/matthew/Documents/classifier/Classifier/Seeds/test_seed_3";
  seedfile[4] = "/home/matthew/Documents/classifier/Classifier/Seeds/test_seed_4";
  seedfile[5] = "/home/matthew/Documents/classifier/Classifier/Seeds/test_seed_5";
  seedfile[6] = "/home/matthew/Documents/classifier/Classifier/Seeds/test_seed_6";
  seedfile[7] = "/home/matthew/Documents/classifier/Classifier/Seeds/test_seed_7";
  seedfile[8] = "/home/matthew/Documents/classifier/Classifier/Seeds/test_seed_8";
  seedfile[9] = "/home/matthew/Documents/classifier/Classifier/Seeds/test_seed_9";

  std::string vocabfile[10];
  vocabfile[0] = "/home/matthew/Documents/classifier/Classifier/Vocabularies/Vocabulary_0_2014-08-27.19:24:57.yml";
  vocabfile[1] = "/home/matthew/Documents/classifier/Classifier/Vocabularies/Vocabulary_1_2014-08-27.20:32:47.yml";
  vocabfile[2] = "/home/matthew/Documents/classifier/Classifier/Vocabularies/Vocabulary_2_2014-08-27.21:41:41.yml";
  vocabfile[3] = "/home/matthew/Documents/classifier/Classifier/Vocabularies/Vocabulary_3_2014-08-27.22:49:23.yml";
  vocabfile[4] = "/home/matthew/Documents/classifier/Classifier/Vocabularies/Vocabulary_4_2014-08-27.23:58:47.yml";
  vocabfile[5] = "/home/matthew/Documents/classifier/Classifier/Vocabularies/Vocabulary_5_2014-08-28.01:06:43.yml";
  vocabfile[6] = "/home/matthew/Documents/classifier/Classifier/Vocabularies/Vocabulary_6_2014-08-28.02:15:34.yml";
  vocabfile[7] = "/home/matthew/Documents/classifier/Classifier/Vocabularies/Vocabulary_7_2014-08-28.03:22:33.yml";
  vocabfile[8] = "/home/matthew/Documents/classifier/Classifier/Vocabularies/Vocabulary_8_2014-08-28.04:30:38.yml";
  vocabfile[9] = "/home/matthew/Documents/classifier/Classifier/Vocabularies/Vocabulary_9_2014-08-28.05:36:45.yml";
  
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
       for(int i = 0; i < 10; i++){
      myclas->getWords(seedfile[i], i, verbose);
    }
  }
  
  // makeFileList() TEST
  else if(((std::string)"makeFileList").compare(argv[1]) == 0){
    myclas->makeFileList(img_folder,1);
    myclas->makeFileList(img_folder,2);
    myclas->makeFileList(img_folder,3);
    myclas->makeFileList(img_folder,4);
    myclas->makeFileList(img_folder,5);
  }
  // make file list for kfold test
  else if(((std::string)"kfoldfiles").compare(argv[1]) == 0){
    myclas->kFoldFileList(img_folder,1);
    cout << "fine" << endl;
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
    myclas->testSVM(sf1,vf1,svm_1,1);
    myclas->testSVM(sf2,vf2,svm_2,2);
    myclas->testSVM(sf3,vf3,svm_3,3);
    myclas->testSVM(sf4,vf4,svm_4,4);
    myclas->testSVM(sf5,vf5,svm_5,5);
  }
  
  // Run FULL TEST AND SAVE RESULTS
  else if(((std::string)"FULLParams").compare(argv[1]) == 0){

    verbose = true;
    for(int i = 0; i < 10; i++){
      std::string svm = myclas->trainSVMParams(vocabfile[i],seedfile[i],i,verbose);
      myclas->testSVM(seedfile[i],vocabfile[i],svm,i,verbose);
    }
  }
  
  else if(((std::string)"hist").compare(argv[1]) == 0){
    Segmentor *myseg = new Segmentor;
    
    //Mat img1 = imread("/home/matthew/Documents/Data/UK-Leps.images/Euphydryas-aurinia/Euphydryas-aurinia-01.tif",1), mask1, res1;
    Mat img1 = imread("/home/matthew/Documents/Data/UK-Leps.images/Coenonympha-pamphilus/Coenonympha-pamphilus-01.tif",1), mask1, res1;

    myseg->getMask(img1,mask1);
    imshow("name",mask1);
    waitKey(0);
    myclas->getHistPyramid(img1, res1, mask1, true);
    /*
    Mat img2 = imread("/home/matthew/Documents/Data/UK-Leps.images/Coenonympha-pamphilus/Coenonympha-pamphilus-01.tif",1), mask2, res2;

    myseg->getMask(img2,mask2);
    myclas->getHist(img2, res2, mask2, true);
    delete myseg;
    */
  }
  
  cout << "still fine" << endl;  
  delete myclas;
  return 0;
}
