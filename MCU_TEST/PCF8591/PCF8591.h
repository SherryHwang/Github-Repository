#ifndef __PCF8591_H__
#define __PCF8951_H__
#define uchar unsigned char
#define uint unsigned int
#define PCF8591  0X90    //PCF8591���ݴ��䷽��
bit DACconversion(unsigned char sla,unsigned char c,  unsigned char Val);//DAC �任, ת������    
bit ISendByte(unsigned char sla,unsigned char c);//ADC�����ֽ�[����]���ݺ���      
uchar IRcvByte(unsigned char sla); //ADC���ֽ����ݺ���     
#endif