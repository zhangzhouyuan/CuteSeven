#!/bin/bash
run()
{
	guake -n "cute" -e  "$1" 
}
echo "准备启动"
cd /home/robot7/catkin_ws/src/
mkfifo /tmp/my_fifo_wakeup
mkfifo /tmp/iot_msg_fifo
guake  -e "roscore" & 
sleep 3
run "roslaunch xf_voice voice.launch"
sleep 3
run "rosrun car car"
sleep 3
#run "rosrun tx_iot iot_explain.py"
#`sleep 10
#run "./Tencent_iot/tx_iot_sdk/run.sh"
#sleep 3
run "./backup/key.sh"
