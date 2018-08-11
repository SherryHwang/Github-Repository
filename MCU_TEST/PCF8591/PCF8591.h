#ifndef __PCF8591_H__
#define __PCF8951_H__
#include "DEFINE.h"

bit DACconversion(uchar sla,uchar c,  uchar Val);//DAC 变换, 转化函数    
bit ISendByte(uchar sla,uchar c);//ADC发送字节[命令]数据函数      
uchar IRcvByte(uchar sla); //ADC读字节数据函数     
#endif