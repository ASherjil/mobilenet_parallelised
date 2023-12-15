#include "utils.h"
#include "Network.h"
#include <string>
#include <fstream>
#include <chrono>
using namespace std::chrono;


float Accuracy_1(float *pfPred, int *pnLab, int nclass)
{
    float flag = 0.0;
    for (int i = 0; i < nclass; i++)
    {
        //cout << pfPred[i] << ':' << pnLab[i] << endl;
        if (pfPred[i] >=0.5 && pnLab[i] == 1)
            flag = 1.0;
    }
    return flag;
}

float Accuracy_all(float *pfPred, int *pnLab, int nclass)
{
    float flag = 0.0, nSum = 0.0;
    int *pnPred = new int[nclass];
    for (int i = 0; i < nclass; i++)
    {
        pnPred[i] = 0;
        if (pfPred[i] >=0.5)
            pnPred[i] = 1;

        if (pnPred[i] == pnLab[i])
            nSum += 1;
    }

    if (nSum == 12.0)
        flag = 1.0;

    delete[] pnPred;
    return flag;
}

void test(const char *pcTestListFile, const char *pcTestLabelsFile, const char *pcTestImageSet)
{
    Network network;

    ifstream file(pcTestListFile);   //name list of test images
    ifstream lab(pcTestLabelsFile);  //label list of test image
    string filename;
    const char *file_name;
    string line, label;
    string filepath = pcTestImageSet;  //paht of image data

    int nNum = 3618, nClass = 12;
    float *pfPredict;
    int nLl[12];

    float fAccuracy_1 = 0.0;
    float fAccuracy_all = 0.0;

    int i = 0;

    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    if (file && lab)
    {
        while (getline(file, line) && getline(lab, label))
        {
            filename = filepath + line;
            file_name = filename.c_str();
            pfPredict = network.Forward(file_name);
            for (int j = 0; j < 12; j++)
                nLl[j] = label[2 * j] - '0'; 
            fAccuracy_1 += Accuracy_1(pfPredict, nLl, nClass);
            fAccuracy_all += Accuracy_all(pfPredict, nLl, nClass);
            i += 1;

        }
    }
    else
    {
        cout << "no such file" << endl;
    }
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>( t2 - t1 ).count();

    cout << "i:" << i << endl;

    float fAccurSum_1 = (fAccuracy_1 / i)*100;
    float fAccurSum_all = (fAccuracy_all / i)*100;

    cout << "accuracy_1:"   << fAccurSum_1     << "%"<<endl;
    cout << "accuracy_all:" << fAccurSum_all << "%"<<endl;
    cout << "Processing time: " << duration  << " ms" << endl;

    if (writeDataToFile){
        writeToFile(duration, "mobilenet_static", numThreads, "../../BenchmarkTimes");
    }

    //system("pause");
	
    // You can use following codes to test only one image
	//input an image
    /*
    network.Forward(".\\data\\0.jpg");
    network.Forward(".\\data\\1.jpg");
    system("pause");
    */
}

void writeToFile(double benchmark_time, const std::string& file_name, int threads, const std::filesystem::path& folder_path) {
    // Construct the full path with the folder path, file name, and number of threads
    auto full_path = folder_path / (file_name + "_" + std::to_string(threads) + "threads.txt");

    // Create the directory if it does not exist
    if (!std::filesystem::exists(folder_path)) {
        std::filesystem::create_directory(folder_path);
    }

    // Open the file for appending (creates file if it doesn't exist)
    std::ofstream file(full_path, std::ios::app);
    if (!file) {
        std::cerr << "Error opening file: " << full_path << std::endl;
        return;
    }

    // Write the benchmark time to the file
    file << benchmark_time << '\n';
}