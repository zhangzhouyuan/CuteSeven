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
  try:
    result=demjson.decode(data)
  except:
    print u"json解码发生错误"
    result= {'Null': 0}
  print data
  if(result.has_key("text")):
    text=result["text"]
    s1 =text
  elif(result.has_key("type")):#if the type is file transfer
    msg_type=result["type"]
    if(msg_type=="file"):
      if(result["errcode"]==0):
        if(result["bussiness_name"]=="AudioMsg"):
          s1=u"收到一条语音"
          MusicPlayCommend = 'echo loadfile '+result["file_path"]+'>/Robot/cmd/Mplayer_cmd'
          MusicAvailable = True
        elif(result["bussiness_name"]=="ImgMsg"):
          s1=u"收到一张图片"
        elif(result["bussiness_name"]=="VideoMsg"):
          s1=u"收到一段视频"
    elif(msg_type=="control"):
      if(result["sub_type"]=="rotate"):
        if(result["direction"]==1):
          s1=u"向左"
        elif(result["direction"]==2):
          s1=u"向右"
        elif(result["direction"]==3):
          s1=u"向前"
        elif(result["direction"]==4):
          s1=u"向后"
      elif(result["sub_type"]=="zoom"):
        if(result["direction"]==1):
          s1=u"启动"
        elif(result["direction"]==-1):
          s1=u"停止"
      
        
  say(s1)
  if(MusicAvailable):
    time.sleep(5)
    os.system(MusicPlayCommend)
    MusicAvailable = False

def timer():
  rate.sleep()
  os.system("mkfifo /tmp/iot_msg_fifo")
  read_fd = os.open(readfifo, os.O_RDONLY)  
  while True:  
    in_msg = os.read(read_fd, 1000)  
    if len(in_msg)>0:
     print 'Get:>' + in_msg  
     callback(in_msg)
    rate.sleep() 

if __name__ == '__main__':
  try:
    rospy.Subscriber("/iot_json", String, callback)
    say(u"物联网解释初始化成功")
    timer()
  except rospy.ROSInterruptException:
    os.close(read_fd)  
    pass
