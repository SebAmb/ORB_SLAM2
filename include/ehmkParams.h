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

/*#include <matplotlibcpp.h>
namespace plt = matplotlibcpp;*/

#include <pangolin/pangolin.h>
#include <GL/glu.h>

namespace EHMK_PARAMS
{
    class DebugEHMK
    {
        public:
            DebugEHMK();

            DebugEHMK(const std::string& _filePath);

            DebugEHMK(const std::string& _filePath, 
                      const bool _isSaveDebugImages,
                      const bool _isDebugConsole, const bool _isDebugConsoleOften, const bool _isDebugConsoleVeryOften);

            void saveLeftImageOrb(const cv::Mat& _imL);
            
            void CaptureAndSavePangolinImage(const pangolin::View& _view);

            void saveConcatenatedDebugImgLeftOrbAndPangolinView(const pangolin::View& _view, const cv::Mat& _imL);


            void setDebugDir(const std::string& _dirDebug);

            void setImagesPath(const std::vector<std::string>& _pathsImgLeft, const std::vector<std::string>& _pathsImgRight);

            void setStereoMatchingInfos(const std::vector<cv::KeyPoint>& _kpL, const std::vector<cv::KeyPoint>& _kpR,
                                        const std::vector<int>& _idxKpR,
                                        const std::vector<float>& _depths, const std::vector<float>& _disparities, 
                                        const std::vector<float>& _uLs, const std::vector<float>& _uRs,
                                        const std::vector<std::pair<int, int> >& _pairDistLR_and_Idx);

            void setStereoMatchingFiltInfos(const float _thresholdDistMatches, const float _medianDistMatches,
                                            const std::vector<float>& _depthsFilt, const std::vector<float>& _uRsFilt);//, const std::vector<int>& _idxKpR);
            
            void setFrame2FrameReProjErrInfos(const std::vector<int>& _idxKpReproj, 
                                              const std::vector<float>& _reprojErrLeft, const std::vector<float>& _reprojErrRight,
                                              const std::vector<float>& _distFeatDescLeft, //const std::vector<float>& _distFeatDescRight,
                                              const std::vector<int>& _leftFeatAge, const bool isFiltered);
            
            void savePoseOptimization1Frame(const std::vector<std::vector<float>>& _chi2PoseOptim1FrameMono,
                                            const std::vector<std::vector<float>>& _chi2PoseOptim1FrameStereo,
                                            const std::vector<std::vector<bool>>& _isOutlierPoseOptim1Frame);

            bool getIsDebugConsole();
            bool getIsDebugConsoleOften(); // will print too much logs...
            bool getIsDebugConsoleVeryOften(); // will print too toooo much logs...
            bool getIsSaveDebugImages();
        
        private:
            // TODO: enlever pas mal de variables static
            static bool isPrintLogDebugEhmk;
            static bool isPrintLogDebugOftenEhmk; // will print too much logs...
            static bool isPrintLogDebugVeryOftenEhmk; // will print too toooo much logs...
            static bool isSaveDebugImagesEhmk;
            static int idxImgEhmk;
            static std::vector<std::string> pathsImgLeft, pathsImgRight;
            static std::string dirImgDebugEhmk;
            static std::string dirConcatOrbPangolinImgDebugEhmk, dirOrbImgDebugEhmk, dirPangolinViewImgDebugEhmk;
            static std::string dirOrbStereoDebugEhmk, dirOrbF2FDebugEhmk, dirOrbLocalMapDebugEhmk, dirF2FPoseOptimDebugEhmk; // save feature displacement, reprojection errors, etc.
            //
            // for Stereo
            //
            // coordinates of left & right keypoints
            static std::vector<cv::KeyPoint> keypointsL, keypointsR;
            static std::vector<int> idxKeypointR, idxKeypointReprojected;
            // depths, x coordinates of left keypoints, x coordinates of right keypoints
            static std::vector<float> depths, disparities, uCoordLs, uCoordRs;
            // pair : (distances between left and right) & (indexes of corresponding left image keypoints)
            static std::vector<std::pair<int, int> > distBtwLeftRightAndIdxsL;
            // matches with dist >= threshold are removed. threshold = median * 1.5 * 1.4
            static float thresholdDistMatches, medianDistMatches;
            // depths and x coordinates of right keypoints after filtering
            static std::vector<float> depthsFilt, uCoordRsFilt;
            // frame to frame (f2f) reprojection infos
            static std::vector<float> f2f_reprojErrLeft, f2f_reprojErrRight, f2f_distFeatDescLeft; // f2f_distFeatDescRight;
            static std::vector<int> f2f_leftFeatAge;

            // TODO: add age & reprojection errors variables
            /*template<typename myVector>
            void saveCsv(const std::vector<myVector>& vecToSave, const std::string pathCsvFile);*/
            //void saveCsv(std::vector<float> vecToSave);

            std::string getDirOfFile(const std::string& filePath);

            void createDir(const std::string& dirFile);

            std::string toStringFormat(int number, int length);

            void ConcatImages(const cv::Mat& img1, const cv::Mat& img2, cv::Mat& concatImg);

            std::string getBasenameOfPath(const std::string& path);

            std::string getBasenameOfPathWoExt(const std::string& path);

    };
    
} // namespace EHMK_PARAMS

#endif // EHMK_PARAMS_H
