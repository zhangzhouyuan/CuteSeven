#include"iostream"
#include"ros/ros.h"
#include"std_msgs/String.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#define GETIT  system("echo loadfile /Robot/voice/wav/Begin.wav>/Robot/cmd/Mplayer_cmd")

#define FIFO_NAME "/tmp/my_fifo_wakeup"
#define BUFFER_SIZE 100
#define TEN_MEG (1024 * 100)

int open_mode = O_RDONLY;
int bytes = 0;
char buffer[BUFFER_SIZE + 1];

int Init_fifo(char * path)
{ 
    int pipe_fd;
    int res;
    if (access(path, F_OK) == -1)
    {
        res = mkfifo(path, 0777);
        if (res != 0)
        {
            fprintf(stderr, "Could not create fifo %s\n", path);
            return -1;
        }
    }
    printf("Process %d opening FIFO O_WRONLY\n", getpid());
    pipe_fd = open(path, open_mode);
    printf("Process %d result %d\n", getpid(), pipe_fd);
    return pipe_fd;
}
int read_fifo(char *buff,int fd)
{
    int res;
    if (fd != -1)
    {
            res = read(fd, buff, BUFFER_SIZE);
            if (res == -1)
            {
                fprintf(stderr, "Write error on pipe\n");
            }else if (res >0)
            {
                printf("res is [%d] %s\n",res,buff);
            }
    }
    return res;
}
int main(int argc,char** argv)
{
    int pipe_fd;
    int res;
    int i=0;
    std_msgs::String msg;
    ros::init(argc,argv,"wakeup");
    ros::NodeHandle n;
    ros::Rate loop(10);
    ros::Publisher pub=n.advertise<std_msgs::String>("xfwakeup",1000);
     GETIT;
     pipe_fd=Init_fifo(FIFO_NAME);
     std::cout<<"wakeup node  Init OK"<<std::endl;
     while(ros::ok())
     {
         res=read_fifo(buffer,pipe_fd);
         if(res>0)
         {
                 std::stringstream ss;
                 GETIT;
                 ss <<"";
                 msg.data = ss.str();
                 std::cout<<"get wakesign!"<<i<<">"<<std::endl;
                 pub.publish(msg);
                //dev->writeStr("RESET\n");
         }

         }
         ros::spinOnce();
         loop.sleep();

}
