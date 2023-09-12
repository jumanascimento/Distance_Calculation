# Distance_Calculation

:bangbang: **IMPORTANTE**:
This repository uses the [Aruco-markers](https://github.com/fdcl-gwu/aruco-markers) as a base. The packages necessary for the application to function were transferred to this repository together with the modified package with the distance calculation.

This repository includes the camera calibration code, the pose estimation code which contains the distance calculation technique, also contains the report of the experiments carried out.

For the application to function fully, it is necessary to use OpenCV4.

## Installing OpenCV

You can install the standalone ArUco library by downloading the source files which can be found in the above website and building and installing them.
But it is highly recommended to install ArUco library packed in OpenCV library.
The instruction below are for installing OpenCV with ArUco library.

You can install OpenCV using the master branch of their repository, **OR** using the submodules added to this repository.
Building and installing OpenCV with the provided submodules guarantees that the other codes on this repository work without issues.
So it is recommended to install from the submodules.


### Installing v4.5.3 (recommended)
```
sudo apt-get install build-essential
sudo apt-get install cmake git libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev
sudo apt-get install python-dev python-numpy libtbb2 libtbb-dev libjpeg-dev libpng-dev libtiff-dev libdc1394-22-dev

git submodule update --init
cd libraries/opencv
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local -DOPENCV_EXTRA_MODULES_PATH=../../opencv_contrib/modules ..
make -j4  # if you have more/less cores on your computer, substitute 4 with the number of cores
          # use command "nproc" to find the number of cores
sudo make install
```
## Mission
The main objective of the algorithm present in this repository is to make it possible to measure the distance from an ArUco marker to the observer, using only a single camera.
## Who to use

The first step is tp clone the repository, after that the camera must be calibrated and the parameters must be changed in the code (main.cpp of pose_estimation), than you can run the pos_estimation to calculate the distance.

### Camera calibration

To accurately detect markers or to get accurate pose data, a camera calibration needs to be performed.

Run below commands to perform the camera calibration:
```
cd camera_calibration
mkdir build && cd build
cmake ../
make

# Below command is accurate only if you used the same parameters when you generated the markers.
# If you changed any of them, change below arguments accordingly.
./camera_calibration -d=16 -dp=../detector_params.yml -h=2 -w=4 -l=<side length of a single marker (in meters)> -s=<separation between two consecutive markers in the grid (in meters)> ../../calibration_params.yml

# If you want to calibrate with an already saved video, use `-v` flag.
./camera_calibration -v=/path/to/your/video.avi -d=16 -dp=../detector_params.yml -h=2 -w=4 -l=<side length of a single marker (in meters)> -s=<separation between two consecutive markers in the grid (in meters)> ../../calibration_params.yml
```

Then points the camera at the marker at different orientations and at different angles, and save those images by pressing key `C`. 
These instructions should appear on the screen.
Around 30 images should be good enough.

### Pose_estimation
To calculate the distance os an ArUco to the camera, run below code:
```
cd pose_estimation

mkdir build && cd build
cmake ../
make

./pose_estimation -l=<side length of a single marker (in meters)>

# or, if you are trying this on an already saved video
./pose_estimation -l=<side length of a single marker (in meters)> -v=<path to the video>
```


The experiment report clearly shows the results obtained when using this repository to calculate the distance from an ArUco marker to a camera.