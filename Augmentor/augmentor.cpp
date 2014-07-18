#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/nonfree.hpp>
#include <opencv2/ml/ml.hpp>
#include <opencv/highgui.h>
#include <boost/filesystem.hpp>
#include <time.h>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <boost/algorithm/string.hpp>
#include "augmentor.hpp"
#include <boost/lambda/bind.hpp>
#include <memory>
#include <map>

using namespace std;
using namespace cv;

Augmentor::Augmentor(){}
Augmentor::~Augmentor(){}

template <typename T>
//=======================================================================================
std::string to_string(T value){
//=======================================================================================
  std::ostringstream os ;
  os << value ;
  return os.str() ;
}
//=======================================================================================
void Augmentor::print2Dvector(std::vector<std::vector<string> > print){
//=======================================================================================
  for(unsigned int i = 0; i < print.size();i++){
    for(unsigned int j = 0; j < print[i].size(); j++){
      std::cout << std::setw(10) << print[i][j];
    }
    std::cout << std::endl;
  }
}
//=======================================================================================
void Augmentor::save2Dvector(std::vector<std::vector<string> > print, int seed){
//=======================================================================================
  ofstream myfile;
  std::string save_name = "Seeds/test_seed_";
  save_name += to_string(seed);
  myfile.open(save_name.c_str());

  for(unsigned int i = 0; i < print.size();i++){
    for(unsigned int j = 0; j < print[i].size(); j++){
      myfile << print[i][j];
      if(j != 2){
	myfile << ",";
      }
    }
    myfile << std::endl;
  }
  myfile.close();
}
//=======================================================================================
void Augmentor::load2Dvector(std::vector<std::vector<string> > &print,std::string file_path){
//=======================================================================================
  std::ifstream myfile;
  
  myfile.open(file_path.c_str());
  for( std::string line; getline( myfile, line ); ){
    vector<std::string> tmp_line;
    boost::split(tmp_line,line, boost::is_any_of(","));
    print.push_back(tmp_line);
  }
 
  myfile.close();
}
Mat Augmentor::asRowMatrix(const vector<Mat>& src, int rtype, double alpha = 1, double beta = 0) {
    // Number of samples:
    size_t n = src.size();
    // Return empty matrix if no matrices given:
    if(n == 0)
        return Mat();
    // dimensionality of (reshaped) samples
    size_t d = src[0].total();
    // Create resulting data matrix:
    Mat data(n, d, rtype);
    // Now copy data:
    for(int i = 0; i < n; i++) {
        //
        if(src[i].empty()) {
            string error_message = format("Image number %d was empty, please check your input data.", i);
            CV_Error(CV_StsBadArg, error_message);
        }
        // Make sure data can be reshaped, throw a meaningful exception if not!
        if(src[i].total() != d) {
            string error_message = format("Wrong number of elements in matrix #%d! Expected %d was %d.", i, d, src[i].total());
            CV_Error(CV_StsBadArg, error_message);
        }
        // Get a hold of the current row:
        Mat xi = data.row(i);
        // Make reshape happy by cloning for non-continuous matrices:
        if(src[i].isContinuous()) {
            src[i].reshape(1, 1).convertTo(xi, rtype, alpha, beta);
        } else {
            src[i].clone().reshape(1, 1).convertTo(xi, rtype, alpha, beta);
        }
    }
    return data;
}

void Augmentor::augImage(std::string image_path){
  cv::Mat image, channels[3];

  image = imread(image_path, CV_LOAD_IMAGE_COLOR);
  namedWindow( "Window", CV_WINDOW_AUTOSIZE );
  imshow("Window",image);
  waitKey();
  split(image,channels);
  imshow("Window",channels[0]);
  waitKey();
  imshow("Window",channels[1]);
  waitKey();
  imshow("Window",channels[2]);
  waitKey();
  std::cout << "image: " << image.type() << std::endl;
  std::cout << "channels: " << channels[0].type() << std::endl;
  std::vector<cv::Mat> channels_vec;
  channels_vec.push_back(channels[0]);
  channels_vec.push_back(channels[1]);
  channels_vec.push_back(channels[2]);
  
  cv::Mat input = asRowMatrix(image, image.type() );
  cv::PCA pca(image, cv::Mat(),CV_PCA_DATA_AS_ROW);
  Mat mean = pca.mean.clone();
  Mat eigenvalues = pca.eigenvalues.clone();
  Mat eigenvectors = pca.eigenvectors.clone();
  Mat test = eigenvalues.row(0)*eigenvectors.row(0) + eigenvalues.row(1)*eigenvectors.row(1) + eigenvalues.row(2)*eigenvectors.row(2);
  //test = test/3;
  imshow("avg", norm_0_255(mean.reshape(1,  image.rows)));

  waitKey();
}
cv::Mat Augmentor::norm_0_255(const cv::Mat& src) {
    // Create and return normalized image:
    Mat dst;
    switch(src.channels()) {
    case 1:
        cv::normalize(src, dst, 0, 255, NORM_MINMAX, CV_8UC1);
        break;
    case 3:
        cv::normalize(src, dst, 0, 255, NORM_MINMAX, CV_8UC3);
        break;
    default:
        src.copyTo(dst);
        break;
    }
    return dst;
}
/*

// vocab_path = Path of vocabulary file
// train_path = Path of seed file
//=======================================================================================
void Classifier::trainSVM(std::string vocab_path, std::string train_path, int seed){
//=======================================================================================
  cv::initModule_nonfree();
  cout << "Training SVM" << endl;
  cout << "reading vocabulary from file: "<< vocab_path <<endl;
  Mat vocabulary;
  FileStorage fs(vocab_path, FileStorage::READ);
  fs["vocabulary"] >> vocabulary;
  fs.release();	
  
  Ptr<FeatureDetector > detector(new SurfFeatureDetector()); //detector
  Ptr<DescriptorExtractor > extractor(
				      new OpponentColorDescriptorExtractor(
									   Ptr<DescriptorExtractor>(new SurfDescriptorExtractor())
									   )
				      );
  Ptr<DescriptorMatcher > matcher(new BFMatcher);
  BOWImgDescriptorExtractor bowide(extractor,matcher);
  bowide.setVocabulary(vocabulary);
  
  // Reading in response histograms
  map<string,Mat> classes_training_data; classes_training_data.clear();
  vector<std::string> class_names;
  cout << "Reading reponse histograms from training data" << endl;
  extractTrainingData(train_path,  classes_training_data, vocabulary);
  
  cout << "Training with " << classes_training_data.size() << " classes." <<endl;
  for (map<string,Mat>::iterator it = classes_training_data.begin(); it != classes_training_data.end(); ++it) {
    cout << " class " << (*it).first << " has " << (*it).second.rows << " samples"<<endl;
    class_names.push_back((*it).first);
  }
  
  //DATA LOADED AND READY TO TRAIN ==================================================
  std::cout << "Training Support Vector Machine" << std::endl;
  time_t     now = time(0);
  struct tm  tstruct;
  char       buf[80];
  tstruct = *localtime(&now);
  strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
  stringstream folder_name;
  folder_name << "SVMS_" << buf;
  boost::filesystem::create_directories(folder_name.str());
  for (unsigned int i=0;i<class_names.size();i++) {
    string class_ = class_names[i];
    
    cv::Mat samples(0,(*classes_training_data.begin()).second.cols,(*classes_training_data.begin()).second.type());
    cv::Mat labels(0,1,CV_32FC1);
    
    //copy class samples and label
    cout << "adding " << classes_training_data[class_].rows << " positive" << endl;
    samples.push_back(classes_training_data[class_]);
    Mat class_label = Mat::ones(classes_training_data[class_].rows, 1, CV_32FC1);
    labels.push_back(class_label);

    //copy rest samples and label
    for (map<string,Mat>::iterator it1 = classes_training_data.begin(); it1 != classes_training_data.end(); ++it1) {
      string not_class_ = (*it1).first;
      if(not_class_.compare(class_)==0) continue;
      samples.push_back(classes_training_data[not_class_]);
      class_label = Mat::zeros(classes_training_data[not_class_].rows, 1, CV_32FC1);
      labels.push_back(class_label);
    }

    cout << "Train.." << endl;
    cv::Mat samples_32f; samples.convertTo(samples_32f, CV_32F);
    if(samples.rows == 0) continue; //phantom class?!
    CvSVM classifier;
    classifier.train(samples_32f,labels);
    
    stringstream ss;
    ss << "SVMS_" << to_string(seed) << buf << "/SVM_classifier_";
    ss << buf << "~";
    ss << class_ << ".yml";
    cout << "Saving as: " << ss.str() << endl;
    classifier.save(ss.str().c_str());
  }
}

*/
