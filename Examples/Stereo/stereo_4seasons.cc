/**
* This file is part of ORB-SLAM2.
*
* Copyright (C) 2014-2016 Raúl Mur-Artal <raulmur at unizar dot es> (University of Zaragoza)
* For more information see <https://github.com/raulmur/ORB_SLAM2>
*
* ORB-SLAM2 is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* ORB-SLAM2 is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with ORB-SLAM2. If not, see <http://www.gnu.org/licenses/>.
*/


#include<iostream>
#include<algorithm>
#include<fstream>
#include<iomanip>
#include<chrono>
//#include <sstream>
#include<algorithm>

#include<opencv2/core/core.hpp>

#include<System.h>
#include "ehmkParams.h"

using namespace std;


bool isFileExist(const std::string &filePath)
{
    std::ifstream file(filePath);
    return file.good();
}


void checkIfFileExist(std::string strPathToFile)
{
    if (!isFileExist(strPathToFile))
        std::cerr << "the file (" << strPathToFile << ") does not exist" << std::endl;
}


void LoadImages(const std::string &strPathToSequence, 
                const std::string &sequenceID, const std::string &dateAcquisition,
                std::vector<std::string> &vstrImageLeft, std::vector<std::string> &vstrImageRight, 
                std::vector<double> &vTimestampsImages, 
                std::vector<std::string> &vstrImageId);

int main(int argc, char **argv)
{

    if(argc != 7)
    {
        cerr << endl << "Usage: ./stereo_4seasons <path_to_vocabulary> <path_to_settings> <path_to_sequence> <sequenceID> <dateAcquisition> <dir_trajectory_result> " << endl;
        return 1;
    }

    // Retrieve paths to images
    vector<string> vstrImageLeft;
    vector<string> vstrImageId;
    vector<string> vstrImageRight;
    vector<double> vTimestamps;

    string strPathToSequence(argv[3]);
    string sequenceID(argv[4]);
    string dateAcquisition(argv[5]);
    string dirTrajResults(string(argv[6]) + "/4seasons/ORB_SLAM2/" + sequenceID + "/" + sequenceID + ".txt");

    cout << "Loading images for sequence " << sequenceID << "..." << std::endl;
    LoadImages(strPathToSequence, sequenceID, dateAcquisition,
               vstrImageLeft, vstrImageRight, vTimestamps, vstrImageId);
    
    if(vstrImageLeft.empty() || vstrImageRight.empty())
    {
        cerr << "ERROR: No images in provided path." << endl;
        return 1;
    }

    if(vstrImageLeft.size()!=vstrImageRight.size())
    {
        cerr << "ERROR: Different number of left and right images." << endl;
        return 1;
    }

    //cout << "end LoadImages" << endl;

    if(vstrImageLeft.empty() || vstrImageRight.empty())
    {
        cerr << "ERROR: No images in provided path." << endl;
        return 1;
    }

    if(vstrImageLeft.size()!=vstrImageRight.size())
    {
        cerr << "ERROR: Different number of left and right images." << endl;
        return 1;
    }

    const int nImages = vstrImageLeft.size();

    // init debug
    EHMK_PARAMS::DebugEHMK MyDebug(dirTrajResults, true, true, false, false);
    MyDebug.setImagesPath(vstrImageLeft, vstrImageRight);

    // Create SLAM system. It initializes all system threads and gets ready to process frames.
    ORB_SLAM2::System SLAM(argv[1],argv[2],ORB_SLAM2::System::STEREO,true);

    // Vector for tracking time statistics
    vector<float> vTimesTrack;
    vTimesTrack.resize(nImages);

    cout << endl << "-------" << endl;
    cout << "Start processing sequence ..." << endl;
    cout << "Images in the sequence: " << nImages << endl << endl;   

    // Main loop
    cv::Mat imLeft, imRight;
    for(int ni=0; ni<nImages; ni++)
    {
        // Read left and right images from file
        imLeft = cv::imread(vstrImageLeft[ni],CV_LOAD_IMAGE_UNCHANGED);
        imRight = cv::imread(vstrImageRight[ni],CV_LOAD_IMAGE_UNCHANGED);
        double tframe = vTimestamps[ni];

        if(imLeft.empty())
        {
            cerr << endl << "Failed to load image at: "
                 << string(vstrImageLeft[ni]) << endl;
            return 1;
        }

#ifdef COMPILEDWITHC11
        std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
#else
        std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
        //std::chrono::monotonic_clock::time_point t1 = std::chrono::monotonic_clock::now();
#endif

        // Pass the images to the SLAM system
        SLAM.TrackStereo(imLeft,imRight,tframe);

#ifdef COMPILEDWITHC11
        std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
#else
        std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
        //std::chrono::monotonic_clock::time_point t2 = std::chrono::monotonic_clock::now();
#endif

        double ttrack= std::chrono::duration_cast<std::chrono::duration<double> >(t2 - t1).count();

        vTimesTrack[ni]=ttrack;

        // Wait to load the next frame
        double T=0;
        if(ni<nImages-1)
            T = vTimestamps[ni+1]-tframe;
        else if(ni>0)
            T = tframe-vTimestamps[ni-1];

        if(ttrack<T)
            usleep((T-ttrack)*1e6);
    }

    // Stop all threads
    SLAM.Shutdown();

    // Tracking time statistics
    sort(vTimesTrack.begin(),vTimesTrack.end());
    float totaltime = 0;
    for(int ni=0; ni<nImages; ni++)
    {
        totaltime+=vTimesTrack[ni];
    }
    cout << "-------" << endl << endl;
    cout << "median tracking time: " << vTimesTrack[nImages/2] << endl;
    cout << "mean tracking time: " << totaltime/nImages << endl;

    // Save camera trajectory
    SLAM.SaveTrajectory4seasons(dirTrajResults, vstrImageId);

    return 0;
}



void LoadImages(const std::string &strPathToSequence, 
                const std::string &sequenceID, const std::string &dateAcquisition,
                std::vector<std::string> &vstrImageLeft, std::vector<std::string> &vstrImageRight, 
                std::vector<double> &vTimestampsImages, 
                std::vector<std::string> &vstrImageId)
{
    std::string tmpIntermediateDir = sequenceID + "/" + "recording_" + dateAcquisition + "_stereo_images_undistorted" + "/recording_" + dateAcquisition;
    std::string strPathTimes = strPathToSequence + "/" + tmpIntermediateDir.c_str() + "/times.txt";
    std::string strDirToImages = strPathToSequence + "/" + tmpIntermediateDir.c_str();
    checkIfFileExist(strPathTimes);
    //checkIfDirExist(strDirToImages);
    std::cout << "strPathTimes for images = " << strPathTimes << std::endl;
    
    std::ifstream fTimes;
    fTimes.open(strPathTimes.c_str());
    while(!fTimes.eof())
    {
        std::string s;
        std::getline(fTimes,s);
        if(!s.empty())
        {
            std::stringstream ss;
            ss << s;
            double time_, exposure_time;
            std::string img_id;
            ss >> img_id >> std::setprecision(10) >> time_ >> std::setprecision(10) >> exposure_time;
            vTimestampsImages.push_back(time_);
            checkIfFileExist(strDirToImages + "/undistorted_images/cam0/" + img_id.c_str() + ".png");
            checkIfFileExist(strDirToImages + "/undistorted_images/cam1/" + img_id.c_str() + ".png");
            vstrImageLeft.push_back(strDirToImages + "/undistorted_images/cam0/" + img_id.c_str() + ".png");
            vstrImageRight.push_back(strDirToImages + "/undistorted_images/cam1/" + img_id.c_str() + ".png");
            vstrImageId.push_back(img_id);
        }
    }
}
