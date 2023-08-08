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

using namespace std;

void LoadImages(const string &strPathToSequence,
                vector<string> &vstrImageLeft, vector<string> &vstrImageRight, vector<double> &vTimestamps);


bool isFileExist(const std::string &filePath)
{
    std::ifstream file(filePath);
    return file.good();
}


int main(int argc, char **argv)
{
    if(argc != 5)
    {
        cerr << endl << "Usage: ./stereo_kitti_360 path_to_vocabulary path_to_settings path_to_sequence_folder path_to_trajectory_result" << endl;
        return 1;
    }

    // Retrieve paths to images
    vector<string> vstrImageLeft;
    vector<string> vstrImageRight;
    vector<double> vTimestamps;
    LoadImages(string(argv[3]), vstrImageLeft, vstrImageRight, vTimestamps);

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
        if(imRight.empty())
        {
            cerr << endl << "Failed to load image at: "
                 << string(vstrImageRight[ni]) << endl;
            return 1;
        }

#ifdef COMPILEDWITHC11
        std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
#else
        std::chrono::monotonic_clock::time_point t1 = std::chrono::monotonic_clock::now();
#endif

        // Pass the images to the SLAM system
        SLAM.TrackStereo(imLeft,imRight,tframe);

#ifdef COMPILEDWITHC11
        std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
#else
        std::chrono::monotonic_clock::time_point t2 = std::chrono::monotonic_clock::now();
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
    SLAM.SaveTrajectoryKITTI360(string(argv[4]));  //"CameraTrajectoryKITTI360.txt");

    return 0;
}


void LoadImages(const string &strPathToSequence,
                vector<string> &vstrImageLeft, vector<string> &vstrImageRight, vector<double> &vTimestamps)
{
    string strPathTimes = strPathToSequence + "/image_00/timestamps.txt";
    ifstream fTimes;
    fTimes.open(strPathTimes.c_str());
    bool isFirstTime = true;
    double timeDoubleBase = 0.0;
    double t_decimal_value_base = 0.0;
    while(!fTimes.eof())
    {
        string line, time_wo_ms, decimal_part;

        while (getline(fTimes, line))
        {
            size_t pos = line.find('.');
            if (pos != string::npos)
            {
                time_wo_ms = line.substr(0, pos);
                decimal_part = line.substr(pos);
            }

            std::istringstream ss(time_wo_ms);
            int year, month, day, hour, minute, second;
            char discard; // To discard '-' and ':' characters
            ss >> year >> discard >> month >> discard >> day >> hour >> discard >> minute >> discard >> second;
            if (ss.fail())
            {
                std::cerr << "Erreur lors de la conversion en temps pour la ligne : " << line << std::endl;
                continue;
            }
            // Création de la structure tm
            std::tm timeStruct = {0};
            timeStruct.tm_year = year - 1900;
            timeStruct.tm_mon = month - 1;
            timeStruct.tm_mday = day;
            timeStruct.tm_hour = hour;
            timeStruct.tm_min = minute;
            timeStruct.tm_sec = second;
            std::time_t timeValue = std::mktime(&timeStruct);

            double timeDouble = static_cast<double>(timeValue);
            double t_decimal_value = std::stod(decimal_part);
            //std::cout << "Temps en double (partie entiere) : " << timeDouble << std::endl;
            //std::cout << "Temps en double (decimal part): " << t_decimal_value << std::endl;
            if (isFirstTime)
            {
                timeDoubleBase = timeDouble;
                t_decimal_value_base = t_decimal_value;
                isFirstTime = false;
            }
            double t = (timeDouble - timeDoubleBase) + (t_decimal_value - t_decimal_value_base);
            //cout << "final time en double : " << t << endl;
            //cout << "original time 1: " << s << "\t Temps en double : " << t << "\t Temps en double (en ns): " << t/1e9 << endl;
            vTimestamps.push_back(t);
        }
    }

    const int nTimes = vTimestamps.size();
    vstrImageLeft.resize(nTimes);
    vstrImageRight.resize(nTimes);

    for(int i=0; i<nTimes; i++)
    {
        stringstream ss;
        ss << setfill('0') << setw(10) << i;
        vstrImageLeft[i] = strPathToSequence + "/image_00/data_rect/" + ss.str() + ".png";
        vstrImageRight[i] = strPathToSequence + "/image_01/data_rect/" + ss.str() + ".png";
        if (!isFileExist(vstrImageLeft[i]))
        {
            std::cout << "WARNING: file " << vstrImageLeft[i] << " does not exist" << std::endl;
        }
        if (!isFileExist(vstrImageRight[i]))
        {
            std::cout << "WARNING: file " << vstrImageRight[i] << " does not exist" << std::endl;
        }
    }

}
