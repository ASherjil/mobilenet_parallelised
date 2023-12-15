#ifndef UTILS_H
#define UTILS_H
#include <filesystem>

extern int  numThreads; // variable for storing the number of threads used
extern bool writeDataToFile; // variable to decide whether to write data to file or not 

float Accuracy_1(float *pfPred, int *pnLab, int nclass);
float Accuracy_all(float *pfPred, int *pnLab, int nclass);
void test(const char *pcTestListName, const char *pcTestLabelsName, const char *pcTestImageSet);
void writeToFile(double benchmark_time, const std::string& file_name, int threads, const std::filesystem::path& folder_path);

#endif
