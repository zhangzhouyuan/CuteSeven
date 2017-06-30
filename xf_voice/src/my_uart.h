#ifndef MY_UART_H
#define MY_UART_H
#include "uart.h"
#include <stdio.h>
#include <strings.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

class My_Uart
{
public:
    My_Uart();
    ~My_Uart();
    bool Is_connect;
    bool Is_PrintDetail;
    int Uart_fd;
    int Uart_data_count;
    int Uart_data_count_cur;
    float RecievedFloatArray[1024];
    void Init(const char *path, int rate);
    void SendStr(const char *str);
    void Sendbyte(unsigned char byte);
    void SendCommend(unsigned char Commend);
    void SendFloat(unsigned char Addr, float FloatData);
    unsigned char RecievetoFloatArray(unsigned char Byte);
    float GetFloatFromCHx(unsigned char CHx);

    int read_port(const char *buf, int iByte);
    int write_port(const char *buf, int iByte);
    void Disconnect();
private:
    int open_port(const char *Portname);
    int set_port(int fd, int iBaudRate, int iDataSize, char cParity, int iStopBit);

    unsigned char RecievedByteArray[1024];
    unsigned char sum=0;
    unsigned int index=0;
    unsigned char RecievedState = 0,ChannalNum=0;
    unsigned int *p  = (unsigned int*)RecievedByteArray;
    float *q = RecievedFloatArray;

};

#endif // MY_UART_H

