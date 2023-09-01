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

            DebugEHMK(const std::string& filePath, 
                      const bool isSaveDebugImages,
                      const bool isDebugConsole, const bool isDebugConsoleOften, const bool isDebugConsoleVeryOften);

            void saveLeftImageOrb(cv::Mat imL);


            void setDebugDir(const std::string& dirDebug);
            //void setKeypointsStereo(const std::vector<cv::KeyPoint>& kpL, const std::vector<cv::KeyPoint>& kpR);
            void setStereoMatchingInfos(const std::vector<cv::KeyPoint>& _kpL, const std::vector<cv::KeyPoint>& _kpR,
                                        const std::vector<float>& _depths, const std::vector<float>& _uLs, const std::vector<float>& _uRs,
                                        const std::vector<std::pair<int, int> > _pairDistLR_and_Idx);

            void setStereoMatchingFiltInfos(const float _thresholdDistMatches, const float _medianDistMatches,
                                            const std::vector<float> _depthsFilt, const std::vector<float> _uRsFilt);
            
            // TODO
            void setFrame2FrameReProjErrInfos(const std::vector<float>& _reprojErrLeft, const std::vector<float>& _reprojErrRight,
                                              const std::vector<float>& _distFeatDescLeft, //const std::vector<float>& _distFeatDescRight,
                                              const std::vector<float>& _leftFeatAge);

            bool getIsDebugConsole();
            bool getIsDebugConsoleOften(); // will print too much logs...
            bool getIsDebugConsoleVeryOften(); // will print too toooo much logs...
            bool getIsSaveDebugImages();
        
        private:
            static bool isPrintLogDebugEhmk;
            static bool isPrintLogDebugOftenEhmk; // will print too much logs...
            static bool isPrintLogDebugVeryOftenEhmk; // will print too toooo much logs...
            static bool isSaveDebugImagesEhmk;
            static int idxImgEhmk;
            static std::string dirImgDebugEhmk;
            static std::string dirOrbImgDebugEhmk;
            //
            // for Stereo
            //
            // coordinates of left & right keypoints
            static std::vector<cv::KeyPoint> keypointsL, keypointsR;
            // depths, x coordinates of left keypoints, x coordinates of right keypoints
            static std::vector<float> depths, uCoordLs, uCoordRs;
            // pair : (distances between left and right) & (indexes of corresponding left image keypoints)
            static std::vector<std::pair<int, int> > distBtwLeftRightAndIdxsL;
            // matches with dist >= threshold are removed. threshold = median * 1.5 * 1.4
            static float thresholdDistMatches, medianDistMatches;
            // depths and x coordinates of right keypoints after filtering
            static std::vector<float> depthsFilt, uCoordRsFilt;

            // TODO: add age & reprojection errors variables

            std::string getDirOfFile(const std::string& filePath);

            void createDir(const std::string& dirFile);

            std::string toStringFormat(int number, int length);
    };
    
} // namespace EHMK_PARAMS

#endif // EHMK_PARAMS_H
