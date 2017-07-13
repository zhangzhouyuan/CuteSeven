# Kinect2 Registration

## Maintainer

- [Thiemo Wiedemeyer](https://ai.uni-bremen.de/team/thiemo_wiedemeyer) <<wiedemeyer@cs.uni-bremen.de>>, [Institute for Artificial Intelligence](http://ai.uni-bremen.de/), University of Bremen

*Note:* ***Please use the GitHub issues*** *for questions and problems regarding the iai_kinect2 package and its components.* ***Do not write emails.***

## Description

This is a library for projecting the depth image obtained by Kinect like sensors to a color image. It has a OpenCL implementation for registering the depth image, to reduce CPU load.
这是用于将由Kinect获得的深度图像投影到彩色图像的库。 它具有用于注册深度图像的OpenCL实现，以减少CPU负载。
## Dependencies

- ROS Hydro/Indigo
- OpenCV
- Eigen (optional, but recommended)
- OpenCL (optional, but recommended)

At least one of OpenCL or Eigen has to be installed. If OpenCL is not installed the CPU will be used. For optimal performance OpenCL is recommended.
必须至少安装一个OpenCL或Eigen。 如果未安装OpenCL，将使用CPU。 为了获得最佳性能，建议使用OpenCL。

*for the ROS packages look at the package.xml*

