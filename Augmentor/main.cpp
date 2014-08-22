#include "augmentor.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main(){
  Augmentor * myAug = new Augmentor;
  cv::Mat res;
  std::string seed[5];
  seed[0] = "/home/matthew/Documents/classifier/Classifier/Seeds/test_seed_1";
  seed[1] = "/home/matthew/Documents/classifier/Classifier/Seeds/test_seed_2";
  seed[2] = "/home/matthew/Documents/classifier/Classifier/Seeds/test_seed_3";
  seed[3] = "/home/matthew/Documents/classifier/Classifier/Seeds/test_seed_4";
  seed[4] = "/home/matthew/Documents/classifier/Classifier/Seeds/test_seed_5";
  int num_components = 3;


  // Perform a PCA:
  for(int i = 1; i < 6; i++){
    myAug->collectData(seed[i-1],res);
    PCA pca(res, Mat(), PCA::DATA_AS_ROW, num_components);

    Mat mean = pca.mean.clone();
    Mat eigenvalues = pca.eigenvalues.clone();
    Mat eigenvectors = pca.eigenvectors.clone();
    std::cout << mean << std::endl;
    std::cout << eigenvalues << std::endl;
    std::cout << eigenvectors << std::endl;
  
    //Write matrices to pca_happy.yml
    std::string save_path = "pca_" + to_string(i) + ".yml";
    FileStorage fs(save_path, FileStorage::WRITE);
    fs << "Eigenvalues" << eigenvalues;
    fs << "Eigenvector" << eigenvectors;
    fs.release();
    res.release();
  }
  
  //cout << res.at<int>(0,0);
  delete myAug;
  return 0;
}

