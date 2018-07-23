#ifndef __IIC_H__
#define __IIC_H__
#define uchar unsigned char
#define uint unsigned int
void IDefine_Bus(bit SDA_NEW,SDL_NEW);  //定义时钟和数据总线
void Start_I2c(); //起动总线函数
void Stop_I2c(); //结束总线函数  
void Ack_I2c(bit a);//应答子函数
void  SendByte(unsigned char  c);//字节数据发送函数            
unsigned char RcvByte();//无子地址读字节数据函数 
#endif