#include "Delay.h"
#include "PCF8591.h"
#include "IIC.h"

/*******************************************************************
DAC �任, ת������               
*******************************************************************/
bit DACconversion(unsigned char sla,unsigned char c,  unsigned char Val)
{
   Start_I2c();              //��������
   SendByte(sla);            //����������ַ
   if(ack==0)return(0);
   SendByte(c);              //���Ϳ����ֽ�
   if(ack==0)return(0);
   SendByte(Val);            //����DAC����ֵ  
   if(ack==0)return(0);
   Stop_I2c();               //��������
   return(1);
}

/*******************************************************************
ADC�����ֽ�[����]���ݺ���               
*******************************************************************/
bit ISendByte(unsigned char sla,unsigned char c)
{
   Start_I2c();              //��������
   SendByte(sla);            //���Ͷ�д�����ź�  д
   if(ack==0)return(0);
   SendByte(c);              //ѡ��ͨ��
   if(ack==0)return(0);
   Stop_I2c();               //��������
   return(1);
}

/*******************************************************************
ADC���ֽ����ݺ���               
*******************************************************************/
unsigned char IRcvByte(unsigned char sla)
{  
	 unsigned char c;
   Start_I2c();          //��������
   SendByte(sla+1);      //���Ͷ�д�����ź� ��
   if(ack==0)return(0);
   c=RcvByte();          //��ȡ����0
   Ack_I2c(1);           //���ͷǾʹ�λ
   Stop_I2c();           //��������
   return(c);
}
 

//*****���ڳ�ʼ������***********

//******************************
void Initial_com(void)
{
 EA=1;        //�����ж�
 ES=1;        //�������ж�
 ET1=1;        //����ʱ��T1���ж�
 TMOD=0x20;   //��ʱ��T1���ڷ�ʽ2�жϲ���������
 PCON=0x00;   //SMOD=0
 SCON=0x50;   // ��ʽ1 �ɶ�ʱ������
 TH1=0xfd;    //����������Ϊ9600
 TL1=0xfd;
 TR1=1;       //����ʱ��T1���п���λ
}