#ifndef __IIC_H__
#define __IIC_H__
#include "DEFINE.h"
sbit     SCL=P2^0;       //I2C  ʱ�� 
sbit     SDA=P2^1;       //I2C  ���� 
sbit     SCL_BH1750=P2^6;       //I2C  ʱ��   num = 12
sbit     SDA_BH1750=P2^7;       //I2C  ���� 
sbit     SCL_ADXL345=P1^4;       //I2C  ʱ�� num = 15
sbit     SDA_ADXL345=P1^5;       //I2C  ���� 
extern bit ack;
//�����ߺ���
void Start_I2c();
//�������ߺ���  
void Stop_I2c();
//Ӧ���Ӻ���
void Ack_I2c(bit a);
//�ֽ����ݷ��ͺ���
void  SendByte(unsigned char  c);
//���ӵ�ַ���Ͷ��ֽ����ݺ���               
bit ISendStr(unsigned char sla,unsigned char suba,unsigned char *s,unsigned char no) ;
//���ӵ�ַ���Ͷ��ֽ����ݺ���   
bit ISendStrExt(unsigned char sla,unsigned char *s,unsigned char no);
//���ӵ�ַ���ֽ����ݺ���               
unsigned char RcvByte();
#endif
            
