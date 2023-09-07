# ORB-SLAM2
# Note: Loop closing & relocalisation are disabled
## maybe should I add re-initialization after lost of tracking ?
**Authors:** [Raul Mur-Artal](http://webdiis.unizar.es/~raulmur/), [Juan D. Tardos](http://webdiis.unizar.es/~jdtardos/), [J. M. M. Montiel](http://webdiis.unizar.es/~josemari/) and [Dorian Galvez-Lopez](http://doriangalvez.com/) ([DBoW2](https://github.com/dorian3d/DBoW2))
##
# compilation
## install matplotlib
```
$ apt-get install -y python-matplotlib python-numpy python2.7-dev
git clone https://github.com/lava/matplotlib-cpp.git
```
```
cd /home/src/ORB_SLAM2/build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j20
```


# 1. License

ORB-SLAM2 is released under a [GPLv3 license](https://github.com/raulmur/ORB_SLAM2/blob/master/License-gpl.txt). For a list of all code/library dependencies (and associated licenses), please see [Dependencies.md](https://github.com/raulmur/ORB_SLAM2/blob/master/Dependencies.md).

For a closed-source version of ORB-SLAM2 for commercial purposes, please contact the authors: orbslam (at) unizar (dot) es.


# 2. Stereo Examples

## KITTI Dataset
```
./stereo_kitti path_to_vocabulary path_to_settings path_to_sequence dir_trajectory_result

/home/src/ORB_SLAM2/Examples/Stereo/stereo_kitti /home/src/ORB_SLAM2/Vocabulary/ORBvoc.txt /home/src/ORB_SLAM2/Examples/Stereo/KITTI00-02.yaml /home/sharedWithHost/VO/Datasets/KITTI/data_odometry_gray/dataset/sequences/01 /home/sharedWithHost/VO/Datasets/resultsOdometry
```

```
/home/src/ORB_SLAM2/Examples/Stereo/stereo_kitti /home/src/ORB_SLAM2/Vocabulary/ORBvoc.txt /home/src/ORB_SLAM2/Examples/Stereo/KITTI00-02.yaml /home/sharedWithHost/VO/Datasets/KITTI/data_odometry_gray/dataset/sequences/02 /home/sharedWithHost/VO/Datasets/resultsOdometry
```

```
/home/src/ORB_SLAM2/Examples/Stereo/stereo_kitti /home/src/ORB_SLAM2/Vocabulary/ORBvoc.txt /home/src/ORB_SLAM2/Examples/Stereo/KITTI03.yaml /home/sharedWithHost/VO/Datasets/KITTI/data_odometry_gray/dataset/sequences/03 /home/sharedWithHost/VO/Datasets/resultsOdometry
```

```
/home/src/ORB_SLAM2/Examples/Stereo/stereo_kitti /home/src/ORB_SLAM2/Vocabulary/ORBvoc.txt /home/src/ORB_SLAM2/Examples/Stereo/KITTI04-12.yaml /home/sharedWithHost/VO/Datasets/KITTI/data_odometry_gray/dataset/sequences/09 /home/sharedWithHost/VO/Datasets/resultsOdometry
```




## KITTI-360 Dataset
```
./stereo_kitti360 path_to_vocabulary path_to_settings path_to_sequence dir_trajectory_result
```
```
/home/src/ORB_SLAM2/Examples/Stereo/stereo_kitti360 /home/src/ORB_SLAM2/Vocabulary/ORBvoc.txt /home/src/ORB_SLAM2/Examples/Stereo/kitti360.yaml /home/sharedWithHost/VO/Datasets/KITTI-360/data_2d_raw/2013_05_28_drive_0000_sync /home/sharedWithHost/VO/Datasets/resultsOdometry
```

```
/home/src/ORB_SLAM2/Examples/Stereo/stereo_kitti360 /home/src/ORB_SLAM2/Vocabulary/ORBvoc.txt /home/src/ORB_SLAM2/Examples/Stereo/kitti360.yaml /home/sharedWithHost/VO/Datasets/KITTI-360/data_2d_raw/2013_05_28_drive_0003_sync /home/sharedWithHost/VO/Datasets/resultsOdometry
```

```
/home/src/ORB_SLAM2/Examples/Stereo/stereo_kitti360 /home/src/ORB_SLAM2/Vocabulary/ORBvoc.txt /home/src/ORB_SLAM2/Examples/Stereo/kitti360.yaml /home/sharedWithHost/VO/Datasets/KITTI-360/data_2d_raw/2013_05_28_drive_0007_sync /home/sharedWithHost/VO/Datasets/resultsOdometry
```




## 4seasons Dataset
```
./stereo_4seasons <path_to_vocabulary> <path_to_settings> <path_to_sequence> <sequenceID> <dateAcquisition> <dir_trajectory_result>
```
```
/home/src/ORB_SLAM2/Examples/Stereo/stereo_4seasons /home/src/ORB_SLAM2/Vocabulary/ORBvoc.txt /home/src/ORB_SLAM2/Examples/Stereo/4seasons.yaml /home/sharedWithHost/VO/Datasets/4seasons/Countryside countryside_1_train 2020-04-07_11-33-45 /home/sharedWithHost/VO/Datasets/resultsOdometry
```

```
/home/src/ORB_SLAM2/Examples/Stereo/stereo_4seasons /home/src/ORB_SLAM2/Vocabulary/ORBvoc.txt /home/src/ORB_SLAM2/Examples/Stereo/4seasons.yaml /home/sharedWithHost/VO/Datasets/4seasons/Countryside countryside_2_train 2020-06-12_11-26-43 /home/sharedWithHost/VO/Datasets/resultsOdometry
```

```
/home/src/ORB_SLAM2/Examples/Stereo/stereo_4seasons /home/src/ORB_SLAM2/Vocabulary/ORBvoc.txt /home/src/ORB_SLAM2/Examples/Stereo/4seasons.yaml /home/sharedWithHost/VO/Datasets/4seasons/CityLoop city_loop_2_train 2021-01-07_14-36-17 /home/sharedWithHost/VO/Datasets/resultsOdometry
```

```
/home/src/ORB_SLAM2/Examples/Stereo/stereo_4seasons /home/src/ORB_SLAM2/Vocabulary/ORBvoc.txt /home/src/ORB_SLAM2/Examples/Stereo/4seasons.yaml /home/sharedWithHost/VO/Datasets/4seasons/OldTown old_town_1_train 2020-10-08_11-53-41 /home/sharedWithHost/VO/Datasets/resultsOdometry
```





## FinnForest Dataset
```
./stereo_finnforest <path_to_vocabulary> <path_to_settings> <path_to_left_folder> <path_to_right_folder> <path_to_times_file> <dir_trajectory_result> <sequenceID>
```
```
xxxxxxxxxxxxx
```
```
/home/src/ORB_SLAM2/Examples/Stereo/stereo_finnforest /home/src/ORB_SLAM2/Vocabulary/ORBvoc.txt /home/src/ORB_SLAM2/Examples/Stereo/finnforest.yaml /home/sharedWithHost/VO/Datasets/FinnForest/S03_8HZ_summer_seq3_shortOdomSeq/S03_8Hz/images_cam2_sr22555667 /home/sharedWithHost/VO/Datasets/FinnForest/S03_8HZ_summer_seq3_shortOdomSeq/S03_8Hz/images_cam3_sr22555660 /home/sharedWithHost/VO/Datasets/FinnForest/S03_8HZ_summer_seq3_shortOdomSeq/S03_8Hz/times_S03.txt /home/sharedWithHost/VO/Datasets/resultsOdometry S03_8HZ_summer_seq3_shortOdomSeq
```


## EuRoC Dataset

1. Download a sequence (ASL format) from http://projects.asl.ethz.ch/datasets/doku.php?id=kmavvisualinertialdatasets

2. Execute the following first command for V1 and V2 sequences, or the second command for MH sequences. Change PATH_TO_SEQUENCE_FOLDER and SEQUENCE according to the sequence you want to run.
```
./Examples/Stereo/stereo_euroc Vocabulary/ORBvoc.txt Examples/Stereo/EuRoC.yaml PATH_TO_SEQUENCE/mav0/cam0/data PATH_TO_SEQUENCE/mav0/cam1/data Examples/Stereo/EuRoC_TimeStamps/SEQUENCE.txt
```
```
./Examples/Stereo/stereo_euroc Vocabulary/ORBvoc.txt Examples/Stereo/EuRoC.yaml PATH_TO_SEQUENCE/cam0/data PATH_TO_SEQUENCE/cam1/data Examples/Stereo/EuRoC_TimeStamps/SEQUENCE.txt
```
```
./Examples/Stereo/stereo_euroc Vocabulary/ORBvoc.txt Examples/Stereo/EuRoC.yaml /home/Datasets/EuRoC/MH_02_easy/mav0/cam0/data /home/Datasets/EuRoC/MH_02_easy/mav0/cam1/data Examples/Stereo/EuRoC_TimeStamps/MH02.txt
```


# 3. Processing your own sequences
You will need to create a settings file with the calibration of your camera. See the settings file provided for the TUM and KITTI datasets for monocular, stereo and RGB-D cameras. We use the calibration model of OpenCV. See the examples to learn how to create a program that makes use of the ORB-SLAM2 library and how to pass images to the SLAM system. Stereo input must be synchronized and rectified. RGB-D input must be synchronized and depth registered.

# 4. SLAM and Localization Modes
You can change between the *SLAM* and *Localization mode* using the GUI of the map viewer.

### SLAM Mode
This is the default mode. The system runs in parallal three threads: Tracking, Local Mapping and Loop Closing. The system localizes the camera, builds new map and tries to close loops.

### Localization Mode
This mode can be used when you have a good map of your working area. In this mode the Local Mapping and Loop Closing are deactivated. The system localizes the camera in the map (which is no longer updated), using relocalization if needed. 

