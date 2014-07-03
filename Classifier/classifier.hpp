#ifndef _CLASSIFIER_H_
#define _CLASSIFIER_H_

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <map>
#include <string>

#define TEST_CASE 1
#define TRAIN_CASE 0

class Classifier{
private:

public:
  // Constructor
  Classifier();
  // Destructor
  ~Classifier();
  // Main functionality of the class
  int getSIFT(const char* argv, cv::Mat& _res);
  int getWords(const char * folderpath);
  int makeFileList(const char * folderpath, int seed);
  void checkFolders(const char * folderpath);
  void print2Dvector(std::vector<std::vector<std::string> > print);
  void save2Dvector(std::vector<std::vector<std::string> > print, int seed);
  void load2Dvector(std::vector<std::vector<std::string> > &print,std::string file_path);
  void extractTrainingData(std::string filepath, std::map<std::string,cv::Mat>& classes_training_data, cv::Mat vocabulary);
  void trainSVM(std::string vocab_path, std::string train_path);
  void testSVM(std::string seed_path, std::string vocab_path, std::string svm_path);
};

#endif
