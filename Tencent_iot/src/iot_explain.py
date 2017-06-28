#!/usr/bin/env python
#coding=utf-8
import rospy
from std_msgs.msg import String
import demjson
import time 
import os
import sys
readfifo='/tmp/iot_msg_fifo'
reload(sys)
sys.setdefaultencoding('utf8')

pub = rospy.Publisher('/xfsaywords', String, queue_size=10)
pub_cmd = rospy.Publisher('/control_cmd', String, queue_size=10)
rospy.init_node('iot_explainer')
rate = rospy.Rate(10)
rate.sleep()
def say(mystr):
    rate.sleep()
    pub.publish(mystr)
    print("I will say:%s"%(mystr))
def sendcmd(mycmd):
    rate.sleep()
    pub_cmd.publish(mycmd)
    print("I will say:%s"%(mycmd))

def callback(data):
    s1=""
    openQAState = False
    openQACommend = '' #存储问答得到的命令

    MusicPalyCommed = ''
    MusicAvailable = False
    result=demjson.decode(data)
    print data
    if(result.has_key("text")):
        text=result["text"]
        s1 =text
    say(s1)
def timer():
    rate.sleep()
    read_fd = os.open(readfifo, os.O_RDONLY)  
    while True:  
        in_msg = os.read(read_fd, 1000)  
        print 'Get:>' + in_msg  
        callback(in_msg)
        rate.sleep() 
    os.close(read_fd)  

if __name__ == '__main__':
    try:
        rospy.Subscriber("/iot_json", String, callback)
        say(u"物联网解释初始化成功")
        timer()
    except rospy.ROSInterruptException:
        pass
