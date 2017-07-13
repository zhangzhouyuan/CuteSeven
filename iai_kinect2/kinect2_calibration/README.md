# Kinect2 Calibration
Kinect2校准
## Maintainer

- [Thiemo Wiedemeyer](https://ai.uni-bremen.de/team/thiemo_wiedemeyer) <<wiedemeyer@cs.uni-bremen.de>>, [Institute for Artificial Intelligence](http://ai.uni-bremen.de/), University of Bremen

*Note:* ***Please use the GitHub issues*** *for questions and problems regarding the iai_kinect2 package and its components.* ***Do not write emails.***

## Description

This tool uses OpenCV to calibrate two cameras to each other. It is specially designed for the Kinect One. It uses chess or circle boards.
这个工具使用OpenCV对两个相机进行校准。它是专门为Kinect设计的。它使用象棋或圆形板。

## Dependencies

- ROS Hydro/Indigo
- OpenCV

*for the ROS packages look at the package.xml*

## Usage

```
kinect2_calibration [options]
  name: 'any string' equals to the kinect2_bridge topic base name
  mode: 'record' or 'calibrate'
  source: 'color', 'ir', 'sync', 'depth'
  board:
    'circle<WIDTH>x<HEIGHT>x<SIZE>'  for symmetric circle grid
    'acircle<WIDTH>x<HEIGHT>x<SIZE>' for asymmetric circle grid
    'chess<WIDTH>x<HEIGHT>x<SIZE>'   for chessboard pattern
  distortion model: 'rational' for using model with 8 instead of 5 coefficients
  output path: '-path <PATH>'
```

## Key bindings

Windows:
- `ESC`, `q`: Quit
- `SPACE`, `s`: Save the current image for calibration
- `l`: decrease min and max value for IR value rage
- `h`: increase min and max value for IR value rage
- `1`: decrease min value for IR value rage
- `2`: increase min value for IR value rage
- `3`: decrease max value for IR value rage
- `4`: increase max value for IR value rage

Terminal:
- `CRTL`+`c`: Quit

## Calibration patterns

Any chessboard pattern or symmetric or asymmetric circle grid should work. Three different chessboard patterns are located inside the `kinect2_calibration/patterns` folder:
任何棋盘图案或对称或不对称的圆网都应该起作用。三种不同的棋盘模式位于“kinect2_校准/模式”文件夹内:

- [chess5x7x0.03.pdf](patterns/chess5x7x0.03.pdf)
- [chess7x9x0.025.pdf](patterns/chess7x9x0.025.pdf)
- [chess9x11x0.02.pdf](patterns/chess9x11x0.02.pdf)

Other patterns are available at OpenCV:
- [Chessboard pattern](http://docs.opencv.org/2.4.2/_downloads/pattern.png)
- [Asymmetric circle grid](http://docs.opencv.org/2.4.2/_downloads/acircles_pattern.png)

The standard board is a 7x6 0.108m chessboard from the PR2. But any other board can be specified with as parameter. For example a circle board with 8x7 circles in 0.02m distance between them `rosrun kinect2_calibration kinect2_calibration record color circle8x7x0.02`.

Recently, to calibrate our sensors, we have used the chess5x7x0.03 pattern, as it can be printed easily on a good laser printer on A4 paper.


## Calibrating the Kinect One

*Recommended preparation:*
- Print your calibration pattern (for the examples, we used chess5x7x0.03) and glue it to a flat object. It is very important that the calibration pattern is very flat. Also, check with a caliper that the distance between the features of the printed pattern is correct. Sometimes printers scale the document, and the calibration won't work. For the mentioned pattern, the distance between intersections of black and white corners should be 3cm exactly.
- Get two tripods, one for holding the calibration pattern, and another one for holding the kinect2 sensor. Ideally, the tripod for the kinect2 will have a ball head, to allow you to move it easily and lock it in place before you take an image. It is very important that the sensor is stable (and the image is clear and not blurred) before you take an image. The tripod will specially help you to make sure that the sensor has not moved between the moment the IR and the RGB images are taken.
- When recording images for all the steps indicated below (RGB, IR, SYNC), start the recording program, then press spacebar to record each image. The calibration pattern should be detected (indicated by color lines overlayed on the calibration pattern), and the image should be clear and stable. 
- It is recommended to take images that show the calibration pattern in all areas of the image, and with different orientations of the pattern (tilting the pattern relative to the plane of the image), and at least two distances. So you can easily reach 100 images per calibration set.
- We normally start at a short distance, where the calibration pattern covers most of the image, there we take several pictures tilting the calibration pattern vertically, then horizontally. Imagine a ray coming out of the camera sensor, this makes sure that you have images where the calibration pattern is not perpendicular to that ray.
- Then we move the calibration pattern further away, and for different orientations (tilting) of the pattern, we take many images so that we calibration pattern is present around most of the camera image. For example, at first the calibration pattern is on the left upper corner. Then on the next image on the upper middle, then on the upper right corner. Then some images where the calibration pattern is in the middle vertically, etc...
*推荐准备：*
- 打印校准模式（例如，我们使用chess5x7x0.03）并将其粘贴到平面对象上。校准模式非常平坦是非常重要的。另外，用卡尺检查打印图案的特征之间的距离是否正确。有时打印机缩放文档，校准将无法正常工作。对于所提到的图案，黑色和白色角落的交点之间的距离应该是3厘米。
- 获取两个三脚架，一个用于固定校准图案，另一个用于固定kinect2传感器。理想情况下，kinect2的三脚架将有一个球头，让您轻松移动它，并在拍摄图像之前将其锁定到位。在拍摄图像之前，传感器是稳定的（并且图像清晰而不模糊）是非常重要的。三脚架将特别帮助您确保传感器在拍摄IR和RGB图像之间没有移动。
- 对于以下所示的所有步骤（RGB，IR，SYNC）记录图像时，启动录制程序，然后按空格键记录每个图像。应检测校准图案（由校准图案上的色线表示），图像应清晰稳定。
- 建议在图像的所有区域以及图案的不同方向（相对于图像的平面倾斜图案）和至少两个距离拍摄显示校准图案的图像。因此，每个校准集可以轻松达到100张图像。
- 我们通常从很短的距离开始，其中校准图案覆盖了大部分图像，我们拍摄几张照片垂直倾斜校准图案，然后水平放置。想象一下从相机传感器出来的光线，这样可以确保您的图像与校准图案不垂直于该光线。
- 然后我们将校准图案进一步移动，并且对于图案的不同取向（倾斜），我们拍摄许多图像，使得校准图案存在于大多数相机图像中。例如，首先校准模式在左上角。然后在上中间的下一个图像上，然后在右上角。然后一些图像中的校准图案在垂直中间等...

*Typical calibration setup*
![kinect2_calibration_setup_small.jpg](https://ai.uni-bremen.de/_media/kinect2_calibration_setup_small.jpg)

*Detailed steps:*
详细步骤：
0. If you haven't already, start the kinect2_bridge with a low number of frames per second (to make it easy on your CPU): `rosrun kinect2_bridge kinect2_bridge _fps_limit:=2`
1. create a directory for your calibration data files, for example: `mkdir ~/kinect_cal_data; cd ~/kinect_cal_data`
2. Record images for the color camera: `rosrun kinect2_calibration kinect2_calibration chess5x7x0.03 record color` 
3. Calibrate the intrinsics: `rosrun kinect2_calibration kinect2_calibration chess5x7x0.03 calibrate color`
4. Record images for the ir camera: `rosrun kinect2_calibration kinect2_calibration chess5x7x0.03 record ir`
5. Calibrate the intrinsics of the ir camera: `rosrun kinect2_calibration kinect2_calibration chess5x7x0.03 calibrate ir`
6. Record images on both cameras synchronized: `rosrun kinect2_calibration kinect2_calibration chess5x7x0.03 record sync`
7. Calibrate the extrinsics: `rosrun kinect2_calibration kinect2_calibration chess5x7x0.03 calibrate sync`
8. Calibrate the depth measurements: `rosrun kinect2_calibration kinect2_calibration chess5x7x0.03 calibrate depth`
9. Find out the serial number of your kinect2 by looking at the first lines printed out by the kinect2_bridge. The line looks like this: 
  `device serial: 012526541941`
10. Create the calibration results directory in kinect2_bridge/data/$serial: `roscd kinect2_bridge/data; mkdir 012526541941`
11. Copy the following files from your calibration directory (~/kinect_cal_data) into the directory you just created: `calib_color.yaml  calib_depth.yaml  calib_ir.yaml  calib_pose.yaml`
12. Restart the kinect2_bridge and be amazed at the better data.
 
如果还没有，请启动每秒钟帧数较少的kinect2_bridge（以使CPU容易）：`rosrun kinect2_bridge kinect2_bridge _fps_limit：= 2`
1.为您的校准数据文件创建一个目录，例如：`mkdir〜/ kinect_cal_data; cd〜/ kinect_cal_data`
2.记录彩色摄像机的图像：`rosrun kinect2_calibration kinect2_calibration chess5x7x0.03 record color`
3.校准内在函数：“rosrun kinect2_calibration kinect2_calibration chess5x7x0.03校准颜色”
4.记录ir相机的图像：`rosrun kinect2_calibration kinect2_calibration chess5x7x0.03 record ir`
5.校准ir相机的内在功能：`rosrun kinect2_calibration kinect2_calibration chess5x7x0.03 calibration ir
记录两台摄像机上的图像同步：`rosrun kinect2_calibration kinect2_calibration chess5x7x0.03 record sync
7.校准外来物质：“rosrun kinect2_calibration kinect2_calibration chess5x7x0.03校准同步”
8.校准深度测量：“rosrun kinect2_calibration kinect2_calibration chess5x7x0.03校准深度”
9.通过查看kinect2_bridge打印出的第一行找出您的kinect2的序列号。线条如下所示：
  `device serial：012526541941`
在kinect2_bridge / data / $ serial中创建校准结果目录：`roscd kinect2_bridge / data; mkdir 012526541941`
11.将以下文件从校准目录（〜/ kinect_cal_data）复制到刚创建的目录中：`calib_color.yaml calib_depth.yaml calib_ir.yaml calib_pose.yaml`
12.重新启动kinect2_bridge，并对更好的数据感到惊讶。


## Calibration of the depth measurements

I did some tests on the measured and the computed distance based on the detected chess board. It seems like the Kinect2 (or at least the Kinect2s I am using) has a static offset of around 24 mm. As shown in the following images, one can see, that the difference between measured and computed distance is unrelated to the x and y coordinates of the pixel and also unrelated to the distance.
我根据检测到的棋盘对测量和计算距离进行了一些测试。 看起来Kinect2（或至少我使用的Kinect2）具有大约24毫米的静态偏移。 如以下图像所示，可以看出，测量距离和计算距离之间的差异与像素的x和y坐标无关，也与距离无关。
![plot.png](http://ai.uni-bremen.de/wiki/_media/software/plot.png)
![plot_x.png](http://ai.uni-bremen.de/wiki/_media/software/plot_x.png)
![plot_y.png](http://ai.uni-bremen.de/wiki/_media/software/plot_y.png)
![plot_xy.png](http://ai.uni-bremen.de/wiki/_media/software/plot_xy.png)

For the images above ~400 images of a 4x5x0.03 chessboard in different orientations, distances and image positions were used. The code for computing the depth offset is added to the calibration tool.
对于不同方向的4x5x0.03棋盘的400张以上的图像，使用距离和图像位置。 用于计算深度偏移的代码被添加到校准工具中。
### GNUPlot

The depth calibration creates a file named `plot.dat` inside the calibration folder. This files contains the results of the calibration in 5 columns: x, y, computed depth, measured depth, difference between computed and measured depth.
深度校准在校准文件夹中创建一个名为“plot.dat”的文件。 该文件包含5列校准的结果：x，y，计算深度，测量深度，计算和测量深度之间的差异。

- Difference between measured/computed distance
测量/计算距离之间的差异
  ```
set xlabel "Measured distance"
set ylabel "Computed distance"
plot 'plot.dat' using 3:4 with dots title "Difference between measured/computed distance"
```

- Difference relative to x coordinate
- 相对于x坐标的差异
  ```
set xlabel "X"
set ylabel "Distance difference"
plot 'plot.dat' using 1:5 with dots title "Difference relative to X-coordinate"
```

- Difference relative to y coordinate

  ```
set xlabel "Y"
set ylabel "Distance difference"
plot 'plot.dat' using 2:5 with dots title "Difference relative to Y-coordinate"
```

- Difference relative to XY-coordinate
- 相对于XY坐标的差异
  ```
set xlabel "X"
set ylabel "Y"
set zlabel "Distance difference"
splot 'plot.dat' using 1:2:5 with dots palette title "Difference relative to XY-coordinate"
```

## Example results

Example calibration results can be found in the directory [kinect2_bridge/data/](../kinect2_bridge/data).
示例校准结果可以在[kinect2_bridge / data /](../ kinect2_bridge / data）目录中找到。
The following images were made before and after the calibration, using the kinect2 viewer.
 - For the superimposed images: `rosrun kinect2_viewer kinect2_viewer hd image`
 - For the point cloud images: `rosrun kinect2_viewer kinect2_viewer hd cloud`
 - For the superimposed and point cloud images: `rosrun kinect2_viewer kinect2_viewer hd both`
在校准之前和之后使用kinect2观察器进行以下图像。
  - 对于叠加的图像：`rosrun kinect2_viewer kinect2_viewer hd image`
  - 对于点云图像：`rosrun kinect2_viewer kinect2_viewer hd cloud`
  - 对于叠加和点云图像：`rosrun kinect2_viewer kinect2_viewer hd both`

raw here stands for raw data transmission aka uncompressed.
原始的这里代表未压缩的原始数据传输。
Uncalibrated rectified images (depth and RGB superimposed): 
![kinect2_cloud_calib](https://ai.uni-bremen.de/_media/kinect2_raw_nocalib.png)

Calibrated rectified images (depth and RGB superimposed):
![kinect2_cloud_calib](https://ai.uni-bremen.de/_media/kinect2_raw_calib.png)

Uncalibrated depth cloud:
![kinect2_cloud_calib](https://ai.uni-bremen.de/_media/kinect2_cloud_nocalib.png)

Calibrated depth cloud:
![kinect2_cloud_calib](https://ai.uni-bremen.de/_media/kinect2_cloud_calib.png)

Note how the color is now correctly applied on the depth data. This is specially evident around strong edges, like the edge of the white column on the left.
请注意颜色如何正确应用于深度数据。 这是围绕强边缘特别明显的，如左侧白柱的边缘。
