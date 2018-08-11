#ifndef __IIC_H__
#define __IIC_H__
#include "DEFINE.h"
sbit     SCL=P2^0;       //I2C  时钟 
sbit     SDA=P2^1;       //I2C  数据 
sbit     SCL_BH1750=P2^6;       //I2C  时钟   num = 12
sbit     SDA_BH1750=P2^7;       //I2C  数据 
sbit     SCL_ADXL345=P1^4;       //I2C  时钟 num = 15
sbit     SDA_ADXL345=P1^5;       //I2C  数据 
extern bit ack;
//起动总线函数
void Start_I2c();
//结束总线函数  
void Stop_I2c();
//应答子函数
void Ack_I2c(bit a);
//字节数据发送函数
void  SendByte(unsigned char  c);
//有子地址发送多字节数据函数               
bit ISendStr(unsigned char sla,unsigned char suba,unsigned char *s,unsigned char no) ;
//无子地址发送多字节数据函数   
bit ISendStrExt(unsigned char sla,unsigned char *s,unsigned char no);
//无子地址读字节数据函数               
unsigned char RcvByte();
#endif
            
