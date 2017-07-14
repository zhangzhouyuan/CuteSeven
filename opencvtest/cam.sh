#!/bin/bash
run()
{
	gnome-terminal -t "Main Node" -x bash -c "$1"

}
echo "准备启动"
run "roscore"
sleep 3
#run "roslaunch usb_cam usb_cam-test.launch"
run "rosrun usb_cam usb_cam_node"
sleep 3
run "rosrun opencvtest opencv_testcam "
