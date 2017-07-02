#!/bin/bash
run()
{
	gnome-terminal -t "Main Node" -x bash -c "$1"

}
echo "准备启动"
mkfifo /tmp/my_fifo_wakeup
run "roscore"
sleep 3
run "roslaunch xf_voice voice.launch"
sleep 3
run " rosrun car car"
sleep 3
run "rosrun tx_iot iot_explain.py"
sleep 10
run " ~/catkin_ws/src/Tencent_iot/tx_iot_sdk/run.sh"
sleep 3
run " ~/catkin_ws/src/backup/key.sh"
