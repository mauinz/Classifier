#ifndef _CLASSIFIER_H_
#define _CLASSIFIER_H_

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "../Segmentor/segmentor.hpp"
#include <map>
#include <string>

class Classifier{
private:
  void print2Dvector(std::vector<std::vector<std::string> > print);
  void save2Dvector(std::vector<std::vector<std::string> > print, int seed);
  void load2Dvector(std::vector<std::vector<std::string> > &print,std::string file_path);
public:
  // Constructor
  Classifier();
  // Destructor
  ~Classifier();
  // Main functionality of the class
  int getWords(std::string folderpath,int seed, bool verbose = false);
  int makeFileList(std::string folderpath, int seed);
  void checkFolders(std::string folderpath);
  void extractTrainingData(std::string filepath, std::map<std::string,cv::Mat>& classes_training_data, cv::Mat vocabulary, bool verbose = false);
  std::string trainSVM(std::string vocab_path, std::string train_path, int seed, bool verbose = false);
  std::string trainSVMParams(std::string vocab_path, std::string train_path, int seed, bool verbose = false);
  void testSVM(std::string seed_path, std::string vocab_path, std::string svm_path, int seed, bool verbose = false);
  void getHist(cv::Mat src, cv::Mat &res, Segmentor* myseg, bool verbose = false);
};

#endif
