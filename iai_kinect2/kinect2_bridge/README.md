# Kinect2 Bridge

## Maintainer

- [Thiemo Wiedemeyer](https://ai.uni-bremen.de/team/thiemo_wiedemeyer) <<wiedemeyer@cs.uni-bremen.de>>, [Institute for Artificial Intelligence](http://ai.uni-bremen.de/), University of Bremen

*Note:* ***Please use the GitHub issues*** *for questions and problems regarding the iai_kinect2 package and its components.* ***Do not write emails.***

## Description

This is a bridge between [libfreenect2](https://github.com/OpenKinect/libfreenect2) and ROS.
这是[libfreenect2]（https://github.com/OpenKinect/libfreenect2）和ROS之间的桥梁。

### Highlights

- delivers up to 30 frames per second on non high end hardware
- delivers up to 30 frames per second over gigabit ethernet
- support for compressed image transport
- utilizes multiple cores and uses special OpenCL based implementation of the depth registration
- 在非高端硬件上每秒可提供30帧
- 通过千兆以太网提供高达30帧/秒的速度
- 支持压缩图像传输
- 利用多个核心并使用特殊的基于OpenCL的深度注册实现

## Dependencies

- ROS Hydro/Indigo
- OpenCV
- [libfreenect2](https://github.com/OpenKinect/libfreenect2)

*for the ROS packages look at the package.xml*

## First steps

For the depth registration the camera intrinsic and extrinsic parameters need to be known. The program reads in the values from the `data/<serialnumber>` folder. For each new sensor you need to add a sub-folder with the serial number of the device as the folder name. In this folder you need to provide 4 yaml files with the intrinsic and extrinsic parameters. These files can be created by the `kinect2_calibration` tool (or you can copy the files provided in one of the other folders, but results can be sub optimal). The device serial number is shown when `kinect2_bridge` or `Protonect` from libfreenect2 is started, it also appears in `dmesg` when you connect the sensor. [More information on calibration](../kinect2_calibration#calibrating-the-kinect-one).

为了深度配准，需要知道相机内在参数和外在参数。 程序读入`data / <serialnumber>`文件夹中的值。 对于每个新传感器，您需要添加具有设备序列号的子文件夹作为文件夹名称。 在这个文件夹中，您需要提供4个yaml文件与内在参数和外部参数。 这些文件可以由`kinect2_calibration`工具创建（或者您可以复制其他文件夹中提供的文件，但是结果可能是次优）。

当从libfreenect2中启动“kinect2_bridge”或“Protonect”时，就会显示设备序列号，当连接传感器时，它也会出现在“dmesg”中。(关于校准的更多信息)(. . / kinect2_calibration # calibrating-the-kinect-one)。


When `kinect2_bridge` is running you can use the `kinect2_viewer` to display the images or point cloud: `rosrun kinect2_viewer kinect2_viewer sd image` or `rosrun kinect2_viewer kinect2_viewer sd cloud`.
当“kinect2_bridge”运行时，你可以使用“kinect2_viewer”来显示图像或点云:“rosrun kinect2_viewer kinect2_viewer图像”或“rosrun kinect2_viewer kinect2_viewer”。
## Topics

### HD Topics

The images in this topics have a FullHD resolution (1920x1080).
这个主题的图像有一个FullHD分辨率(1920x1080)。
*Note: For correct registration of the depth image to the color image it is needed to perform a calibration.*
*注意:对图像的深度图像进行正确的注册，需要进行校准

```
/kinect2/hd/camera_info
/kinect2/hd/image_color
/kinect2/hd/image_color/compressed
/kinect2/hd/image_color_rect
/kinect2/hd/image_color_rect/compressed
/kinect2/hd/image_depth_rect
/kinect2/hd/image_depth_rect/compressed
/kinect2/hd/image_mono
/kinect2/hd/image_mono/compressed
/kinect2/hd/image_mono_rect
/kinect2/hd/image_mono_rect/compressed
/kinect2/hd/points
```

### Quater HD Topics

The images in this topics have a quarter FullHD resolution (960x540).
本主题的图像有四分之一全高清分辨率(960x540)。
*Note: For correct registration of the depth image to the color image it is needed to perform a calibration.*
*注意:对图像的深度图像进行正确的注册，需要进行校准
```
/kinect2/qhd/camera_info
/kinect2/qhd/image_color
/kinect2/qhd/image_color/compressed
/kinect2/qhd/image_color_rect
/kinect2/qhd/image_color_rect/compressed
/kinect2/qhd/image_depth_rect
/kinect2/qhd/image_depth_rect/compressed
/kinect2/qhd/image_mono
/kinect2/qhd/image_mono/compressed
/kinect2/qhd/image_mono_rect
/kinect2/qhd/image_mono_rect/compressed
/kinect2/qhd/points
```

### IR/Depth Topics

This are the raw IR and depth images from the sensor (512x424).
他是来自传感器的原始IR和深度图像(512x424)。
*Note: The registration of the color image is available without a calibration. Parameters for the registration are provided by the sensor itself.*
*注:不经校准就可获得彩色图像的注册。注册的参数由传感器本身提供。*
```
/kinect2/sd/camera_info
/kinect2/sd/image_color_rect
/kinect2/sd/image_color_rect/compressed
/kinect2/sd/image_depth
/kinect2/sd/image_depth/compressed
/kinect2/sd/image_depth_rect
/kinect2/sd/image_depth_rect/compressed
/kinect2/sd/image_ir
/kinect2/sd/image_ir/compressed
/kinect2/sd/image_ir_rect
/kinect2/sd/image_ir_rect/compressed
/kinect2/sd/points
```

## Notes

- Point clouds are only published when the launch file is used. Run `roslaunch kinect2_bridge kinect2_bridge.launch`.
- Images from the same frame have the same timestamp. Using the `message_filters::sync_policies::ExactTime` policy is recommended.


点云只有在使用启动文件时才会发布。运行“roslaunch kinect2_bridge kinect2_bridge.launch”。
-来自同一框架的图像具有相同的时间戳。使用“message_filter::sync_policy::精确时间”策略被推荐。

## Usage

```
roslaunch kinect2_bridge kinect2_bridge.launch [options:=value]
base_name:=<string>
    default: kinect2
    info:    set base name for all topics
sensor:=<string>
    default:
    info:    serial of the sensor to use
fps_limit:=<double>
    default: -1.0
    info:    limit the frames per second
calib_path:=<string>
    default: /home/wiedemeyer/work/src/iai_kinect2/kinect2_bridge/data/
    info:    path to the calibration files
use_png:=<bool>
    default: false
    info:    Use PNG compression instead of TIFF
jpeg_quality:=<int>
    default: 90
    info:    JPEG quality level from 0 to 100
png_level:=<int>
    default: 1
    info:    PNG compression level from 0 to 9
depth_method:=<string>
    default: cuda
    info:    Use specific depth processing: default, cpu, opengl, opencl, cuda, clkde, cudakde
depth_device:=<int>
    default: -1
    info:    openCL device to use for depth processing
reg_method:=<string>
    default: opencl
    info:    Use specific depth registration: default, cpu, opencl
reg_device:=<int>
    default: -1
    info:    openCL device to use for depth registration
max_depth:=<double>
    default: 12.0
    info:    max depth value
min_depth:=<double>
    default: 0.1
    info:    min depth value
queue_size:=<int>
    default: 2
    info:    queue size of publisher
bilateral_filter:=<bool>
    default: true
    info:    enable bilateral filtering of depth images
edge_aware_filter:=<bool>
    default: true
    info:    enable edge aware filtering of depth images
publish_tf:=<bool>
    default: false
    info:    publish static tf transforms for camera
base_name_tf:=<string>
    default: as base_name
    info:    base name for the tf frames
worker_threads:=<int>
    default: 4
    info:    number of threads used for processing the images
```

## Key bindings

Terminal:
- `CRTL`+`c`: Quit

