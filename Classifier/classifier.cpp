#include <boost/python.hpp>
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
#include <math.h>
#include <map>

using namespace std;
using namespace cv;


const int histSize = 30;
const int h_bins = 25;
const int s_bins = 30;
const int hist_level = 2;
const int py_level = 4;
const bool use_hist_py = false;
const bool use_hist_pyramid = true;
const bool use_hist = true;
const bool use_hsv = true;
const bool use_flips = true;
const bool use_pca = true;
const int img_total = 571; // estimate of number of images
//const float hist_factor = 0.2;

Classifier::Classifier(){
  cv::initModule_nonfree();
  cv::initModule_features2d();
  
  detector =  makePtr<PyramidAdaptedFeatureDetector>(FeatureDetector::create("SIFT"),py_level); //det]ector
  extractor = DescriptorExtractor::create("OpponentSIFT"); // Extractor  
  matcher = DescriptorMatcher::create("BruteForce");
  myseg = new Segmentor;
}

Classifier::~Classifier(){
  //delete detector;
  //delete extractor;
  //delete matcher;
  delete myseg;
}
template <typename T>
//=======================================================================================
std::string to_string(T value){
//=======================================================================================
  std::ostringstream os ;
  os << value ;
  return os.str() ;
}
//=======================================================================================
void Classifier::checkFolders(std::string folderpath){
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
// folderpath = If directory, must point to directory of images, each class seperated into folders
// folderpath = If file, must point to a seed file which can be generated with makeFileList()
//=======================================================================================
int Classifier::getWords(std::string folderpath, int seed, bool verbose){
//=======================================================================================
  // Set up components

  cv::initModule_nonfree();
  cv::initModule_features2d();
  
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
	Mat img = imread(seed_data[i][0]), mask;
	if(verbose){
	  std::cout << "Detecting" << std::endl;
	}
	myseg->getMask(img, mask);
	detector->detect(img, keypoints,mask);
	if(verbose){
	  std::cout << "Extracting" << std::endl;
	}
	extractor->compute(img,keypoints,descriptors);
	training_descriptors.push_back(descriptors);
      }
    }
  }
  
  time_t     now = time(0);
  struct tm  tstruct;
  char       buf[80];
  tstruct = *localtime(&now);
  strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
  
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
int Classifier::makeFileList(std::string folderpath, int seed){
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

  // Save to csv
  save2Dvector(image_split,seed);

  return 0;
  
}
int Classifier::kFoldFileList(std::string folderpath, int seed){

  std::vector<std::string> filelist;

  int count = 10;
  // the random test sets
  srand (seed);

  // Store a list of all files
  for ( boost::filesystem::recursive_directory_iterator end, dir(folderpath); 
	dir != end; ++dir ) {
    if(boost::filesystem::is_regular_file(*dir)){
      filelist.push_back(dir->path().string());
    }
  }
  
  // Choose which test case each image will belong to
  int test_count[filelist.size()];
  
  for(unsigned int i = 0; i < filelist.size(); i++){
    //std::cout << folderlist[i] << ": " << count << std::endl;
    test_count[i] = rand() % count;
    //std::cout << test_count[i] << std::endl;
  }
  for(int test = 0; test < 10; test++){
    cout << test << endl;
    std::vector<std::vector<string> > image_split;
    for(unsigned int i = 0; i < filelist.size(); i++){
      std::vector<string> tmp;
      boost::filesystem::path dir(filelist[i]);
      tmp.push_back(dir.string());
      tmp.push_back(dir.parent_path().filename().string());
      if(test_count[i] == test){
	tmp.push_back("test");
      }
      else{
	tmp.push_back("train");
      }
      image_split.push_back(tmp);
    }
    save2Dvector(image_split,test);
    cout << "saving seed " << test << endl;
  }

  return 0;

}
// vocab_path = Path of vocabulary file
// train_path = Path of seed file
//=======================================================================================
std::string Classifier::trainSVM(std::string vocab_path, std::string train_path, int seed, bool verbose){
//=======================================================================================
  cv::initModule_nonfree();

  if(verbose){
    cout << "Training SVM" << endl;
    cout << "reading vocabulary from file: "<< vocab_path <<endl;
  }

  Mat vocabulary;
  FileStorage fs(vocab_path, FileStorage::READ);
  fs["vocabulary"] >> vocabulary;
  fs.release();	
  
  BOWImgDescriptorExtractor bowide(extractor,matcher);
  bowide.setVocabulary(vocabulary);
  
  // Reading in response histograms
  map<string,Mat> classes_training_data; classes_training_data.clear();
  vector<std::string> class_names;
  if(verbose){
    cout << "Reading reponse histograms from training data" << endl;
  }
  extractTrainingData(train_path,  classes_training_data, vocabulary, seed);
  
  if(verbose){  
    cout << "Training with " << classes_training_data.size() << " classes." <<endl;
  }
  for (map<string,Mat>::iterator it = classes_training_data.begin(); it != classes_training_data.end(); ++it) {
    if(verbose){
      cout << " class " << (*it).first << " has " << (*it).second.rows << " samples"<<endl;
    }
    class_names.push_back((*it).first);
  }
  
  //DATA LOADED AND READY TO TRAIN ==================================================
  if(verbose){
    std::cout << "Training Support Vector Machine" << std::endl;
  }
  time_t     now = time(0);
  struct tm  tstruct;
  char       buf[80];
  tstruct = *localtime(&now);
  strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
  stringstream folder_name;
  folder_name << "SVMS_" << to_string(seed) << "_" << buf;
  boost::filesystem::create_directories(folder_name.str());
  for (unsigned int i=0;i<class_names.size();i++) {
    string class_ = class_names[i];
    
    cv::Mat samples(0,(*classes_training_data.begin()).second.cols,(*classes_training_data.begin()).second.type());
    cv::Mat labels(0,1,CV_32FC1);
    
    //copy class samples and label
    if(verbose){
      cout << "adding " << classes_training_data[class_].rows << " positive" << endl;
    }
    samples.push_back(classes_training_data[class_]);
    Mat class_label = Mat::ones(classes_training_data[class_].rows, 1, CV_32FC1);
    labels.push_back(class_label);

    //copy rest of samples and label
    for (map<string,Mat>::iterator it1 = classes_training_data.begin(); it1 != classes_training_data.end(); ++it1) {
      string not_class_ = (*it1).first;
      if(not_class_.compare(class_)==0) continue;
      samples.push_back(classes_training_data[not_class_]);
      class_label = Mat::zeros(classes_training_data[not_class_].rows, 1, CV_32FC1);
      labels.push_back(class_label);
    }
    if(verbose){
      cout << "Train.." << endl;
    }
    cv::Mat samples_32f; samples.convertTo(samples_32f, CV_32F);
    if(samples.rows == 0) continue; //phantom class?!
    CvSVM classifier;
    classifier.train(samples_32f,labels);
    
    stringstream ss;
    ss << "SVMS_" << to_string(seed) << "_" << buf << "/SVM_classifier_";
    ss << buf << "+";
    ss << class_ << ".yml";
    if(verbose){
      cout << "Saving as: " << ss.str() << endl;
    }
    classifier.save(ss.str().c_str());
  }
  return folder_name.str();
}

//=======================================================================================
void Classifier::extractTrainingData(std::string filepath, std::map<string,Mat>& classes_training_data, cv::Mat vocabulary, int seed, bool verbose ){
//=======================================================================================
  cv::initModule_nonfree();

  Mat eigenvalues, eigenvectors, res;
  
  std::string pca_file = "PCA/pca_" + to_string(seed) + ".yml";
  FileStorage fs(pca_file, FileStorage::READ);
  fs["Eigenvalues"] >> eigenvalues;
  fs["Eigenvector"] >> eigenvectors;
  
  if(verbose){
    cout << "Extracting Training Data" << endl;
    cout << "Reading seed information from file: "<< filepath <<endl;
  }
  vector<KeyPoint> keypoints;
  cv::Mat full_hist;
  cv::Mat img, mask,img_flip,mask_flip,img_pca,img_flip_pca;
  std::vector<std::vector<string> > img_list;
  load2Dvector(img_list,filepath);
  
  BOWImgDescriptorExtractor bowide(extractor,matcher);
  bowide.setVocabulary(vocabulary);
  std::cout << "Descriptor size: " << bowide.descriptorSize() << std::endl; 
  
  //Perform on the fly data augmentation
  for(unsigned int i = 0; i < img_list.size(); i++){
    if(img_list[i][2] == "train"){
      img = imread(img_list[i][0]);
      myseg->getMask(img, mask);
      getFeatures(img,mask,full_hist,&bowide);
      if(classes_training_data.count(img_list[i][1]) == 0) { //not yet created...
	classes_training_data[img_list[i][1]].create(0,(full_hist.cols),full_hist.type());
      }
      classes_training_data[img_list[i][1]].push_back(full_hist);
      full_hist.release();
      if(use_flips && use_pca){
	flip(img,img_flip,1);
	flip(mask,mask_flip,1);
	getFeatures(img_flip,mask_flip,full_hist,&bowide);
	classes_training_data[img_list[i][1]].push_back(full_hist);
	full_hist.release();
	pcaImage(img, eigenvalues, eigenvectors, img_pca);
	getFeatures(img_pca,mask,full_hist,&bowide);
	classes_training_data[img_list[i][1]].push_back(full_hist);
	full_hist.release();
      }
      else if(use_pca){
	pcaImage(img, eigenvalues, eigenvectors, img_pca);
	getFeatures(img_pca,mask,full_hist,&bowide);
	classes_training_data[img_list[i][1]].push_back(full_hist);
	full_hist.release();
      }
      else if(use_flips){
	flip(img,img_flip,1);
	flip(mask,mask_flip,1);
	getFeatures(img_flip,mask_flip,full_hist,&bowide);
	classes_training_data[img_list[i][1]].push_back(full_hist);
	full_hist.release();
      }
    }
  }

}

// vocab_path = Path of vocabulary file
// train_path = Path of seed file
//=======================================================================================
std::string Classifier::trainSVMParams(std::string vocab_path, std::string train_path, int seed, bool verbose){
//=======================================================================================
  cv::initModule_nonfree();
  if(verbose){
    cout << "Training SVM Parameters" << endl;
    cout << "reading vocabulary from file: "<< vocab_path <<endl;
  }
  Mat vocabulary;
  FileStorage fs(vocab_path, FileStorage::READ);
  fs["vocabulary"] >> vocabulary;
  fs.release();	
  
  BOWImgDescriptorExtractor bowide(extractor,matcher);
  bowide.setVocabulary(vocabulary);
  cout << "working" << endl;
  // Reading in response histograms
  map<string,Mat> classes_training_data; classes_training_data.clear();
  vector<std::string> class_names;
  if(verbose){
    cout << "Reading reponse histograms from training data" << endl;
  }
  extractTrainingData(train_path,  classes_training_data, vocabulary, seed);
  
  if(verbose){  
    cout << "Training with " << classes_training_data.size() << " classes." <<endl;
  }
  for (map<string,Mat>::iterator it = classes_training_data.begin(); it != classes_training_data.end(); ++it) {
    if(verbose){
      cout << " class " << (*it).first << " has " << (*it).second.rows << " samples"<<endl;
    }
    class_names.push_back((*it).first);
  }
  
  //DATA LOADED AND READY TO TRAIN ==================================================
  if(verbose){
    std::cout << "Training Support Vector Machine" << std::endl;
  }
  time_t     now = time(0);
  struct tm  tstruct;
  char       buf[80];
  tstruct = *localtime(&now);
  strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
  stringstream folder_name;
  folder_name << "SVMS_" << to_string(seed) << "_" << buf;

  boost::filesystem::create_directories(folder_name.str());

  for (unsigned int i=0;i<class_names.size();i++) {
    string class_ = class_names[i];
    
    cv::Mat samples(0,(*classes_training_data.begin()).second.cols,(*classes_training_data.begin()).second.type());
    cv::Mat labels(0,1,CV_32FC1);
    CvSVMParams parameters;
    CvParamGrid CvParamGrid_C(pow(2.0,-5), pow(2.0,15), pow(2.0,1));
    CvParamGrid CvParamGrid_gamma(pow(2.0,-15), pow(2.0,3), pow(2.0,1));


    //copy class samples and label
    if(verbose){
      cout << "adding " << classes_training_data[class_].rows << " positive: " << (i+1) << "out of " << class_names.size() << endl;
    }
    samples.push_back(classes_training_data[class_]);
    Mat class_label = Mat::ones(classes_training_data[class_].rows, 1, CV_32FC1);
    labels.push_back(class_label);

    //copy rest of samples and label
    for (map<string,Mat>::iterator it1 = classes_training_data.begin(); it1 != classes_training_data.end(); ++it1) {
      string not_class_ = (*it1).first;
      if(not_class_.compare(class_)==0) continue;
      samples.push_back(classes_training_data[not_class_]);
      class_label = Mat::zeros(classes_training_data[not_class_].rows, 1, CV_32FC1);
      labels.push_back(class_label);
    }
    if(verbose){
      cout << "Train.." << endl;
    }
    cv::Mat samples_32f; samples.convertTo(samples_32f, CV_32F);
    if(samples.rows == 0) continue; //phantom class?!
    CvSVM classifier;
    if(classes_training_data[class_].rows > 1){
      classifier.train_auto(samples_32f,labels, cv::Mat(), cv::Mat(), parameters, 10,CvParamGrid_C, CvParamGrid_gamma, CvSVM::get_default_grid(CvSVM::P), CvSVM::get_default_grid(CvSVM::NU), CvSVM::get_default_grid(CvSVM::COEF), CvSVM::get_default_grid(CvSVM::DEGREE), true);
    }
    else{
      classifier.train(samples_32f,labels);
    }
    stringstream ss;
    ss << "SVMS_" << to_string(seed) << "_" << buf << "/SVM_classifier_";
    ss << buf << "+";
    ss << class_ << ".yml";
    if(verbose){
      cout << "Saving as: " << ss.str() << endl;
    }
    classifier.save(ss.str().c_str());
  }
  return folder_name.str();
}
// seed_path  = Path to seed file
// vocav_path = Path to vocabulary file
// svm_path   = Path to SVM files
//=======================================================================================
void Classifier::testSVM(std::string seed_path, std::string vocab_path, std::string svm_path, int seed, bool verbose){
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
      if(verbose){
	std::cout << "Reading: " << dir->path().string() << std::endl;
      }
      vector<std::string> tmp_line;
      boost::split(tmp_line,dir->path().string(), boost::is_any_of("+"));
      
      std::string class_name = tmp_line[1].substr(0, tmp_line[1].size()-4);

      classes_classifiers.insert(make_pair(class_name, unique_ptr<CvSVM>(new CvSVM())));
      classes_classifiers[class_name]->load((dir->path().string()).c_str());
      
    }
  }

  for(unsigned int i = 0; i < test_images.size(); i++) {
    if(test_images[i][2] == "test"){
      Mat img = imread(test_images[i][0]), mask,response_hist, colour_hist,full_hist, tmp;
      myseg->getMask(img, mask);
      vector<KeyPoint> keypoints;
      double hu[7];
      getmoments(mask,hu);
      cv::Mat mom = (Mat_<double>(1,7) << hu[0], hu[1], hu[2], hu[3], hu[4], hu[5], hu[6]); 
      detector->detect(img,keypoints,mask);
      if(use_hist){
	bowide.compute(img, keypoints, response_hist);
	if(use_hist_pyramid){
          getHistPyramid(img,colour_hist,mask);
        }
        else{
          getHist(img,colour_hist,mask);
        }
	colour_hist.convertTo(colour_hist,response_hist.type());
	mom.convertTo(mom,response_hist.type());
	hconcat(response_hist,colour_hist,tmp);
	hconcat(tmp,mom, full_hist);
      }
      else{
	bowide.compute(img, keypoints, response_hist);
	mom.convertTo(mom,response_hist.type());
	hconcat(response_hist,mom,full_hist);
      }

      float minf = FLT_MAX; string minclass;
      for (map<string,unique_ptr<CvSVM>>::iterator it = classes_classifiers.begin(); it != classes_classifiers.end(); ++it) {
	float res = (*it).second->predict(full_hist,true);
	//cout << "res: " << res << endl;
	if (res < minf) {
	  minf = res;
	  minclass = (*it).first;
	}
      }
      if(verbose){
	std::cout << "Case: " << test_images[i][1] << " MinClass prediction: " << minclass << std::endl;
      }
      if(test_images[i][1] == minclass){
	correct++;
      }
      count++;
      confusion_matrix[minclass][test_images[i][1]]++; 
    }
  }

  float accuracy = (correct/count)*100;

  ofstream myfile;
  std::string save_name = "Confusions/cm_";
  save_name += to_string(seed);
  save_name + ".txt";
  myfile.open(save_name.c_str());
  
  if(verbose){
    std::cout << "Total Accuracy: " << accuracy << "%" << std::endl;
    std::cout << "------------------Confusion Matix------------------" << std::endl;
  }
  myfile << "Total Accuracy: " << accuracy << "%" << std::endl;
  myfile << "------------------Confusion Matix------------------" << std::endl;
 
  for(map<string,map<string,int> >::iterator it = confusion_matrix.begin(); it != confusion_matrix.end(); ++it) {
    myfile << (*it).first << " -> ";
    if(verbose){
      cout << (*it).first << " -> ";
    }
    for(map<string,int>::iterator it1 = (*it).second.begin(); it1 != (*it).second.end(); ++it1) {
      myfile << (*it1).first << ":" << (*it1).second << endl;
      if(verbose){
	cout << (*it1).first << ":" << (*it1).second << endl;
      }
    }
  }
  myfile.close();

  delete myseg;
}

//=======================================================================================
void Classifier::getHist(cv::Mat src, cv::Mat &res, cv::Mat mask, bool verbose){
//=======================================================================================
  Mat dst, tmp;
  res.release();
  // Get mask from segmentor

  

  /// Separate the image in 3 places ( B, G and R )
  vector<Mat> bgr_planes, hsv_planes;
  split( src, bgr_planes );
  
  /// Set the ranges ( for B,G,R) )
  float range[] = { 0, 256 } ;
  const float* histRange = { range };

  bool uniform = true; bool accumulate = false;

  Mat b_hist, g_hist, r_hist;
  Mat hsv_image, h_hist, s_hist;

  if(use_hsv){
  
    cvtColor( src, hsv_image, COLOR_BGR2HSV );
    split( hsv_image, hsv_planes );
   
    // hue varies from 0 to 179, saturation from 0 to 255
    float h_ranges[] = { 0, 180 };
    float s_ranges[] = { 0, 256 };
    
    const float* hRange = { h_ranges};
    const float* sRange = { s_ranges};

    // Use the o-th and 1-st channels
    calcHist( &hsv_planes[0], 1, 0, mask, h_hist, 1, &h_bins, &hRange, uniform, accumulate );
    calcHist( &hsv_planes[1], 1, 0, mask, s_hist, 1, &s_bins, &sRange, uniform, accumulate );

    vconcat(h_hist,s_hist,tmp);
    
    transpose(tmp,tmp);
    CvScalar total = sum(tmp);
    tmp /= total.val[0];
    res = tmp;
  }
  else{
  
    /// Compute the histograms:
    calcHist( &bgr_planes[0], 1, 0, mask, b_hist, 1, &histSize, &histRange, uniform, accumulate );
    calcHist( &bgr_planes[1], 1, 0, mask, g_hist, 1, &histSize, &histRange, uniform, accumulate );
    calcHist( &bgr_planes[2], 1, 0, mask, r_hist, 1, &histSize, &histRange, uniform, accumulate );
  
    // Concatenate the histograms
    vconcat(b_hist,g_hist,tmp);
    vconcat(tmp,r_hist,tmp);

    transpose(tmp,tmp);
    CvScalar total = sum(tmp);
    tmp /= total.val[0];
    res = tmp;
    //normalize(tmp,res,0,hist_factor,NORM_MINMAX,-1,Mat());
  }
  // Verbose used for testing purposes
  if(verbose == true){
    // Draw the histograms for B, G and R
    int hist_w = 512; int hist_h = 400;
    int s_bin_w = cvRound( (double) hist_w/s_bins );
    int h_bin_w = cvRound( (double) hist_w/h_bins );
    
    Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );
    
    /// Normalize the result to [ 0, histImage.rows ]
    
    normalize(h_hist, h_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    normalize(s_hist, s_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    
  
    /// Draw for each channel
    
    for( int i = 1; i < h_bins; i++ )
      {
	line( histImage, Point( h_bin_w*(i-1), hist_h - cvRound(h_hist.at<float>(i-1)) ) ,
	      Point( h_bin_w*(i), hist_h - cvRound(h_hist.at<float>(i)) ),
	      Scalar( 255, 0, 0), 2, 8, 0  );
      }
    
    for( int i = 1; i < s_bins; i++ )
      {
	line( histImage, Point( s_bin_w*(i-1), hist_h - cvRound(s_hist.at<float>(i-1)) ) ,
	      Point( s_bin_w*(i), hist_h - cvRound(s_hist.at<float>(i)) ),
	      Scalar( 0, 255, 0), 2, 8, 0  );
      }
    
    /// Display
    namedWindow("calcHist Demo", CV_WINDOW_AUTOSIZE );
    imshow("calcHist Demo", histImage );
    //imwrite("Euphydryas-aurinia-hist.jpeg",histImage);
    waitKey(0);
  }


}
//=======================================================================================
void Classifier::getHistPyramid(cv::Mat src, cv::Mat &res, cv::Mat mask, bool verbose){
//=======================================================================================
  cv::Mat full;
  int step_y = src.rows/hist_level, step_x = src.cols/hist_level;

  for(int i = 0; i < hist_level; i++){
    for(int j = 0; j < hist_level; j++){
      cv::Mat tmp, rect_mask;
      rect_mask = cv::Mat::zeros(src.size(),CV_8UC1);
      rect_mask(Rect(i*step_x,j*step_y,step_x, step_y)) = 1;
      bitwise_and(rect_mask,mask,rect_mask);
      
      if(i == 0 && j == 0){   
	getHist(src,full,rect_mask,verbose);
      }
      else{
	getHist(src,tmp,rect_mask,verbose);
	hconcat(full,tmp,full);
      }
    }
  }
  CvScalar total = sum(full);
  full /= total.val[0];
  res = full;
}
//=======================================================================================
void Classifier::getFeatures(cv::Mat img,cv::Mat mask, cv::Mat &res, BOWImgDescriptorExtractor * bowide){
//=======================================================================================
  double hu[7];
  getmoments(mask,hu);
  vector<KeyPoint> keypoints;
  cv::Mat mom = (Mat_<double>(1,7) << hu[0], hu[1], hu[2], hu[3], hu[4], hu[5], hu[6]);
  cv::Mat response_hist,colour_hist,tmp,full_hist;
  detector->detect(img,keypoints,mask);
  if(use_hist){
    bowide->compute(img, keypoints, response_hist);
    if(use_hist_pyramid){
      getHistPyramid(img,colour_hist,mask);
    }
    else{
      getHist(img,colour_hist,mask);
    }
    colour_hist.convertTo(colour_hist,response_hist.type());	
    mom.convertTo(mom,response_hist.type());
    hconcat(response_hist,colour_hist,tmp);
    hconcat(tmp,mom, full_hist);
  }
  else{
    bowide->compute(img, keypoints, response_hist);
    mom.convertTo(mom,response_hist.type());
    hconcat(response_hist,mom,full_hist);
  }
  res=full_hist;
}
//=======================================================================================
void Classifier::getmoments(cv::Mat binmask, double hu[7]){
//=======================================================================================
  vector<float> res;
  cv::Moments mom = cv::moments(binmask,true);
  cv::HuMoments(mom, hu);
}
//=======================================================================================
void Classifier::pcaImage(cv::Mat img, cv::Mat eigenvalues, cv::Mat eigenvectors, cv::Mat &res){
//=======================================================================================
  //Alter an image using PCA eigenvalues and vectors
  transpose(eigenvalues,eigenvalues);
  
  Mat alpha, rgb, tmp;
  
  Mat new_image = Mat::zeros( img.size(), img.type() );

  std::default_random_engine de(time(0));
  std::normal_distribution<double> distribution(0.0,0.1);
  
  alpha = (Mat_<float>(1,3) << distribution(de)/img_total, distribution(de)/img_total, distribution(de)/img_total);
  
  transpose(alpha,alpha);
  tmp = alpha*eigenvalues;
  transpose(eigenvectors,eigenvectors);
  transpose(tmp,tmp);
  rgb = eigenvectors*tmp;
  
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
  res = new_image;
}
//Function which is visible to the python wrapper 
std::string classify(std::string svm_path, std::string vocab_path, std::string img_src)
{
  //TIMER============================================
  std::clock_t    start;
  start = std::clock();
  //TIMER============================================
  cv::initModule_nonfree();
  Segmentor * myseg = new Segmentor;
  Classifier * myclas = new Classifier;
  
  //map<string,unique_ptr<CvSVM>> classes_classifiers;//CvSVM tmp_SVM;

  vector<string> classes, svm_paths;
  vector<CvSVM*> classifiers;

  Ptr<FeatureDetector> detector =  makePtr<PyramidAdaptedFeatureDetector>(FeatureDetector::create("SIFT"),py_level); //detector
  Ptr<DescriptorExtractor > extractor = DescriptorExtractor::create("OpponentSIFT"); // Extractor
  Ptr<DescriptorMatcher > matcher(new BFMatcher);
  BOWImgDescriptorExtractor bowide(extractor,matcher);

  //cout << "Reading vocabulary from file: "<< vocab_path <<endl;
  Mat vocabulary;
  FileStorage fs(vocab_path, FileStorage::READ);
  fs["vocabulary"] >> vocabulary;
  fs.release();	
 
  bowide.setVocabulary(vocabulary);
  
  // Populate SVMs
  //#pragma omp for
  int svm_count = 0;
  for ( boost::filesystem::recursive_directory_iterator end, dir(svm_path); 
	dir != end; ++dir ) {
    if(boost::filesystem::is_regular_file(*dir)){
      vector<std::string> tmp_line;
      boost::split(tmp_line,dir->path().string(), boost::is_any_of("+"));
      
      std::string class_name = tmp_line[1].substr(0, tmp_line[1].size()-4);
      
      classes.push_back(class_name);
      classifiers.push_back(new CvSVM());
      svm_paths.push_back(dir->path().string());
      svm_count++;
    }
  }
  std::cout << "Preped file list and classes: " << (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << std::endl;
  
#pragma omp parallel for
  for(int i = 0; i < svm_count; i++){
    //std::cout << "int i:  " << i << endl;
    classifiers[i]->load(svm_paths[i].c_str());
  }

  std::cout << "Loaded SVMS: " << (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << std::endl;
  Mat img = imread(img_src), mask, response_hist, colour_hist,full_hist;      
  vector<KeyPoint> keypoints;
  myseg->getMask(img, mask);
  myclas->getFeatures( img, mask, full_hist, &bowide);
  
  float minf = FLT_MAX; string minclass;
 
  int pos;
  float private_minf;
  float res;
  int private_pos;
#pragma omp parallel private(private_minf,private_pos,res)
  {
    private_minf = FLT_MAX;
#pragma omp for
    for ( int i = 0; i < svm_count; i++) {
      res = classifiers[i]->predict(full_hist,true);
      //std::cout << "int i:  " << i << endl;
      if (res < private_minf ){
	private_minf = res;
	private_pos = i;
      }
    }
    if ( private_minf < minf ) {
#pragma omp critical
      {
	if ( private_minf < minf ){
	  minf = private_minf;
	  pos = private_pos;
	}
      }
    }
  }
  delete myseg;
  delete myclas;
  minclass = classes[pos];
  return minclass;
}
 

 
BOOST_PYTHON_MODULE(classifier)
{
  using namespace boost::python;
  def("classify", classify);
}
