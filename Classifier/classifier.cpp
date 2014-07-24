#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/highgui.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/ml/ml.hpp>
#include <boost/filesystem.hpp>
#include <time.h>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <boost/algorithm/string.hpp>
#include "classifier.hpp"
#include "../Segmentor/segmentor.hpp"
#include <boost/lambda/bind.hpp>
#include <memory>
#include <map>

using namespace std;
using namespace cv;

Classifier::Classifier(){}
Classifier::~Classifier(){}

const int py_level = 3;

template <typename T>
//=======================================================================================
std::string to_string(T value){
//=======================================================================================
  std::ostringstream os ;
  os << value ;
  return os.str() ;
}
//=======================================================================================
int Classifier::getSIFT(const char* argv, cv::Mat& _res){
//=======================================================================================
  const cv::Mat input = cv::imread(argv, 0);

    cv::SiftFeatureDetector detector;
    std::vector<cv::KeyPoint> keypoints;
    detector.detect(input, keypoints);

    // Add results to image and save.
    cv::drawKeypoints(input, keypoints, _res);

    return 0;
}
// folderpath = If directory, must point to directory of images, each class seperated into folders
// folderpath = If file, must point to a seed file which can be generated with makeFileList()
//=======================================================================================
int Classifier::getWords(const char * folderpath, int seed, bool verbose = true){
//=======================================================================================
  // Set up components

  cv::initModule_nonfree();
  cv::initModule_features2d();
  
  Ptr<FeatureDetector> detector =  makePtr<PyramidAdaptedFeatureDetector>(FeatureDetector::create("SIFT"),py_level); //detector
  Ptr<DescriptorExtractor > extractor = DescriptorExtractor::create("OpponentSIFT"); // Extractor
  //Ptr<DescriptorMatcher > matcher(new BFMatcher); // Brute Force matcher
  std::vector<cv::KeyPoint> keypoints;
  cv::Mat training_descriptors(0,extractor->descriptorSize(),extractor->descriptorType());
  cv::Mat descriptors;

  // Get file names and containing folder
  if(verbose){
    std::cout << "Opening files and extracting features" << std::endl;
  }
  //==========================================
  // For use with full data set METHOD 1
  //==========================================
  // If directory
  boost::filesystem::path  dir(folderpath);
  if(!boost::filesystem::is_regular_file(dir)){
    if(verbose){
      std::cout << "Using full directory" << std::endl;
    }
    for ( boost::filesystem::recursive_directory_iterator end, dir(folderpath); 
	  dir != end; ++dir ) {
      if(boost::filesystem::is_regular_file(*dir) && boost::filesystem::extension(*dir)== ".tif" ){
	if(verbose){
	  std::cout << "Reading: " << dir->path().string() << std::endl;
	}
	Mat img = imread(dir->path().string());
	if(verbose){ 
	  std::cout << "Detecting" << std::endl;
	}
	detector->detect(img, keypoints);
	if(verbose){
	  std::cout << "Extracting" << std::endl;
	}
	extractor->compute(img,keypoints,descriptors);
	training_descriptors.push_back(descriptors);
      }
    }
  }
  
  //==========================================
  // For use with seed file METHOD 2
  //==========================================
  // Else seed file
  else{
    if(verbose){
      std::cout << "Using Seed file" << std::endl;
    }
    std::vector<std::vector<string> > seed_data;
    load2Dvector(seed_data, folderpath);
    for(unsigned int i = 0; i < seed_data.size(); i++){
      if(seed_data[i][2] == "train"){
	if(verbose){
	  std::cout << "Reading: " << seed_data[i][0] << std::endl;
	}
	Mat img = imread(seed_data[i][0]);
	if(verbose){
	  std::cout << "Detecting" << std::endl;
	}
	detector->detect(img, keypoints);
	if(verbose){
	  std::cout << "Extracting" << std::endl;
	}
	extractor->compute(img,keypoints,descriptors);
	training_descriptors.push_back(descriptors);
      }
    }
  }

  if(verbose){
    std::cout << "Total descriptors: " << training_descriptors.rows << std::endl
	      << "Saving Descriptors" << std::endl;
  }
  // Save descriptors
  std::string save_des = "Descriptors/Descriptors_";
  time_t     now = time(0);
  struct tm  tstruct;
  char       buf[80];
  tstruct = *localtime(&now);
  strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
  save_des += buf;
  save_des += ".yml";
  // Currently not saving descriptors as file is a bit large
  //std::cout << "Saving descriptors as: " << save_des << std::endl;
  //FileStorage fs1(save_des, FileStorage::WRITE);
  //fs1 << "training_descriptors" << training_descriptors;
  //fs1.release();

  // Find vocabulary
  cv::BOWKMeansTrainer bowtrainer(1000); //num clusters
  bowtrainer.add(training_descriptors);
  std::cout << "clustering features" << std::endl;
  cv::Mat vocabulary = bowtrainer.cluster();
  
  // Save vocabulary
  std::cout << "Saving vocabulary" << std::endl;
  std::string save_voc = "Vocabularies/Vocabulary_";
  save_voc += to_string(seed);
  save_voc += "_";
  save_voc += buf;
  save_voc += ".yml";
  std::cout << "Saving vocabulary as: " << save_voc << std::endl;
  FileStorage fs2(save_voc, FileStorage::WRITE);
  fs2 << "vocabulary" << vocabulary;
  fs2.release();
  return 0;
}
//=======================================================================================
int Classifier::makeFileList(const char * folderpath, int seed){
//=======================================================================================
  std::vector<std::string> folderlist;
  std::vector<std::vector<string> > image_split;
  int count, folder_count= -1;
  // Set between 1 - 5 for each test case, can be used to replicate
  // the random test sets
  srand (seed);

  // Store a list of all folders
  for ( boost::filesystem::recursive_directory_iterator end, dir(folderpath); 
	dir != end; ++dir ) {
    if(!boost::filesystem::is_regular_file(*dir)){
      folderlist.push_back(dir->path().string());
    }
  }
  
  // Choose which image in each folder will be our test case
  int test_count[folderlist.size()];
  
  for(unsigned int i = 0; i < folderlist.size(); i++){
    count = 0;
    for( boost::filesystem::recursive_directory_iterator end, dir(folderlist[i]); dir!= end; dir++){
      count++;
    }
    //std::cout << folderlist[i] << ": " << count << std::endl;
    test_count[i] = rand() % count;
    //std::cout << test_count[i] << std::endl;
  }
  
  // Generate a full list of filepath / classification / test or train
  for ( boost::filesystem::recursive_directory_iterator end, dir(folderpath); 
	dir != end; ++dir) {
    if(!boost::filesystem::is_regular_file(*dir)){
      folder_count++;
      count = 0;
    }
    if(boost::filesystem::is_regular_file(*dir)){
      std::vector<string> tmp;
      tmp.push_back(dir->path().string());
      tmp.push_back(dir->path().parent_path().filename().string());
      if(test_count[folder_count] == count){
	tmp.push_back("test");
      }
      else{
	tmp.push_back("train");
      }
      image_split.push_back(tmp);
      count++;
      //std::cout <<test_count[folder_count] << ":"<< count << std::endl;
    }
  }

  // print2Dvector(image_split);
  
  // Save to csv
  save2Dvector(image_split,seed);

  // load2Dvector() TEST
  /*
  vector<vector<std::string> > test;
  load2Dvector(test, "test_seed_1");
  print2Dvector(test);
  */
  return 0;
  
}
//=======================================================================================
void Classifier::checkFolders(const char * folderpath){
//=======================================================================================
  bool need_checking = false;
  for ( boost::filesystem::recursive_directory_iterator end, dir(folderpath); 
       dir != end; ++dir ) {
    if(boost::filesystem::is_regular_file(*dir) &&!(boost::filesystem::extension(*dir)== ".tif") ){
      std::cout << "Please check file: " << *dir << std::endl;
      need_checking = true;
    }
  }
  if(!need_checking){
    std::cout << "All files are as expected" << std::endl;
  }
}
//=======================================================================================
void Classifier::print2Dvector(std::vector<std::vector<string> > print){
//=======================================================================================
  for(unsigned int i = 0; i < print.size();i++){
    for(unsigned int j = 0; j < print[i].size(); j++){
      std::cout << std::setw(10) << print[i][j];
    }
    std::cout << std::endl;
  }
}
//=======================================================================================
void Classifier::save2Dvector(std::vector<std::vector<string> > print, int seed){
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
void Classifier::load2Dvector(std::vector<std::vector<string> > &print,std::string file_path){
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
  
  Ptr<FeatureDetector> detector =  makePtr<PyramidAdaptedFeatureDetector>(FeatureDetector::create("SIFT"),py_level); //detector
  Ptr<DescriptorExtractor > extractor = DescriptorExtractor::create("OpponentSIFT"); // Extractor
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
  folder_name << "SVMS_" << to_string(seed) << buf;
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
    ss << buf << "+";
    ss << class_ << ".yml";
    cout << "Saving as: " << ss.str() << endl;
    classifier.save(ss.str().c_str());
  }
}

//=======================================================================================
void Classifier::extractTrainingData(std::string filepath, std::map<string,Mat>& classes_training_data, cv::Mat vocabulary){
//=======================================================================================
  cv::initModule_nonfree();
  Segmentor * myseg = new Segmentor;
  cout << "Extracting Training Data" << endl;
  cout << "Reading seed information from file: "<< filepath <<endl;
  vector<KeyPoint> keypoints;
  cv::Mat response_hist, colour_hist,full_hist;
  cv::Mat img;
  std::vector<std::vector<string> > seed;
  load2Dvector(seed,filepath);
  Ptr<FeatureDetector> detector =  makePtr<PyramidAdaptedFeatureDetector>(FeatureDetector::create("SIFT"),py_level); //detector
  Ptr<DescriptorExtractor > extractor = DescriptorExtractor::create("OpponentSIFT"); // Extractor
  Ptr<DescriptorMatcher > matcher(new BFMatcher);
  BOWImgDescriptorExtractor bowide(extractor,matcher);
  bowide.setVocabulary(vocabulary);
  std::cout << "Descriptor size: " << bowide.descriptorSize() << std::endl; 

  for(unsigned int i = 0; i < seed.size(); i++){
    if(seed[i][2] == "train"){
      img = imread(seed[i][0]);
      detector->detect(img,keypoints);
      bowide.compute(img, keypoints, response_hist);
      getHist(img,colour_hist,myseg);
      colour_hist.convertTo(colour_hist,response_hist.type());
      if(classes_training_data.count(seed[i][1]) == 0) { //not yet created...
	classes_training_data[seed[i][1]].create(0,(response_hist.cols + colour_hist.cols),response_hist.type());
      }
      hconcat(response_hist,colour_hist,full_hist);
      classes_training_data[seed[i][1]].push_back(full_hist);
      
    }
  }
  delete myseg;
}
// seed_path  = Path to seed file
// vocav_path = Path to vocabulary file
// svm_path   = Path to SVM files
//=======================================================================================
void Classifier::testSVM(std::string seed_path, std::string vocab_path, std::string svm_path, int seed){
//=======================================================================================
  Segmentor * myseg = new Segmentor;
  cv::initModule_nonfree();
  float count = 0, correct = 0;
  map<string,map<string,int> > confusion_matrix; // confusionMatrix[classA][classB] = number_of_times_A_voted_for_B;
  map<string,unique_ptr<CvSVM>> classes_classifiers;//CvSVM tmp_SVM;
  vector<string> files; //load up with images
  vector<string> classes; //load up with the respective classes
  std::vector<std::vector<string> > test_images;
  load2Dvector(test_images, seed_path);
  Ptr<FeatureDetector> detector =  makePtr<PyramidAdaptedFeatureDetector>(FeatureDetector::create("SIFT"),py_level); //detector
  Ptr<DescriptorExtractor > extractor = DescriptorExtractor::create("OpponentSIFT"); // Extractor
  Ptr<DescriptorMatcher > matcher(new BFMatcher);
  BOWImgDescriptorExtractor bowide(extractor,matcher);

  cout << "Reading vocabulary from file: "<< vocab_path <<endl;
  Mat vocabulary;
  FileStorage fs(vocab_path, FileStorage::READ);
  fs["vocabulary"] >> vocabulary;
  fs.release();	
 
  bowide.setVocabulary(vocabulary);
  
  // Populate SVMs
  for ( boost::filesystem::recursive_directory_iterator end, dir(svm_path); 
	dir != end; ++dir ) {
    if(boost::filesystem::is_regular_file(*dir)){
      std::cout << "Reading: " << dir->path().string() << std::endl;
      vector<std::string> tmp_line;
      boost::split(tmp_line,dir->path().string(), boost::is_any_of("+"));
      
      std::string class_name = tmp_line[1].substr(0, tmp_line[1].size()-4);

      classes_classifiers.insert(make_pair(class_name, unique_ptr<CvSVM>(new CvSVM())));
      classes_classifiers[class_name]->load((dir->path().string()).c_str());
      
    }
  }

  for(unsigned int i = 0; i < test_images.size(); i++) {
    if(test_images[i][2] == "test"){
      Mat img = imread(test_images[i][0]), response_hist, colour_hist,full_hist;      
      vector<KeyPoint> keypoints;
      detector->detect(img,keypoints);
      bowide.compute(img, keypoints, response_hist);
      getHist(img,colour_hist,myseg);
      colour_hist.convertTo(colour_hist,response_hist.type());
      hconcat(response_hist,colour_hist,full_hist);

      float minf = FLT_MAX; string minclass;
      for (map<string,unique_ptr<CvSVM>>::iterator it = classes_classifiers.begin(); it != classes_classifiers.end(); ++it) {
	float res = (*it).second->predict(full_hist,true);
	if (res < minf) {
	  minf = res;
	  minclass = (*it).first;
	}
      }
      std::cout << "Case: " << test_images[i][1] << " MinClass prediction: " << minclass << std::endl;
      if(test_images[i][1] == minclass){
	correct++;
      }
      count++;
      confusion_matrix[minclass][test_images[i][1]]++; 
    }
  }
  std::cout << "Total Accuracy: " << (correct/count)*100 << "%" << std::endl;
  
  ofstream myfile;

  std::string save_name = "Confusions/cm_";
  save_name += to_string(seed);
  myfile.open(save_name.c_str());

  std::cout << "------------------Confusion Matix------------------" << std::endl;
  for(map<string,map<string,int> >::iterator it = confusion_matrix.begin(); it != confusion_matrix.end(); ++it) {
    myfile << (*it).first << " -> ";
    cout << (*it).first << " -> ";
    for(map<string,int>::iterator it1 = (*it).second.begin(); it1 != (*it).second.end(); ++it1) {
      myfile << (*it1).first << ":" << (*it1).second << endl;
      cout << (*it1).first << ":" << (*it1).second << endl;
    }
  }
  myfile.close();

  delete myseg;
}

//=======================================================================================
void Classifier::getHist(cv::Mat src, cv::Mat &res, Segmentor* myseg, bool verbose){
//=======================================================================================
  Mat dst, tmp, mask;

  // Get mask from segmentor

  myseg->getMask(src, mask);

  /// Separate the image in 3 places ( B, G and R )
  vector<Mat> bgr_planes;
  split( src, bgr_planes );
  
  /// Establish the number of bins
  int histSize = 30;

  /// Set the ranges ( for B,G,R) )
  float range[] = { 0, 256 } ;
  const float* histRange = { range };

  bool uniform = true; bool accumulate = false;

  Mat b_hist, g_hist, r_hist;

  /// Compute the histograms:
  calcHist( &bgr_planes[0], 1, 0, mask, b_hist, 1, &histSize, &histRange, uniform, accumulate );
  calcHist( &bgr_planes[1], 1, 0, mask, g_hist, 1, &histSize, &histRange, uniform, accumulate );
  calcHist( &bgr_planes[2], 1, 0, mask, r_hist, 1, &histSize, &histRange, uniform, accumulate );

  // Concatenate the histograms
  vconcat(b_hist,g_hist,tmp);
  vconcat(tmp,r_hist,tmp);
  transpose(tmp,res);

  // Verbose used for testing purposes
  if(verbose == true){
    // Draw the histograms for B, G and R
    int hist_w = 512; int hist_h = 400;
    int bin_w = cvRound( (double) hist_w/histSize );
    
    Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );
    
    /// Normalize the result to [ 0, histImage.rows ]
    normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    
    
  
    /// Draw for each channel
    
    for( int i = 1; i < histSize; i++ )
      {
	line( histImage, Point( bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1)) ) ,
	      Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
	      Scalar( 255, 0, 0), 2, 8, 0  );
	line( histImage, Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1)) ) ,
	      Point( bin_w*(i), hist_h - cvRound(g_hist.at<float>(i)) ),
	    Scalar( 0, 255, 0), 2, 8, 0  );
	line( histImage, Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1)) ) ,
	      Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),
	      Scalar( 0, 0, 255), 2, 8, 0  );
      }
    
    /// Display
    namedWindow("calcHist Demo", CV_WINDOW_AUTOSIZE );
    imshow("calcHist Demo", histImage );
    
    waitKey(0);
  }


}
