#include <iostream>
#include"mraa.hpp"
#include "pthread.h"
#include <unistd.h>
#include"balance/Num.h"
#include"ros/ros.h"
#include"std_msgs/Float32.h"
#include"std_msgs/String.h"
int flag_heat=0;
using namespace std;
class my_uart
{
public:
    bool Is_connect;
    my_uart()
    {
        Is_connect=0;
    }
    ~my_uart(){}
    mraa::Uart *dev;

    float RecievedFloatArray[10];

    void Init(const std::string path,int rate)
    {
        try { dev = new mraa::Uart(0);  }
        catch (std::exception& e) {   std::cout << e.what() << ", likely invalid platform config" << std::endl;      }
        try {   dev = new mraa::Uart(path);   }
        catch (std::exception& e)
        {
            std::cout << "Error while setting up raw UART, do you have a uart?" << std::endl;
            std::terminate();
        }
        if (dev->setBaudRate(rate) != mraa::SUCCESS) {  std::cout << "Error setting parity on UART" << std::endl;    }
        if (dev->setMode(8, mraa::UART_PARITY_NONE, 1) != mraa::SUCCESS) {  std::cout << "Error setting parity on UART" << std::endl;        }
        if (dev->setFlowcontrol(false, false) != mraa::SUCCESS) { std::cout << "Error setting flow control UART" << std::endl;
        }
        Is_connect=1;
       //  dev->writeStr("<A>");
    }
    void SendStr(const std::string str)
    {
        if(Is_connect)dev->writeStr(str);
        else std::cout<<"you haven't conect!"<<std::endl;
    }
    void Sendbyte(unsigned char byte)
    {
        char *data=new char;
        *data=byte;
        if(Is_connect)dev->write(data,1);
        else std::cout<<"you haven't conect!"<<std::endl;
    }
    void SendCommend(unsigned char Commend)//have tested
    {
        Sendbyte('<');
        Sendbyte(Commend);
        Sendbyte('>');
    }
    void SendFloat(unsigned char Addr,float FloatData)//have tested
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
     static unsigned char RecievedByteArray[40];
     static unsigned char sum=0;
     static unsigned char index=0;
     static unsigned char RecievedState = 0;
     static float *p  = (float*)RecievedByteArray;
     static float *q = RecievedFloatArray;
     unsigned char i;
     bool NotTakeData ;
     NotTakeData = true;
      if(RecievedState ==0||RecievedState ==3&&NotTakeData)
      {
         NotTakeData = false;
         if(Byte=='$')
          {
             RecievedState = 1;
             index   = 0;
         }
      }
      if(RecievedState ==1&&NotTakeData)
      {
          NotTakeData = false;
          if(index<40)
          {
              RecievedByteArray[index++] = Byte;
          }
          else
          {
              RecievedState =2;
          }
          sum = sum +Byte;
      }
      if(RecievedState ==2&&NotTakeData)
      {
          NotTakeData = false;
          sum = sum+'$';
          if(sum == Byte)
          {
              for(i=0;i<10;i++)
                 *q++ = *p++;
              RecievedState = 3;
          }

          else
          {
             ; //error
             RecievedState =0;
          }
          p  = (float*)RecievedByteArray;
          q = RecievedFloatArray;
          sum = 0;
      }
      return RecievedState ;
  }
    float GetFloatFromCHx(unsigned char CHx)
    {
        return RecievedFloatArray[CHx];
    }

    void SetYunTai(unsigned char Angle) //have tested
    {
        float PWM_YunTai;
        PWM_YunTai = (float)Angle/180.0*1000+250;
        SendFloat(11,PWM_YunTai);
    }
    void QiLuo12Down(){SendFloat(12,600);}//have tested
    void QiLuo3Down(){SendFloat(13,1000);}//have tested
    void QiLuo4Down(){SendFloat(14,1000);}//have tested
    void QiLuo12Up(){SendFloat(12,1000);}//have tested
    void QiLuo3Up(){SendFloat(13,600);}//have tested
    void QiLuo4Up(){SendFloat(14,600);}//have tested

};

void demo_uart(void)
{
    mraa::Uart * dev;
    try {
        dev = new mraa::Uart(0);
    } catch (std::exception& e) {
        std::cout << e.what() << ", likely invalid platform config" << std::endl;
    }
    try {
        dev = new mraa::Uart("/dev/ttyS5");
    } catch (std::exception& e) {
        std::cout << "Error while setting up raw UART, do you have a uart?" << std::endl;
        std::terminate();
    }
    if (dev->setBaudRate(115200) != mraa::SUCCESS) {
        std::cout << "Error setting parity on UART" << std::endl;
    }
    if (dev->setMode(8, mraa::UART_PARITY_NONE, 1) != mraa::SUCCESS) {
        std::cout << "Error setting parity on UART" << std::endl;
    }    if (dev->setFlowcontrol(false, false) != mraa::SUCCESS) {
        std::cout << "Error setting flow control UART" << std::endl;
    }
    dev->writeStr("<A>");
}

my_uart dev;
void* callback(void* ptr)
{
  static unsigned char RecievedData=0;
  static  char *data=new char;
  static int num;
  data=(char*)&RecievedData;
  while(1)
  {
      while(!dev.dev->dataAvailable(0));
      num = dev.dev->read(data,1);
      //printf("%d ",RecievedData);
      //printf("i have get the  data:%d \n",RecievedData);
     // printf("Recieve state is :%d\n",dev.RecievetoFloatArray(RecievedData));
      if(dev.RecievetoFloatArray(RecievedData)==3)
      {
      //   printf("i have get the ok data \n");
         usleep(50*1000);
      }
  }
}
void cmd_callback(const std_msgs::String::ConstPtr& msg)
{
    std::string cmd;
    cmd=msg->data;
    std::cout<<"arduino::Get comond:"<<cmd<<std::endl;
    if(cmd=="A")
    {
            dev.SendStr("<A>");
    }
    if(cmd=="B")
    {
            dev.SendStr("<B>");
    }
    if(cmd=="C")
    {
            dev.SendStr("<C>");
    }
    if(cmd=="D")
    {
            dev.SendStr("<D>");
    }
}
int main(int argc ,char ** argv)
{
    float flag_now,flag_last,heart;
    pthread_t pid;
    ros::init(argc, argv, "arduino");
    ros::NodeHandle n;
    ros::Rate loop_rate(20);
//    ros::Subscriber sub = n.subscribe("control_speed", 1000, control_callback);
       ros::Subscriber pub_control= n.subscribe("/control_cmd",10000,cmd_callback);
       ros::Publisher pub_heart = n.advertise<std_msgs::Float32>("heart_rate", 1000);
       ros::Publisher pub_tts = n.advertise<std_msgs::String>("/voice/xfsaywords", 1000);
       std_msgs::Float32 heart_rate;
 //   balance::Num speed;
    dev.Init("/dev/ttyS0",115200);
    pthread_create(&pid, NULL, callback, NULL);
    while(ros::ok())
    {
     //printf("i am 0:%f\n",dev.GetFloatFromCHx(0));
     //speed.leftspeed=dev.GetFloatFromCHx(2)*0.01999;//cm
     //speed.rightspeed=dev.GetFloatFromCHx(3)*0.01999;//cm
     //pub_speed.publish(speed);

        if(dev.GetFloatFromCHx(2)==1)
        {
            printf("i am 0:%f\n",dev.GetFloatFromCHx(0));
            heart_rate.data=dev.GetFloatFromCHx(0);
            pub_heart.publish(heart_rate);
        }
        flag_now=dev.GetFloatFromCHx(2);
        std::cout<<flag_now<<std::endl;
        if(flag_now==1&&flag_last==0)
         {
            std_msgs::String msg;
            std::stringstream ss;
            ss << "测量中，请保持不动";
            std::cout<<ss;
            msg.data = ss.str();
            pub_tts.publish(msg);
        }
        if(flag_now==0&&flag_last==1)
        {
            heart=dev.GetFloatFromCHx(3);
            std_msgs::String msg;
            std::stringstream ss;
            ss << "测量结束，您的心率为每分钟： "<<heart<<"次。";
             std::cout<<ss;
            msg.data = ss.str();
            pub_tts.publish(msg);
        }
        flag_last=flag_now;
         ros::spinOnce();
         loop_rate.sleep();
    }
   //  cout << "Hello World!" << endl;
    return 0;
}

