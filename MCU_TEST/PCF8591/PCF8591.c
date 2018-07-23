#include "PCF8591.h"
#include "IIC.h"
bit ack2;
void Set_Channels(uchar n)  //���ڼ�ͨ��������
{
	switch(n)
	{
		case 0:
			ISendByte(PCF8591,0x40);  //����ͨ��0
			break;
		case 1:
			ISendByte(PCF8591,0x41);  //����ͨ��1
			break;
		case 2:
			ISendByte(PCF8591,0x42);  //����ͨ��2
			break;
		case 3:
			ISendByte(PCF8591,0x43);  //����ͨ��3
			break;
		default:
			ISendByte(PCF8591,0x40);  //Ĭ������ͨ��0	
	}
}
bit DACconversion(unsigned char sla,unsigned char c,  unsigned char Val)  
{
   Start_I2c();              //��������
   SendByte(sla);            //����������ַ
   if(ack2==0)return(0);
   SendByte(c);              //���Ϳ����ֽ�
   if(ack2==0)return(0);
   SendByte(Val);            //����DAC����ֵ  
   if(ack2==0)return(0);
   Stop_I2c();               //��������
   return(1);
}

bit ISendByte(unsigned char sla,unsigned char c)
{
	 Start_I2c();              //��������
   SendByte(sla);            //����������ַ
   if(ack2==0)return(0);
   SendByte(c);              //��������
   if(ack2==0)return(0);
   Stop_I2c();               //��������
   return(1);
}
uchar IRcvByte(unsigned char sla)
{
	 uchar c;
   Start_I2c();          //��������
   SendByte(sla+1);      //����������ַ
   if(ack2==0)return(0);
   c=RcvByte();          //��ȡ����0
   Ack_I2c(1);           //���ͷǾʹ�λ
   Stop_I2c();           //��������
   return(c);
}