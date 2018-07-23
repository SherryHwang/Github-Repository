#ifndef __PCF8591_H__
#define __PCF8951_H__
#define uchar unsigned char
#define uint unsigned int
#define PCF8591  0X90    //PCF8591数据传输方向
bit DACconversion(unsigned char sla,unsigned char c,  unsigned char Val);//DAC 变换, 转化函数    
bit ISendByte(unsigned char sla,unsigned char c);//ADC发送字节[命令]数据函数      
uchar IRcvByte(unsigned char sla); //ADC读字节数据函数     
#endif