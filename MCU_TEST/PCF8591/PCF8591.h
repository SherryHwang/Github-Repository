#ifndef __PCF8591_H__
#define __PCF8951_H__
#include "DEFINE.h"

bit DACconversion(uchar sla,uchar c,  uchar Val);//DAC �任, ת������    
bit ISendByte(uchar sla,uchar c);//ADC�����ֽ�[����]���ݺ���      
uchar IRcvByte(uchar sla); //ADC���ֽ����ݺ���     
#endif