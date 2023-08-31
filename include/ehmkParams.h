#ifndef EHMK_PARAMS_H
#define EHMK_PARAMS_H

#pragma once

#include<string>
#include<iostream>
#include<algorithm>
#include<fstream>
#include<iomanip>
#include<chrono>

#include<opencv2/core/core.hpp>
#include<opencv2/features2d/features2d.hpp>
#include <opencv2/opencv.hpp>

namespace EHMK_PARAMS
{
    class DebugEHMK
    {
        public:
            DebugEHMK();

            DebugEHMK(const std::string& filePath);

            void setDebugDir(const std::string& dirDebug);

            void saveLeftImageOrb(cv::Mat imL);

            bool getIsDebug();
        
        private:
            static bool isDebugEHMK;
            static int idxImgEhmk;
            static std::string dirImgDebugEhmk;
            static std::string dirOrbImgDebugEhmk;

            std::string getDirOfFile(const std::string& filePath);

            void createDir(const std::string& dirFile);

            std::string toStringFormat(int number, int length);
    };
    
} // namespace EHMK_PARAMS

#endif // EHMK_PARAMS_H
