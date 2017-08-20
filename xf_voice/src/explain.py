#!/usr/bin/env python
#coding=utf-8
import rospy
from std_msgs.msg import String
import demjson
import time 
import os
import sys
reload(sys)
sys.setdefaultencoding('utf8')

pub = rospy.Publisher('xfsaywords', String, queue_size=10)
pub_cmd = rospy.Publisher('/control_cmd', String, queue_size=10)
rospy.init_node('explainer')
rate = rospy.Rate(10)
rate.sleep()
def say(mystr,ask):
    rate.sleep()
    pub.publish(mystr)
    print("I will say:%s"%(mystr))
    cmd=u"~/catkin_ws/src/GUI/hello -t 60000 -a \'" + ask + u"\' -s \'" + mystr +u"\' &"
    os.system(cmd);
    print(cmd)
def sendcmd(mycmd):
    rate.sleep()
    pub_cmd.publish(mycmd)
    print("I will say:%s"%(mycmd))
def callback(data):
    s1=u" "
    s2=u" "
    openQAState = False
    openQACommend = '' #存储问答得到的命令

    MusicPalyCommed = ''
    MusicAvailable = False
    result=demjson.decode(data.data)
    print data.data
    if(result.has_key("text")):
        s2=result["text"]
    if(result.has_key("service")):
        service=result["service"]
        if(service=="chat"):
            s1 = result["answer"]["text"]
        elif(service=="music"):
            s1+=u"接下来播放"+\
                    result["data"]["result"][0]["singer"]+\
                    u"的"+\
                    result["data"]["result"][0]["name"]
            MusicPlayCommend = 'echo loadfile '+\
                    result["data"]["result"][0]["downloadUrl"]+\
                    '>/tmp/Mplayer_cmd'
           # print MusicPlayCommend
            MusicAvailable = True
        elif(service=="openQA"):
            for letter in result["answer"]["text"]:
                if(openQAState == True):
                    openQACommend = letter
                    openQAState = False
                if(letter == '<'):
                    openQAState = True
            for letter in result["answer"]["text"]:
                if(letter=='<'):
                    break
                s1 += letter
            print openQACommend
            sendcmd(openQACommend)
        elif(service=="datetime" or service=="calc" or service=="baike" or service== "faq"):
            s1 = result["answer"]["text"]
        elif(service=="schedule"):
            s1 = u"正在创建提醒事项... ...好的,我会提醒你"
                   # result["semantic"]["slots"]["datatime"]["dateOrig"]+\
                   # result["semantic"]["content"]["datatime"]["timeOrig"]
    else:
        s1+=u"对不起，没听明白"
    say(s1,s2)
    if(MusicAvailable):
        time.sleep(1)
        os.system(MusicPlayCommend)
        MusicAvailable = False

if __name__ == '__main__':
    try:
        rospy.Subscriber("xfunderstand", String, callback)
        say(u"语音解释初始化成功",u"startup")
        rospy.spin()
    except rospy.ROSInterruptException:
        pass
