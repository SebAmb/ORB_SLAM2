#include "ehmkParams.h"

namespace EHMK_PARAMS
{
    bool DebugEHMK::isPrintLogDebugEhmk = false;
    bool DebugEHMK::isPrintLogDebugOftenEhmk = false;
    bool DebugEHMK::isPrintLogDebugVeryOftenEhmk = false;
    bool DebugEHMK::isSaveDebugImagesEhmk = false;
    int DebugEHMK::idxImgEhmk = 0;
    std::string DebugEHMK::dirImgDebugEhmk = "/home/sharedWithHost/NoInitForDebug";
    std::string DebugEHMK::dirOrbImgDebugEhmk = DebugEHMK::dirImgDebugEhmk + "/LeftImgOrb";
    std::vector<cv::KeyPoint> DebugEHMK::keypointsL, DebugEHMK::keypointsR;
    std::vector<float> DebugEHMK::depths, DebugEHMK::uCoordLs, DebugEHMK::uCoordRs;
    std::vector<std::pair<int, int> > DebugEHMK::distBtwLeftRightAndIdxsL;
    float DebugEHMK::thresholdDistMatches, DebugEHMK::medianDistMatches;
    std::vector<float> DebugEHMK::depthsFilt, DebugEHMK::uCoordRsFilt;

    DebugEHMK::DebugEHMK() {}

    DebugEHMK::DebugEHMK(const std::string& filePath)
    {
        //isPrintLogDebugEhmk = true;
        isSaveDebugImagesEhmk = true;
        idxImgEhmk = 0;
        dirImgDebugEhmk = getDirOfFile(filePath);
        dirOrbImgDebugEhmk = dirImgDebugEhmk + "/LeftImgOrb";
        createDir(dirImgDebugEhmk);
        createDir(dirOrbImgDebugEhmk);
    }

    DebugEHMK::DebugEHMK(const std::string& filePath, 
                         const bool isSaveDebugImages,
                         const bool isDebugConsole, const bool isDebugConsoleOften, const bool isDebugConsoleVeryOften)
    {
        idxImgEhmk = 0;
        dirImgDebugEhmk = getDirOfFile(filePath);
        dirOrbImgDebugEhmk = dirImgDebugEhmk + "/LeftImgOrb";
        createDir(dirImgDebugEhmk);
        createDir(dirOrbImgDebugEhmk);
        isSaveDebugImagesEhmk = isSaveDebugImages;
        isPrintLogDebugEhmk = isDebugConsole;
        isPrintLogDebugOftenEhmk = isDebugConsoleOften;
        isPrintLogDebugVeryOftenEhmk = isDebugConsoleVeryOften;
    }

    void DebugEHMK::saveLeftImageOrb(cv::Mat imL)
    {
        std::string im_name_debug = "left_" + toStringFormat(idxImgEhmk, 6) + ".png";
        cv::imwrite(dirOrbImgDebugEhmk + "/" + im_name_debug, imL);
        if (false)
        {
            std::cout << " idxImgEhmk = " << idxImgEhmk << std::endl;
            std::cout << " dirOrbImgDebugEhmk = " << dirOrbImgDebugEhmk << std::endl;
            std::cout << " im_name_debug = " << im_name_debug << std::endl;
        }
        idxImgEhmk++;
    }


    void DebugEHMK::setDebugDir(const std::string& dirDebug) {
        dirImgDebugEhmk = dirDebug;
    }

    /*void DebugEHMK::setKeypoints(const std::vector<cv::KeyPoint>& kpL, const std::vector<cv::KeyPoint>& kpR) {
        keypointsL = kpL;
        keypointsR = kpR;
    }*/

    void DebugEHMK::setStereoMatchingInfos(const std::vector<cv::KeyPoint>& _kpL, const std::vector<cv::KeyPoint>& _kpR,
                                        const std::vector<float>& _depths, const std::vector<float>& _uLs, const std::vector<float>& _uRs,
                                        const std::vector<std::pair<int, int> > _pairDistLR_and_Idx)
    {
        keypointsL = _kpL;
        keypointsR = _kpR;
        depths = _depths;
        uCoordLs = _uLs;
        uCoordRs = _uRs;
        distBtwLeftRightAndIdxsL = _pairDistLR_and_Idx;
    }

    void DebugEHMK::setStereoMatchingFiltInfos(const float _thresholdDistMatches, const float _medianDistMatches,
                                               const std::vector<float> _depthsFilt, const std::vector<float> _uRsFilt)
    {
        thresholdDistMatches = _thresholdDistMatches;
        medianDistMatches = _medianDistMatches;
        depthsFilt = _depthsFilt;
        uCoordRsFilt = _uRsFilt;
    }

    bool DebugEHMK::getIsDebugConsole() {
        return isPrintLogDebugEhmk;
    }

    bool DebugEHMK::getIsDebugConsoleOften() {
        return isPrintLogDebugOftenEhmk;
    }

    bool DebugEHMK::getIsDebugConsoleVeryOften() {
        return isPrintLogDebugVeryOftenEhmk;
    }

    bool DebugEHMK::getIsSaveDebugImages() {
        return isSaveDebugImagesEhmk;
    }



    std::string DebugEHMK::getDirOfFile(const std::string& filePath)
    {
        size_t position_dir_tmp(filePath.find_last_of("/"));
        std::string save_dir_tmp(filePath.substr(0, position_dir_tmp));
        std::cout << " folder: " << save_dir_tmp << std::endl;
        return save_dir_tmp;
    }

    void DebugEHMK::createDir(const std::string& dirFile)
    {
        std::string cmd_save_dir_tmp("mkdir -p " + dirFile);
        //cout << " file: " << filePath.substr(found+1) << endl;
        int system_return_val = system(cmd_save_dir_tmp.c_str());
        if(system_return_val == -1)
        {
            // The system method failed
            std::cerr << "The command '" << cmd_save_dir_tmp << "' failed." << std::endl;
        }
    }

    std::string DebugEHMK::toStringFormat(int number, int length)
    {
        /*
        convert int into legnght digits
        eg: nb=2 & lenght=4 => returns "0004"
        */
        
        std::string num_str = std::to_string(number);
        
        if(int (num_str.length()) >= length) return num_str;
        
        std::string leading_zeros(length - num_str.length(), '0');
        
        return leading_zeros + num_str;
    }
}
