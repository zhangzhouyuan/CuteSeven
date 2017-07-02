#include"iostream"
#include"ros/ros.h"
#include"std_msgs/String.h"
#include "pthread.h"
#include<stdio.h>      /*标准输入输出定义*/
#include<stdlib.h>     /*标准函数库定义*/
#include<unistd.h>     /*Unix 标准函数定义*/
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>      /*文件控制定义*/
#include<termios.h>    /*PPSIX 终端控制定义*/
#include<errno.h>      /*错误号定义*/
#include<string.h>

#define GETIT  system("echo loadfile /Robot/voice/wav/Begin.wav>/tmp/Mplayer_cmd")

#define READ_FIFO "cat /tmp/my_fifo_wakeup"
#define MAKE_FIFO "makefifo /tmp/my_fifo_wakeup"

int main(int argc,char** argv)
{
    int pipe_fd;
    int res;
    int i=0;
    FILE *pp;
    std::string wakeup_cmd;
    std_msgs::String msg;
    ros::init(argc,argv,"wakeup");
    ros::NodeHandle n;
    ros::Rate loop(10);
    ros::Publisher pub=n.advertise<std_msgs::String>("/xfwakeup",1000);
     GETIT;
     system(MAKE_FIFO);
     std::cout<<"wakeup node  Init OK"<<std::endl;
     while(ros::ok())
     {
         //res=read_fifo(buffer,pipe_fd);
         pp= popen(READ_FIFO, "r");
         res=0;
         if (pp != NULL)
         {
             char tmp[1024] = {0};
             if(fgets(tmp, sizeof(tmp), pp) != NULL)
             {
                res=1;
                wakeup_cmd=tmp;
             }
          }
         if(res>0)
         {
                 std::stringstream ss;
                 GETIT;
                 ss <<"";
                 msg.data = ss.str();
                 std::cout<<"get wakesign!"<<wakeup_cmd<<">"<<std::endl;
                 pub.publish(msg);
                //dev->writeStr("RESET\n");
         }

         }
         ros::spinOnce();
         loop.sleep();

}
