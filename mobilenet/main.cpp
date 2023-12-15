#include "utils.h"
#include "convLayer.h"
#include <iostream>
#include <omp.h>
#include <cstdlib> // For std::atoi

bool g_DebugMode     = false; 
bool writeDataToFile = false;
int  numThreads      = omp_get_max_threads(); 

int main(int argc, char* argv[])
{
    //---------------------------Command line app logic
    bool testAllImages = false;
    if (argc > 1) {
        std::string firstArg(argv[1]);
        testAllImages = (firstArg == "test_all");
    }
    if (argc > 2) {
        std::string secondArg(argv[2]);
        writeDataToFile = (secondArg == "write_to_file");
        g_DebugMode = !writeDataToFile; 
    }
    if (argc > 3) {
        numThreads = std::atoi(argv[3]);
        if (numThreads <= 0) {
            numThreads = omp_get_max_threads();
        }
    }
    //--------------------------------------------------

    omp_set_num_threads(numThreads);
    std::cout<<"Using " << numThreads << " threads.\n";

    std::string testFile = testAllImages ? "../../data/test_list.txt" : "../../data/test_list_1image.txt";
    test(testFile.c_str(),
         "../../data/test_labels.txt",
         "../../data/images/");
}