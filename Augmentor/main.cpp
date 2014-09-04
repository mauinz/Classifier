#include "augmentor.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <math.h>
#include <random>
using namespace std;
using namespace cv;

int main(){
  
  Augmentor * myAug = new Augmentor;
  
  cv::Mat res;
  std::string seed[10];
  seed[0] = "/home/matthew/Documents/classifier/Classifier/Seeds/test_seed_0";
  seed[1] = "/home/matthew/Documents/classifier/Classifier/Seeds/test_seed_1";
  seed[2] = "/home/matthew/Documents/classifier/Classifier/Seeds/test_seed_2";
  seed[3] = "/home/matthew/Documents/classifier/Classifier/Seeds/test_seed_3";
  seed[4] = "/home/matthew/Documents/classifier/Classifier/Seeds/test_seed_4";
  seed[5] = "/home/matthew/Documents/classifier/Classifier/Seeds/test_seed_5";
  seed[6] = "/home/matthew/Documents/classifier/Classifier/Seeds/test_seed_6";
  seed[7] = "/home/matthew/Documents/classifier/Classifier/Seeds/test_seed_7";
  seed[8] = "/home/matthew/Documents/classifier/Classifier/Seeds/test_seed_8";
  seed[9] = "/home/matthew/Documents/classifier/Classifier/Seeds/test_seed_9";
  //int num_components = 3;


  // Perform a PCA:
  for(int i = 0; i < 10; i++){
    //Collect all the pixel values
    myAug->collectData(seed[i],res);
    //Calculate eigenvalues/vectors
    PCA pca(res, Mat(), PCA::DATA_AS_ROW);
    
    //Save and store values
    Mat mean = pca.mean.clone();
    Mat eigenvalues = pca.eigenvalues.clone();
    Mat eigenvectors = pca.eigenvectors.clone();
    std::cout << mean << std::endl;
    std::cout << eigenvalues << std::endl;
    std::cout << eigenvectors << std::endl;
  
    //Write matrices to pca_seed.yml
    std::string save_path = "pca_" + to_string(i) + ".yml";
    FileStorage fs(save_path, FileStorage::WRITE);
    fs << "Eigenvalues" << eigenvalues;
    fs << "Eigenvector" << eigenvectors;
    fs.release();
    res.release();
  }
  
  delete myAug;
  return 0;
  

}

