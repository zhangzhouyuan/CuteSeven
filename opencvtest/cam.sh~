#!/bin/bash
run()
{
	gnome-terminal -t "Main Node" -x bash -c "$1"

}
echo "准备启动"
run "roscore"
sleep 1
run "roslaunch usb_cam usb_cam-test.launch"
sleep 1
run "rosrun opencvtest opencv_testcam "
