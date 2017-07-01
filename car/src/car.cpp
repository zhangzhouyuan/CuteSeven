#include <iostream>
#include "pthread.h"
#include"balance/Num.h"
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

int flag_auto=1;
using namespace std;
class my_uart
{
public:
    bool Is_connect;
    bool Is_PrintDetail;
    int Uart_fd;
    int Uart_data_count;
    int Uart_data_count_cur;
    float RecievedFloatArray[1024];

    my_uart()
    {
        int i;
        Is_connect=0;
        sum=0;
        index=0;
        RecievedState = 0;
        ChannalNum=0;
        p = ( float *)RecievedByteArray;
        q = RecievedFloatArray;
        Is_PrintDetail=0;
        for(i=0;i<255;i++)
        {
            RecievedFloatArray[i]=0;
        }
    }
    ~my_uart()
    {
        if(Is_connect)
        {
            Is_connect=false;
            Is_PrintDetail=false;
            close(Uart_fd);
        }
    }
    void Init(const char *path, int rate)
    {
        int iResult = -1;
        int iBaudRate,iDataSize,iStopBit;
        int err=0;
        char cParity;
        Uart_data_count=0;
        Uart_data_count_cur=0;

        Uart_fd=-1;
        Uart_fd = open_port(path);
        if( Uart_fd<0 )
        {
            printf("open_port error !");
            err=1;
        }

        iBaudRate = rate;
        iDataSize = 8;
        cParity = 'N';
        iStopBit = 1;
        iResult = set_port(Uart_fd,iBaudRate,iDataSize,cParity,iStopBit);
        if( iResult<0 )
        {
            printf("set_port error !");
            err=2;
        }
        if(err==0)
        {
            Is_connect=1;
        }else
        {
            Is_connect=0;
        }
    }
    void SendStr(const char *str)
    {
        if(Is_connect)write_port(str,strlen(str));
        else std::cout<<"you haven't conect!"<<std::endl;
    }
    void Sendbyte(unsigned char byte)
    {
        char *data=new char;
        *data=byte;
        if(Is_connect)write_port(data,1);
        else std::cout<<"you haven't conect!"<<std::endl;
    }
    void SendCommend(unsigned char Commend)
    {
        Sendbyte('<');
        Sendbyte(Commend);
        Sendbyte('>');
    }
    void SendFloat(unsigned char Addr, float FloatData)
    {
        unsigned char i = 0;
        unsigned char *p = ( unsigned char *)&FloatData;
        unsigned char sum = 0;
        Sendbyte('{');
        Sendbyte(Addr);
        for(i=0;i<4;i++)
       {
            sum = sum+*p;
            Sendbyte(*p++);
        }
        sum  = sum + '{' +Addr;
        Sendbyte(sum);
        Sendbyte('}');
    }

    unsigned char RecievetoFloatArray(unsigned char Byte)
    {
        unsigned char i;
        bool NotTakeData= true ;
        if(RecievedState ==0 && NotTakeData)
        {
            NotTakeData = false;
            if(Byte=='$')
            {
                sum=Byte;
                index   = 0;
                RecievedState = 1;
            }
        }
        if(RecievedState ==1 && NotTakeData)
        {
            NotTakeData = false;
            sum = sum +Byte;
            ChannalNum=Byte;
            RecievedState = 2;
        }
        if(RecievedState ==2 && NotTakeData)
        {
            NotTakeData = false;
            if(index<ChannalNum*4)
            {
                sum = sum +Byte;
                RecievedByteArray[index++] = Byte;
            }
            else
            {
                NotTakeData = true;
                RecievedState =3;
            }
        }
        if(RecievedState ==3 && NotTakeData)
        {
            NotTakeData = false;
            RecievedState =0;
            if(sum == Byte)
            {
                p  = (float *)RecievedByteArray;
                q = RecievedFloatArray;
                for(i=0;i<ChannalNum;i++)
                {
                    *q++ = *p++;
                }
                return 0xff;
            } else
            {
                printf("check sum err Getsum:%d,Checksum:%d ChanalNum:%d \r\n",Byte,sum,ChannalNum); //error
                return 0xfe ;
            }
        }
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        return 0;
    }
    float GetFloatFromCHx(unsigned char CHx)
    {
        return RecievedFloatArray[CHx];
    }

    int read_port(const char *buf, int iByte)
    {
        int iLen = 0;
        if( !iByte )
        {
             printf("Read byte number error !\n");
            return iLen;
        }

        iLen = read(Uart_fd,(void *)buf,iByte);

        return iLen;
    }
    int write_port(const char *buf, int iByte)
    {
        int iLen = 0;
        if( !iByte )
        {
            // printf("Write byte number error !\n");
            printf("Write byte number error !\n");

            return iLen;
        }

        iLen = write(Uart_fd,(void *)buf,iByte);

        return iLen;
    }
    void Disconnect()
    {
        if(Is_connect)
        {
            Is_connect=false;
            Is_PrintDetail=false;
            close(Uart_fd);
            Uart_fd=0;
        }
    }
private:
    unsigned char RecievedByteArray[1024];
    unsigned char sum=0;
    unsigned int index=0;
    unsigned char RecievedState = 0,ChannalNum=0;
    float *p  = (float *)RecievedByteArray;
    float *q = RecievedFloatArray;
    int open_port(const char *Portname)
    {
        int fd = -1;
        fd = open(Portname,O_RDWR|O_NOCTTY);
        if( fd<0 )
        {
            printf("Can't Open Serial Port !");
            return (-1);
        }
        else printf("Open %s ......\n",Portname);
        if( fcntl(fd,F_SETFL,0)<0 )/*恢复串口的状态为阻塞状态，用于等待串口数据的读入*/
        {
            printf("fcntl failed !\n");
            return (-1);
        }
        else  printf("fcntl = %d !\n",fcntl(fd,F_SETFL,0));
        /*测试打开的文件描述符是否应用一个终端设备，以进一步确认串口是否正确打开*/
        if( !isatty(STDIN_FILENO) )
        {
            printf("Standard input isn't a terminal device !\n");
            return (-1);
        }
        else  printf("It's a serial terminal device!\n");
        printf("open_port file ID= %d !\n",fd);
        return fd;
    }
    int set_port(int fd, int iBaudRate, int iDataSize, char cParity, int iStopBit)
    {
        int iResult = 0;
        struct termios oldtio,newtio;
        iResult = tcgetattr(fd,&oldtio);/*保存原先串口配置*/
        if( iResult )
        {
            printf("Can't get old terminal description !");
            return (-1);
        }
        bzero(&newtio,sizeof(newtio));
        newtio.c_cflag |= CLOCAL | CREAD;/*设置本地连接和接收使用*/

        /*设置输入输出波特率*/
        switch( iBaudRate )
        {
            case 2400:
                                cfsetispeed(&newtio,B2400);
                                cfsetospeed(&newtio,B2400);
                                break;
            case 4800:
                                cfsetispeed(&newtio,B4800);
                                cfsetospeed(&newtio,B4800);
                                break;
            case 9600:
                                cfsetispeed(&newtio,B9600);
                                cfsetospeed(&newtio,B9600);
                                break;
            case 19200:
                                cfsetispeed(&newtio,B19200);
                                cfsetospeed(&newtio,B19200);
                                break;
            case 38400:
                                cfsetispeed(&newtio,B38400);
                                cfsetospeed(&newtio,B38400);
                                break;
            case 57600:
                                cfsetispeed(&newtio,B57600);
                                cfsetospeed(&newtio,B57600);
                                break;
            case 115200:
                                cfsetispeed(&newtio,B115200);
                                cfsetospeed(&newtio,B115200);
                                break;
            case 460800:
                                cfsetispeed(&newtio,B460800);
                                cfsetospeed(&newtio,B460800);
                                break;
            default     :
                                /*perror("Don't exist iBaudRate !");*/
                                 printf("Don't exist iBaudRate %d !\n",iBaudRate);
                                return (-1);
        }

        /*设置数据位*/
        newtio.c_cflag &= (~CSIZE);
        switch( iDataSize )
        {
            case    7:
                            newtio.c_cflag |= CS7;
                            break;
            case    8:
                            newtio.c_cflag |= CS8;
                            break;
            default:
                            /*perror("Don't exist iDataSize !");*/
                             printf("Don't exist iDataSize %d !\n",iDataSize);
                            return (-1);
        }

        /*设置校验位*/
        switch( cParity )
        {
            case    'N':                    /*无校验*/
                                newtio.c_cflag &= (~PARENB);
                                break;
            case    'O':                    /*奇校验*/
                                newtio.c_cflag |= PARENB;
                                newtio.c_cflag |= PARODD;
                                newtio.c_iflag |= (INPCK | ISTRIP);
                                break;
            case    'E':                    /*偶校验*/
                                newtio.c_cflag |= PARENB;
                                newtio.c_cflag &= (~PARODD);
                                newtio.c_iflag |= (INPCK | ISTRIP);
                                break;
            default:
                                /*perror("Don't exist cParity  !");*/
                                 printf("Don't exist cParity %c !\n",cParity);
                                return (-1);
        }

        /*设置停止位*/
        switch( iStopBit )
        {
            case    1:
                            newtio.c_cflag &= (~CSTOPB);
                            break;
            case    2:
                            newtio.c_cflag |= CSTOPB;
                            break;
            default:
                            /*perror("Don't exist iStopBit !");*/
                             printf("Don't exist iStopBit %d !\n",iStopBit);
                            return (-1);
        }

        newtio.c_cc[VTIME] = 0; /*设置等待时间*/
        newtio.c_cc[VMIN] = 0;  /*设置最小字符*/
        tcflush(fd,TCIFLUSH);       /*刷新输入队列(TCIOFLUSH为刷新输入输出队列)*/
        iResult = tcsetattr(fd,TCSANOW,&newtio);    /*激活新的设置使之生效,参数TCSANOW表示更改立即发生*/

        if( iResult )
        {
            perror("Set new terminal description error !");
            return (-1);
        }
         printf("set_port success !\n");
         return 0;
    }


};

my_uart dev;

void* callback(void* ptr)
{
    static unsigned char RecievedData=0;
    static  char *data=new char;
    static char temp;
    static int err;
    data=(char*)&RecievedData;
    while(1)
    {
        if(dev.Is_connect==1)
        {
            if(dev.read_port(&temp,1))
            {
                do{
                    err=dev.RecievetoFloatArray(temp);
                    if(err==0xff)
                    {
                        dev.Uart_data_count_cur++;
                        //printf("receive ok,ch1 [%f]\n\r",dev.GetFloatFromCHx(0));
                        usleep(50*1000);
                    }
                }while(dev.Is_connect==1&&dev.read_port(&temp,1));
            }
        }
    }
}
void control_callback(const balance::Num::ConstPtr& msg)
{
    float xspeed,turnspeed;
    float kx=20;
    float kturn=10;
    static int i=0;

    if(i==1)i=0;
    else   i++;
    if(flag_auto)
    {
     xspeed=(msg->leftspeed+msg->rightspeed)*kx;
     turnspeed=(msg->leftspeed-msg->rightspeed)*kturn;
    //std::cout<<"xspeed:"<<xspeed<<"turn:"<<turnspeed<<std::endl;
  //  dev.SendFloat(16,xspeed);
  //  dev.SendFloat(17,turnspeed);
    if(i==0)
    {
        if(turnspeed>0)dev.SendCommend('d');
        else if(turnspeed<0)dev.SendCommend('a');
        std::cout<<"xspeed:"<<xspeed<<std::endl;
        if(turnspeed<6&&turnspeed>-6)dev.SendCommend(5);
    }
      if(i==1)
      {
        if(xspeed>0)dev.SendCommend('w');
        else if(xspeed<0)dev.SendCommend('s');
        std::cout<<"turn:"<<turnspeed<<std::endl;
       //if(xspeed<6&&xspeed>-6)dev.SendCommend(5);
      }

    }else
    {
        std::cout<<"notic :current mode is manual"<<std::endl;
    }
}

void cmd_callback(const std_msgs::String::ConstPtr& msg)
{
    std::string cmd;
    cmd=msg->data;
    std::cout<<"balance::Get comond:"<<cmd<<std::endl;
}

int flag_get_guicmd;
std::string gui_cmd;

void *callback_get_guicmd(void *ptr)
{
    std::cout<<"Get gui_cmd child begin"<<std::endl;
    char cmd[] = "cat /Robot/cmd/cmd_of_gui ";
    FILE *pp;
    while(1)
   {
    printf("waiting for fifo----");
    pp= popen(cmd, "r");
    if (pp != NULL)
    {
        char tmp[1024] = {0};
        if(fgets(tmp, sizeof(tmp), pp) != NULL)
        {
            flag_get_guicmd=1;
            gui_cmd=tmp;
            std::cout<<gui_cmd<<std::endl;
        }
    }
    if(flag_get_guicmd)
    {
         flag_get_guicmd=0;
         if(gui_cmd.find("noauto")==0)flag_auto=0;
         if(gui_cmd.find("auto")==0)flag_auto=1;
         if(flag_auto)std::cout<<"notice : current mode is auto"<<std::endl;
        else
        {
            std::cout<<"notice : current mode is mannual"<<std::endl;
            if(gui_cmd.find("cmd")==0)
            {
                char cmd[]="<0>";
                const char *data;
                data= gui_cmd.c_str();
                dev.SendCommend(data[4]);
                cmd[1]=data[4];
                std::cout<<cmd<<std::endl;
            }
        }
    }
    usleep(50*1000);
}
}

int main(int argc ,char ** argv)
{
    float count;
    pthread_t pid,pid_cmd;
    ros::init(argc, argv, "balance");
    ros::NodeHandle n;
    ros::Rate loop_rate(20);
    ros::Subscriber sub = n.subscribe("/control_speed", 1000, control_callback);
    ros::Subscriber pub_control= n.subscribe("/control_cmd",10000,cmd_callback);
    ros::Publisher pub_speed = n.advertise<balance::Num>("/wheel_speed", 1000);
    balance::Num speed;
    dev.Init("/dev/ttyUSB0",115200);
    pthread_create(&pid, NULL, callback, NULL);
    pthread_create(&pid, NULL, callback_get_guicmd, NULL);

    dev.SendStr("<A>");
    while(ros::ok())
    {
          //printf("i am 0:%f\n",dev.GetFloatFromCHx(0));
           speed.leftspeed=dev.GetFloatFromCHx(2)*(-0.01969);//cm
           speed.rightspeed=dev.GetFloatFromCHx(3)*(-0.01969);//cm
           pub_speed.publish(speed);
           ros::spinOnce();
           loop_rate.sleep();
           count+=(dev.GetFloatFromCHx(2)-dev.GetFloatFromCHx(3));
      //     std::cout<<"count::"<<count<<std::endl;
    }
   //  cout << "Hello World!" << endl;
    return 0;
}

