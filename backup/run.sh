roscore&
sleep 5s
rosrun  xf_voice xf_tts &
sleep 2s 
rosrun xf_voice explain.py &
sleep 2s
rosrun xf_voice xf_asr &
rosrun xf_voice wakeup & 
sleep 2s 
rosrun tx_iot iot_explain.py & 
sleep 2s 
./catkin_ws/src/Tencent_iot/tx_iot_sdk/run.sh &

