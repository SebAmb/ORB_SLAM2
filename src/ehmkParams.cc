#include "ehmkParams.h"

namespace EHMK_PARAMS
{
    std::string extSaveImg = ".png";
    bool DebugEHMK::isPrintLogDebugEhmk = false;
    bool DebugEHMK::isPrintLogDebugOftenEhmk = false;
    bool DebugEHMK::isPrintLogDebugVeryOftenEhmk = false;
    bool DebugEHMK::isSaveDebugImagesEhmk = false;
    int DebugEHMK::idxImgEhmk = 0;
    std::string DebugEHMK::dirImgDebugEhmk = "/home/sharedWithHost/NoInitForDebug";
    std::string DebugEHMK::dirOrbImgDebugEhmk = DebugEHMK::dirImgDebugEhmk + "/LeftImgOrb";
    std::string DebugEHMK::dirPangolinViewImgDebugEhmk = DebugEHMK::dirImgDebugEhmk + "/PangolinViewImg";
    std::string DebugEHMK::dirOrbStereoDebugEhmk = DebugEHMK::dirImgDebugEhmk + "/OrbStereo";
    std::string DebugEHMK::dirOrbF2FDebugEhmk = DebugEHMK::dirImgDebugEhmk + "/OrbFrame2Frame";
    std::string DebugEHMK::dirOrbLocalMapDebugEhmk = DebugEHMK::dirImgDebugEhmk + "/OrbLocalMap";
    std::string DebugEHMK::dirF2FPoseOptimDebugEhmk = DebugEHMK::dirImgDebugEhmk + "/f2fPoseOptim";
    std::vector<cv::KeyPoint> DebugEHMK::keypointsL, DebugEHMK::keypointsR;
    std::vector<float> DebugEHMK::depths, DebugEHMK::disparities, DebugEHMK::uCoordLs, DebugEHMK::uCoordRs;
    std::vector<int> DebugEHMK::idxKeypointR, DebugEHMK::idxKeypointReprojected;
    std::vector<std::pair<int, int> > DebugEHMK::distBtwLeftRightAndIdxsL;
    float DebugEHMK::thresholdDistMatches, DebugEHMK::medianDistMatches;
    std::vector<float> DebugEHMK::depthsFilt, DebugEHMK::uCoordRsFilt;
    std::vector<float> DebugEHMK::f2f_reprojErrLeft, DebugEHMK::f2f_reprojErrRight, DebugEHMK::f2f_distFeatDescLeft;
    // std::vector<float> DebugEHMK::f2f_distFeatDescRight;
    std::vector<int> DebugEHMK::f2f_leftFeatAge;

    DebugEHMK::DebugEHMK() {}

    DebugEHMK::DebugEHMK(const std::string& filePath)
    {
        //isPrintLogDebugEhmk = true;
        //isSaveDebugImagesEhmk = true;
        idxImgEhmk = 0;
        dirImgDebugEhmk = getDirOfFile(filePath);
        dirOrbImgDebugEhmk = dirImgDebugEhmk + "/LeftImgOrb";
        dirPangolinViewImgDebugEhmk = dirImgDebugEhmk + "/PangolinViewImg";
        dirOrbStereoDebugEhmk = dirImgDebugEhmk + "/OrbStereo";
        dirOrbF2FDebugEhmk = dirImgDebugEhmk + "/OrbFrame2Frame";
        dirOrbLocalMapDebugEhmk = dirImgDebugEhmk + "/OrbLocalMap";
        dirF2FPoseOptimDebugEhmk = dirImgDebugEhmk + "/f2fPoseOptim";
        createDir(dirImgDebugEhmk);
        createDir(dirOrbImgDebugEhmk);
        createDir(dirPangolinViewImgDebugEhmk);
        createDir(dirOrbStereoDebugEhmk);
        createDir(dirOrbF2FDebugEhmk);
        createDir(dirOrbLocalMapDebugEhmk);
        createDir(dirF2FPoseOptimDebugEhmk);
    }

    DebugEHMK::DebugEHMK(const std::string& filePath, 
                         const bool isSaveDebugImages,
                         const bool isDebugConsole, const bool isDebugConsoleOften, const bool isDebugConsoleVeryOften)
    {
        idxImgEhmk = 0;
        dirImgDebugEhmk = getDirOfFile(filePath);
        dirOrbImgDebugEhmk = dirImgDebugEhmk + "/LeftImgOrb";
        dirPangolinViewImgDebugEhmk = dirImgDebugEhmk + "/PangolinViewImg";
        dirOrbStereoDebugEhmk = dirImgDebugEhmk + "/OrbStereo";
        dirOrbF2FDebugEhmk = dirImgDebugEhmk + "/OrbFrame2Frame";
        dirOrbLocalMapDebugEhmk = dirImgDebugEhmk + "/OrbLocalMap";
        dirF2FPoseOptimDebugEhmk = dirImgDebugEhmk + "/f2fPoseOptim";
        createDir(dirImgDebugEhmk);
        createDir(dirOrbImgDebugEhmk);
        createDir(dirPangolinViewImgDebugEhmk);
        createDir(dirOrbStereoDebugEhmk);
        createDir(dirOrbF2FDebugEhmk);
        createDir(dirOrbLocalMapDebugEhmk);
        createDir(dirF2FPoseOptimDebugEhmk);
        isSaveDebugImagesEhmk = isSaveDebugImages;
        isPrintLogDebugEhmk = isDebugConsole;
        isPrintLogDebugOftenEhmk = isDebugConsoleOften;
        isPrintLogDebugVeryOftenEhmk = isDebugConsoleVeryOften;
    }


    void DebugEHMK::CaptureAndSavePangolinImage(const pangolin::View& view)
    {
        std::string pangolin_img_name_debug = "pangolin_" + toStringFormat(idxImgEhmk, 6) + extSaveImg;
        if (false)
        {
            std::cout << " idxImgEhmk = " << idxImgEhmk << std::endl;
            std::cout << " dirPangolinViewImgDebugEhmk = " << dirPangolinViewImgDebugEhmk << std::endl;
            std::cout << " pangolin_img_name_debug = " << pangolin_img_name_debug << std::endl;
        }

        // get view dimension
        pangolin::Viewport view_dim = pangolin::DisplayBase().GetBounds();

        int width = view_dim.w;
        int height = view_dim.h;

        // Créez un tableau pour stocker les pixels de l'image
        std::vector<uint8_t> buffer(3 * width * height);

        // Capturez le contenu de la vue
        glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, buffer.data());

        // Inversez l'image (OpenGL stocke les lignes à l'envers)
        cv::Mat image(height, width, CV_8UC3);
        for (int i = 0; i < height; ++i) {
            memcpy(image.ptr(i), buffer.data() + (height - 1 - i) * 3 * width, 3 * width);
        }

        cv::imwrite(dirPangolinViewImgDebugEhmk + "/" + pangolin_img_name_debug, image);
    }


    void DebugEHMK::saveLeftImageOrb(const cv::Mat& imL)
    {
        std::string im_name_debug = "left_" + toStringFormat(idxImgEhmk, 6) + extSaveImg;
        cv::imwrite(dirOrbImgDebugEhmk + "/" + im_name_debug, imL);
        if (false)
        {
            std::cout << " idxImgEhmk = " << idxImgEhmk << std::endl;
            std::cout << " dirOrbImgDebugEhmk = " << dirOrbImgDebugEhmk << std::endl;
            std::cout << " im_name_debug = " << im_name_debug << std::endl;
        }
        idxImgEhmk++;
    }



    void DebugEHMK::savePoseOptimization1Frame(const std::vector<std::vector<float>>& _chi2PoseOptim1FrameMono,
                                               const std::vector<std::vector<float>>& _chi2PoseOptim1FrameStereo,
                                               const std::vector<std::vector<bool>>& _isOutlierPoseOptim1Frame)
    {
        if ((_chi2PoseOptim1FrameMono.size() != _chi2PoseOptim1FrameStereo.size()) 
            || (_chi2PoseOptim1FrameMono.size() != _isOutlierPoseOptim1Frame.size())
            || (_chi2PoseOptim1FrameMono[0].size() != _chi2PoseOptim1FrameStereo[0].size()) 
            || (_chi2PoseOptim1FrameMono[0].size() != _isOutlierPoseOptim1Frame[0].size()))
        {
            std::cerr << "Error: size of vectors in savePoseOptimization1Frame function are different." << std::endl;
        }

        std::string csv_filename("f2f_pose_optim_" + toStringFormat(idxImgEhmk, 6) + ".csv");
        const std::string path_csv_file = dirF2FPoseOptimDebugEhmk + "/" + csv_filename;
        if (false)
        {
            std::cout << " idxImgEhmk = " << idxImgEhmk << std::endl;
            std::cout << " dirF2FPoseOptimDebugEhmk = " << dirF2FPoseOptimDebugEhmk << std::endl;
            std::cout << " csv_filename = " << csv_filename << std::endl;
        }

        // save data in csv
        std::ofstream csv_file(path_csv_file);
        if (csv_file.is_open())
        {
            csv_file << "indexPoint, chi2Mono_0, chi2Stereo_0, isOutlier_0, " <<
                        "chi2Mono_1, chi2Stereo_1, isOutlier_1, " << 
                        "chi2Mono_2, chi2Stereo_2, isOutlier_2, " << 
                        "chi2Mono_3, chi2Stereo_3, isOutlier_3, " << 
                        std::endl;
            //std::cout << "max index csv file row = " << f2f_leftFeatAge.size() << std::endl;
            for (size_t i = 0; i < _chi2PoseOptim1FrameMono[0].size(); i++)
            {
                //std::cout << "index csv file row = " << i << std::endl;
                csv_file << std::fixed << 
                            i << "," << 
                            std::setprecision(5) << 
                            _chi2PoseOptim1FrameMono[0][i] << "," << _chi2PoseOptim1FrameStereo[0][i] << "," << _isOutlierPoseOptim1Frame[0][i] << "," <<
                            _chi2PoseOptim1FrameMono[1][i] << "," << _chi2PoseOptim1FrameStereo[1][i] << "," << _isOutlierPoseOptim1Frame[1][i] << "," <<
                            _chi2PoseOptim1FrameMono[2][i] << "," << _chi2PoseOptim1FrameStereo[2][i] << "," << _isOutlierPoseOptim1Frame[2][i] << "," <<
                            _chi2PoseOptim1FrameMono[3][i] << "," << _chi2PoseOptim1FrameStereo[3][i] << "," << _isOutlierPoseOptim1Frame[3][i] << "," <<
                            std::endl;
            }
            csv_file.close();
        }
        else
        {
            std::cerr << "Cannot open Csv file (" << csv_filename.c_str() << ")." << std::endl;
        }
    }


    void DebugEHMK::setDebugDir(const std::string& dirDebug) {
        dirImgDebugEhmk = dirDebug;
    }


    void DebugEHMK::setStereoMatchingInfos(const std::vector<cv::KeyPoint>& _kpL, const std::vector<cv::KeyPoint>& _kpR,
                                           const std::vector<int>& _idxKpR,
                                           const std::vector<float>& _depths, const std::vector<float>& _disparities, 
                                           const std::vector<float>& _uLs, const std::vector<float>& _uRs,
                                           const std::vector<std::pair<int, int> >& _pairDistLR_and_Idx)
    {
        keypointsL = _kpL;
        keypointsR = _kpR;
        idxKeypointR = _idxKpR;
        depths = _depths;
        disparities = _disparities;
        uCoordLs = _uLs;
        uCoordRs = _uRs;
        distBtwLeftRightAndIdxsL = _pairDistLR_and_Idx;
        if (false)
        {
            if ((keypointsL.size() != disparities.size()) || (keypointsL.size() != depths.size()) || (keypointsL.size() != uCoordLs.size())
                || (keypointsL.size() != uCoordRs.size()))
            {
                std::cout << "sizes: \n" << 
                            "keypointsL = " << keypointsL.size() << " , depths = " << depths.size() << 
                            " , disparities = " << disparities.size() << " , uCoordLs = " << uCoordLs.size() << 
                            " , uCoordRs = " << uCoordRs.size() <<
                            std::endl;
                std::cerr << "Error: size of vectors in setStereoMatchingInfos function are different." << std::endl;
            }
            else if (keypointsR.size() != idxKeypointR.size())
            {
                std::cout << "sizes: \n" << 
                            "keypointsR = " << keypointsR.size() << " , idxKeypointR = " << idxKeypointR.size() << 
                            std::endl;
                std::cerr << "Error: size of vectors in setStereoMatchingInfos function are different." << std::endl;
            }
        }
    }

    void DebugEHMK::setStereoMatchingFiltInfos(const float _thresholdDistMatches, const float _medianDistMatches,
                                               const std::vector<float>& _depthsFilt, const std::vector<float>& _uRsFilt)//, const std::vector<int>& _idxKpR)
    {
        thresholdDistMatches = _thresholdDistMatches;
        medianDistMatches = _medianDistMatches;
        depthsFilt = _depthsFilt;
        uCoordRsFilt = _uRsFilt;
        //idxKeypointR = _idxKpR;
        if (depthsFilt.size() != uCoordRsFilt.size())
        {
            std::cerr << "Error: size of vectors in setStereoMatchingFiltInfos function are different." << std::endl;
        }

        if (false)
        {
            std::cout << "keypointsL = " << keypointsL.size() << 
                        " , keypointsR = " << keypointsR.size() << 
                        " , depths = " << depths.size() << 
                        " , disparities = " << disparities.size() << 
                        " , uCoordLs = " << uCoordLs.size() << 
                        " , uCoordRs = " << uCoordRs.size() << 
                        " , idxKeypointR = " << idxKeypointR.size() << 
                        " , distBtwLeftRightAndIdxsL = " << distBtwLeftRightAndIdxsL.size() << 
                        " , depthsFilt = " << depthsFilt.size() << 
                        " , uCoordRsFilt = " << uCoordRsFilt.size() << 
                        std::endl;
        }

        std::string csv_filename = "stereo_orb_" + toStringFormat(idxImgEhmk, 6) + ".csv";
        std::string path_csv_file = dirOrbStereoDebugEhmk + "/" + csv_filename;
        if (false)
        {
            std::cout << " idxImgEhmk = " << idxImgEhmk << std::endl;
            std::cout << " dirOrbStereoDebugEhmk = " << dirOrbStereoDebugEhmk << std::endl;
            std::cout << " csv_filename = " << csv_filename << std::endl;
        }

        // save data in csv
        std::ofstream csv_file(path_csv_file);
        if (csv_file.is_open())
        {
            csv_file << "keypointsLeft_X, keypointsLeft_Y, " <<
                        "depths, disparities, " <<
                        "uCoordLefts, uCoordRights, " << 
                        "indexKeypointRight, " << 
                        "thresholdDistMatches, medianDistMatches, depthsFilt, uCoordRightsFilt" <<
                        std::endl;
            
            for (size_t i = 0; i < depthsFilt.size(); i++)
            {
                csv_file << std::fixed << 
                            std::setprecision(2) << keypointsL[i].pt.x << "," << keypointsL[i].pt.y << "," <<
                            std::setprecision(2) << depths[i] << "," << disparities[i] << "," <<
                            std::setprecision(2) << uCoordLs[i] << "," << uCoordRs[i] << "," << idxKeypointR[i] << "," <<
                            std::setprecision(5) << thresholdDistMatches << "," << medianDistMatches << "," <<
                            std::setprecision(2) << depthsFilt[i] << "," << uCoordRsFilt[i] << "," << 
                            std::endl;
            }
            csv_file.close();
        }
        else
        {
            std::cerr << "Cannot open Csv file (" << csv_filename.c_str() << ")." << std::endl;
        }

        //**************************************************
        //**************************************************

        csv_filename = "stereo_orb_" + toStringFormat(idxImgEhmk, 6) + "_2.csv";
        path_csv_file = dirOrbStereoDebugEhmk + "/" + csv_filename;
        if (false)
        {
            std::cout << " idxImgEhmk = " << idxImgEhmk << std::endl;
            std::cout << " dirOrbStereoDebugEhmk = " << dirOrbStereoDebugEhmk << std::endl;
            std::cout << " csv_filename = " << csv_filename << std::endl;
        }

        // save data in csv
        std::ofstream csv_file_2(path_csv_file);
        if (csv_file_2.is_open())
        {
            csv_file_2 << "keypointsRight_X, keypointsRight_Y, " << std::endl;
            
            for (size_t i = 0; i < keypointsR.size(); i++)
            {
                csv_file_2 << std::fixed << 
                            std::setprecision(2) << keypointsR[i].pt.x << "," << keypointsR[i].pt.y << "," <<
                            std::endl;
            }
            csv_file_2.close();
        }
        else
        {
            std::cerr << "Cannot open Csv file (" << csv_filename.c_str() << ")." << std::endl;
        }

        //**************************************************
        //**************************************************

        csv_filename = "stereo_orb_" + toStringFormat(idxImgEhmk, 6) + "_3.csv";
        path_csv_file = dirOrbStereoDebugEhmk + "/" + csv_filename;
        if (false)
        {
            std::cout << " idxImgEhmk = " << idxImgEhmk << std::endl;
            std::cout << " dirOrbStereoDebugEhmk = " << dirOrbStereoDebugEhmk << std::endl;
            std::cout << " csv_filename = " << csv_filename << std::endl;
        }

        // save data in csv
        std::ofstream csv_file_3(path_csv_file);
        if (csv_file_3.is_open())
        {
            csv_file_3 << "distBtwLeftRight, idxLeftForDistBtwLeftRight, " << 
                        std::endl;
            
            for (size_t i = 0; i < distBtwLeftRightAndIdxsL.size(); i++)
            {
                csv_file_3 << std::fixed << 
                            std::setprecision(2) << distBtwLeftRightAndIdxsL[i].first << "," << distBtwLeftRightAndIdxsL[i].second << "," <<
                            std::endl;
            }
            csv_file_3.close();
        }
        else
        {
            std::cerr << "Cannot open Csv file (" << csv_filename.c_str() << ")." << std::endl;
        }

        //**************************************************
        //**************************************************

        thresholdDistMatches = -1.0;
        medianDistMatches = -1.0;
        depthsFilt.clear();
        uCoordRsFilt.clear();
        keypointsL.clear();
        keypointsR.clear();
        depths.clear();
        disparities.clear();
        uCoordLs.clear();
        uCoordRs.clear();
        distBtwLeftRightAndIdxsL.clear();
    }

    void DebugEHMK::setFrame2FrameReProjErrInfos(const std::vector<int>& _idxKpReproj, 
                                                 const std::vector<float>& _reprojErrLeft, const std::vector<float>& _reprojErrRight,
                                                 const std::vector<float>& _distFeatDescLeft, //const std::vector<float>& _distFeatDescRight,
                                                 const std::vector<int>& _leftFeatAge, const bool isFiltered)
    {
        //std::cout << "debut function setFrame2FrameReProjErrInfos " << std::endl;
        idxKeypointReprojected = _idxKpReproj;
        f2f_reprojErrLeft = _reprojErrLeft;
        f2f_reprojErrRight = _reprojErrRight;
        f2f_distFeatDescLeft = _distFeatDescLeft;
        f2f_leftFeatAge = _leftFeatAge;
        
        if ((f2f_reprojErrLeft.size() != f2f_reprojErrRight.size()) || (f2f_reprojErrLeft.size() != f2f_distFeatDescLeft.size())
            || (f2f_reprojErrLeft.size() != f2f_leftFeatAge.size()))
        {
            std::cerr << "Error: size of vectors in setFrame2FrameReProjErrInfos function are different." << std::endl;
        }

        std::string csv_filename("f2f_orb_" + toStringFormat(idxImgEhmk, 6) + ".csv");
        if (isFiltered)
            csv_filename = "f2f_orb_" + toStringFormat(idxImgEhmk, 6) + "_filtered.csv";
        const std::string path_csv_file = dirOrbF2FDebugEhmk + "/" + csv_filename;
        if (false)
        {
            std::cout << " idxImgEhmk = " << idxImgEhmk << std::endl;
            std::cout << " dirOrbF2FDebugEhmk = " << dirOrbF2FDebugEhmk << std::endl;
            std::cout << " csv_filename = " << csv_filename << std::endl;
        }

        // save data in csv
        std::ofstream csv_file(path_csv_file);
        if (csv_file.is_open())
        {
            csv_file << "indexKpReprojected, reprojectionErrorLeft, reprojectionErrorRight, " <<
                        "distFeatDescriptorLeft, leftFeatureAge, " <<
                        std::endl;
            //std::cout << "max index csv file row = " << f2f_leftFeatAge.size() << std::endl;
            for (size_t i = 0; i < f2f_leftFeatAge.size(); i++)
            {
                //std::cout << "index csv file row = " << i << std::endl;
                csv_file << std::fixed << 
                            _idxKpReproj[i] << "," <<
                            std::setprecision(5) << f2f_reprojErrLeft[i] << "," << f2f_reprojErrRight[i] << "," <<
                            std::setprecision(5) << f2f_distFeatDescLeft[i] << "," << f2f_leftFeatAge[i] << "," <<
                            std::endl;
            }
            csv_file.close();
        }
        else
        {
            std::cerr << "Cannot open Csv file (" << csv_filename.c_str() << ")." << std::endl;
        }

        f2f_reprojErrLeft.clear();
        f2f_reprojErrRight.clear();
        f2f_distFeatDescLeft.clear();
        f2f_leftFeatAge.clear();
        //std::cout << "end fucntion setFrame2FrameReProjErrInfos " << std::endl;
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

    //template<typename myVector>
    //void saveCsv(const std::vector<myVector>& vecToSave);
    //void DebugEHMK::saveCsv(const std::vector<myVector>& vecToSave, const std::string pathCsvFile)
    //void DebugEHMK::saveCsv(const std::vector<float> vecToSave)
    //{
        //std::ofstream csv_file(pathCsvFile);
        /*if (csv_file.is_open())
        {
            csv_file << "Vecteur1, Vecteur2, Vecteur3" << std::endl;
            for (size_t i = 0; i < vecteur1.size(); ++i)
            {
                csv_file << std::fixed << std::setprecision(2) << vecteur1[i] << ","
                        << vecteur2[i] << ","
                        << vecteur3[i] << std::endl;
            }
            csv_file.close();
        }
        else
        {
            std::cerr << "Cannot open Csv file." << std::endl;
        }*/
    //}
}
