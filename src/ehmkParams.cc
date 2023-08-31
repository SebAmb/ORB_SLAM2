#include "ehmkParams.h"

namespace EHMK_PARAMS
{
    bool DebugEHMK::isDebugEHMK = true;
    int DebugEHMK::idxImgEhmk = 0;
    std::string DebugEHMK::dirImgDebugEhmk = "/home/sharedWithHost/VO/Datasets/debugAlgos/ORB_SLAM2/KITTI";
    std::string DebugEHMK::dirOrbImgDebugEhmk = DebugEHMK::dirImgDebugEhmk + "/LeftImgOrb";

    DebugEHMK::DebugEHMK() {}

    DebugEHMK::DebugEHMK(const std::string& filePath)//: isDebugEHMK(true), idxImgEhmk(0), dirImgDebugEhmk(dirImageDebug)
    {
        isDebugEHMK = true;
        idxImgEhmk = 0;
        dirImgDebugEhmk = getDirOfFile(filePath);
        dirOrbImgDebugEhmk = dirImgDebugEhmk + "/LeftImgOrb";
        createDir(dirImgDebugEhmk);
        createDir(dirOrbImgDebugEhmk);
    }

    void DebugEHMK::saveLeftImageOrb(cv::Mat imL)
    {
        std::string im_name_debug = "left_" + toStringFormat(idxImgEhmk, 6) + ".png";
        cv::imwrite(dirOrbImgDebugEhmk + "/" + im_name_debug, imL);
        std::cout << " idxImgEhmk = " << idxImgEhmk << std::endl;
        std::cout << " dirOrbImgDebugEhmk = " << dirOrbImgDebugEhmk << std::endl;
        std::cout << " im_name_debug = " << im_name_debug << std::endl;
        idxImgEhmk++;
    }

    void DebugEHMK::setDebugDir(const std::string& dirDebug)
    {
        dirImgDebugEhmk = dirDebug;
    }

    bool DebugEHMK::getIsDebug()
    {
        return isDebugEHMK;
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
