#include "readdata.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <ctime>
#include <cassert>
#include <fstream>
#include <filesystem> // Requires C++17
namespace fs = std::filesystem;
using namespace std;

ReadData::ReadData(const char *pcMean, int nInputWidth, int nInputHeight, int nInputChannel):
			m_nInputWidth(nInputWidth), m_nInputHeight(nInputHeight), m_nInputChannel(nInputChannel)
{
	m_nImageSize = nInputWidth * nInputHeight;
	m_nInputSize = nInputWidth * nInputHeight * nInputChannel;
	m_pfInputData = new float[m_nInputSize];
	m_pfMean = new float[m_nInputSize];
    ReadMean(pcMean);
}

ReadData::~ReadData()
{
	delete[] m_pfInputData;
	delete[] m_pfMean;
}

float* ReadData::ReadInput(const char* pcName) {
    std::cout << "Reading Picture: " << pcName << "..." << std::endl;

    // Use cv::Mat for image representation
    cv::Mat srcImage = cv::imread(pcName, cv::IMREAD_UNCHANGED);
    if (srcImage.empty()) {
        std::cerr << "Error: Image not loaded." << std::endl;
        return nullptr;
    }

    // Resize image
    cv::Mat dstImage;
    cv::resize(srcImage, dstImage, cv::Size(m_nInputWidth, m_nInputHeight), 0, 0, cv::INTER_LINEAR);

    int nOutputIndex = 0;

    for (int i = 0; i < dstImage.rows; i++) {
        for (int j = 0; j < dstImage.cols; j++) {
            nOutputIndex = i * m_nInputWidth + j;
            cv::Vec3b pixel = dstImage.at<cv::Vec3b>(i, j);
            m_pfInputData[nOutputIndex] = static_cast<float>(pixel[0]) - m_pfMean[nOutputIndex];
            m_pfInputData[nOutputIndex + m_nImageSize] = static_cast<float>(pixel[1]) - m_pfMean[nOutputIndex + m_nImageSize];
            m_pfInputData[nOutputIndex + 2 * m_nImageSize] = static_cast<float>(pixel[2]) - m_pfMean[nOutputIndex + 2 * m_nImageSize];
        }
    }

    std::cout << "Reading Picture Done..." << std::endl;

    return m_pfInputData;
}

void ReadData::ReadMean(const char *pcMean)
{
	int nMsize, nMreadsize;
	FILE *pM;
    pM = fopen(pcMean, "rb");

	if (pM == NULL) {
		std::cerr << "Error: Unable to open file " << pcMean << std::endl;
		return;
	}

	nMsize = m_nInputSize;

	nMreadsize = fread(m_pfMean, sizeof(float), nMsize, pM);

	fclose(pM);
}