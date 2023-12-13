#include "utils.h"
#include <iostream>
#include <omp.h>
#include <cstdlib> // For std::atoi

int main(int argc, char* argv[])
{
    int numThreads = omp_get_max_threads(); // Default to max available threads

    // Check if the user has specified the number of threads as an argument
    if (argc > 1) {
        numThreads = std::atoi(argv[1]);
        if (numThreads <= 0) {
            numThreads = omp_get_max_threads();
        }
    }

    // Set the number of threads for OpenMP
    omp_set_num_threads(numThreads);
    std::cout << "Using " << numThreads << " threads.\n";

    // Your function call
    test("../../data/test_list.txt",
         "../../data/test_labels.txt",
         "../../data/images/");
    
    return 0;
}