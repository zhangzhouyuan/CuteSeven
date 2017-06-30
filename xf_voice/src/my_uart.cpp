#include "my_uart.h"
#include "pthread.h"
#include <unistd.h>
#include <iostream>
#include "stdio.h"
#include "string.h"
#include "QDebug"
#include "sys.h"
using namespace std;
My_Uart::My_Uart()
{
   int i;
   Is_connect=0;
   sum=0;
   index=0;
   RecievedState = 0;
   ChannalNum=0;
   p = (unsigned int *)RecievedByteArray;
   q = RecievedFloatArray;
   Is_PrintDetail=0;
   for(i=0;i<255;i++)
   {
       RecievedFloatArray[i]=0;
   }

}

My_Uart::~My_Uart()
{

   if(Is_connect)
   {
       Is_connect=false;
       Is_PrintDetail=false;
       close(Uart_fd);
   }
}
void My_Uart::Disconnect()
{
    if(Is_connect)
    {
        Is_connect=false;
        Is_PrintDetail=false;
        close(Uart_fd);
        Uart_fd=0;
    }
}
void My_Uart::Init(const char *path,int rate)
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
        qDebug("open_port error !");
        err=1;
    }

    iBaudRate = rate;
    iDataSize = 8;
    cParity = 'N';
    iStopBit = 1;
    iResult = set_port(Uart_fd,iBaudRate,iDataSize,cParity,iStopBit);
    if( iResult<0 )
    {
        qDebug("set_port error !");
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
int My_Uart::open_port(const char *Portname)
{
    int fd = -1;
    fd = open(Portname,O_RDWR|O_NOCTTY);
    if( fd<0 )
    {
        qDebug("Can't Open Serial Port !");
        return (-1);
    }
    else
    {
         qDebug("Open %s ......\n",Portname);
    }

    if( fcntl(fd,F_SETFL,0)<0 )/*恢复串口的状态为阻塞状态，用于等待串口数据的读入*/
    {
         qDebug("fcntl failed !\n");
        return (-1);
    }
    else
    {
         qDebug("fcntl = %d !\n",fcntl(fd,F_SETFL,0));
    }

    /*测试打开的文件描述符是否应用一个终端设备，以进一步确认串口是否正确打开*/
    if( !isatty(STDIN_FILENO) )
    {
         qDebug("Standard input isn't a terminal device !\n");
        return (-1);
    }
    else
    {
         qDebug("It's a serial terminal device!\n");
    }

     qDebug("open_port file ID= %d !\n",fd);

    return fd;

}

int My_Uart::set_port(int fd,int iBaudRate,int iDataSize,char cParity,int iStopBit)
{
    int iResult = 0;
    struct termios oldtio,newtio;


    iResult = tcgetattr(fd,&oldtio);/*保存原先串口配置*/
    if( iResult )
    {
        qDebug("Can't get old terminal description !");
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
                             qDebug("Don't exist iBaudRate %d !\n",iBaudRate);
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
                         qDebug("Don't exist iDataSize %d !\n",iDataSize);
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
                             qDebug("Don't exist cParity %c !\n",cParity);
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
                         qDebug("Don't exist iStopBit %d !\n",iStopBit);
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

     qDebug("set_port success !\n");

    return 0;
}




void My_Uart::SendStr(const char *str)
{
    if(Is_connect)write_port(str,strlen(str));
    else std::cout<<"you haven't conect!"<<std::endl;
}

void My_Uart::Sendbyte(unsigned char byte)
{
    char *data=new char;
    *data=byte;
    if(Is_connect)write_port(data,1);
    else std::cout<<"you haven't conect!"<<std::endl;
}

void My_Uart::SendCommend(unsigned char Commend)//have tested
{
    Sendbyte('<');
    Sendbyte(Commend);
    Sendbyte('>');
}

void My_Uart::SendFloat(unsigned char Addr,float FloatData)//have tested
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
unsigned char My_Uart::RecievetoFloatArray(unsigned char Byte)
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
        }else if(Byte=='{')
        {
            sum=Byte;
            index   = 0;
            RecievedState = 11;
        }else if(Byte==0xAA)
        {
            sum=Byte;
            index   = 0;
            RecievedState = 21;
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
            p  = (unsigned int *)RecievedByteArray;
            q = RecievedFloatArray;
            for(i=0;i<ChannalNum;i++)
            {
                *q++ = *p++;
            }
            return 0xff;
        } else
        {
            qDebug("check sum err Getsum:%d,Checksum:%d ChanalNum:%d \r\n",Byte,sum,ChannalNum); //error
            return 0xfe ;
        }
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if(RecievedState ==11 && NotTakeData)
    {
        NotTakeData = false;
        if(index<5)
        {
            sum = sum +Byte;
            RecievedByteArray[index++] = Byte;
        }
        else
        {
            NotTakeData = true;
            RecievedState =12;
        }
    }
    if(RecievedState ==12 && NotTakeData)
    {
        NotTakeData = false;
        RecievedState =0;
        if(sum == Byte)
        {
            p  = (unsigned int *)(RecievedByteArray+1);
            q[RecievedByteArray[0]]=*p;
            AddData(RecievedByteArray[0],*p);
            return 0xff;
        } else
        {
            printf("check sum err Getsum:%d,Checksum:%d DataAddr:%d \r\n",Byte,sum,RecievedByteArray[0]); //error
            AddData(RecievedByteArray[0],RecievedFloatArray[RecievedByteArray[0]]);
            return 0xfe ;
        }
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if(RecievedState ==21 && NotTakeData)
    {
        NotTakeData = false;
        if(Byte==0xAA)
        {
            RecievedState=22;
        }
        else
        {
            RecievedState=0;
        }
    }
    if(RecievedState ==22 && NotTakeData)
    {
        if(index<10)
        {
            RecievedByteArray[index++] = Byte;
        }else
        {
            NotTakeData = true;
            RecievedState =23;
        }
    }
    if(RecievedState ==23 && NotTakeData)
    {
        if(RecievedByteArray[0]==4)
        {
            AddData(20,(float)(RecievedByteArray[1]*0x100+float(RecievedByteArray[2])));
            AddData(22,(float)(RecievedByteArray[3]));
        }
        if(RecievedByteArray[4]==5)
        {
            AddData(21,(float)(RecievedByteArray[5]*0x100+float(RecievedByteArray[6])));
            AddData(23,(float)(RecievedByteArray[7]));
        }
        RecievedState=0;
    }
    return 0;
}
float My_Uart::GetFloatFromCHx(unsigned char CHx)
{
    return RecievedFloatArray[CHx];
}
int My_Uart::read_port(const char *buf, int iByte)
{
    int iLen = 0;
    if( !iByte )
    {
         qDebug("Read byte number error !\n");
        return iLen;
    }

    iLen = read(Uart_fd,(void *)buf,iByte);

    return iLen;
}

int My_Uart::write_port(const char *buf,int iByte)
{
    int iLen = 0;
    if( !iByte )
    {
        // qDebug("Write byte number error !\n");
        qDebug("Write byte number error !\n");

        return iLen;
    }

    iLen = write(Uart_fd,(void *)buf,iByte);

    return iLen;
}


