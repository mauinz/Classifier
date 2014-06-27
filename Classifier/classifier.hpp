#ifndef _CLASSIFIER_H_
#define _CLASSIFIER_H_

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
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
  int makeFileList(const char * folderpath);
  void checkFolders(const char * folderpath);
void print2Dvector(std::vector<std::vector<std::string> > print);
void save2Dvector(std::vector<std::vector<std::string> > print, int seed);
void load2Dvector(std::vector<std::vector<std::string> > &print,std::string file_path);
};

#endif
