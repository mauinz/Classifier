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
    myAug->collectData(seed[i],res);
    PCA pca(res, Mat(), PCA::DATA_AS_ROW);

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
  
  
  /*
  cv::Mat img = imread("image.tif",1);
  imshow("window",img);
  cv::waitKey(0);
  int seed = 1;
  Mat eigenvalues, eigenvectors, res;
  
  std::string pca_file = "PCA/pca_" + to_string(seed) + ".yml";
  FileStorage fs(pca_file, FileStorage::READ);
  fs["Eigenvalues"] >> eigenvalues;
  fs["Eigenvector"] >> eigenvectors;
  
  transpose(eigenvalues,eigenvalues);
  
  Mat alpha, rgb, tmp;
  //Vec3b colour;
  
  Mat new_image = Mat::zeros( img.size(), img.type() );

  std::default_random_engine de(time(0));
  std::normal_distribution<double> distribution(0.0,0.1);
  
  alpha = (Mat_<float>(1,3) << distribution(de)/600, distribution(de)/600, distribution(de)/600);
  
  transpose(alpha,alpha);
  //cout << eigenvalues.rows << endl;
  //cout << eigenvalues.cols << endl;
  //cout << eigenvalues.type() << endl;
  //cout << alpha.type() << endl;
  tmp = alpha*eigenvalues;
  transpose(eigenvectors,eigenvectors);
  transpose(tmp,tmp);
  rgb = eigenvectors*tmp;
  
  cout << "b:" << rgb.at<float>(0,0) << " using: " << (int)round(rgb.at<float>(0,0)) << endl;
  cout << "g:" << rgb.at<float>(0,1) << " using: " << (int)round(rgb.at<float>(0,1)) << endl;
  cout << "r:" << rgb.at<float>(0,2) << " using: " << (int)round(rgb.at<float>(0,2)) << endl;
  cout << "alpha:" << alpha << endl;
  cout << "img " << (int)img.at<Vec3b>(0,0)[0] << endl;
  cout << "img " << (int)img.at<Vec3b>(0,0)[1] << endl;
  cout << "img " << (int)img.at<Vec3b>(0,0)[2] << endl;
  //colour.val[1] = (int)rgb.at<double>(0,1);
  //colour.val[2] = (int)rgb.at<double>(0,2);
  for( int y = 0; y < img.rows; y++ ){
    for( int x = 0; x < img.cols; x++ ){
      for( int c = 0; c < 3; c++ ){
	int val = (int)img.at<Vec3b>(y,x)[c] + (int)round(rgb.at<float>(0,c));
	if(val <= 0){
	  new_image.at<Vec3b>(y,x)[c] = (uchar)0;
	}
	else if(val >= 255){
	  new_image.at<Vec3b>(y,x)[c] = (uchar)255;
	}
	else{
	  new_image.at<Vec3b>(y,x)[c] = (uchar)val;
	}
      }
    }
  }
  imshow("window2",new_image);
  cv::waitKey(0);
  return 0;
  */
}

