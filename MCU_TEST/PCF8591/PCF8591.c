#include "PCF8591.h"
#include "IIC.h"
bit ack2;
void Set_Channels(uchar n)  //读第几通道的数据
{
	switch(n)
	{
		case 0:
			ISendByte(PCF8591,0x40);  //设置通道0
			break;
		case 1:
			ISendByte(PCF8591,0x41);  //设置通道1
			break;
		case 2:
			ISendByte(PCF8591,0x42);  //设置通道2
			break;
		case 3:
			ISendByte(PCF8591,0x43);  //设置通道3
			break;
		default:
			ISendByte(PCF8591,0x40);  //默认设置通道0	
	}
}
bit DACconversion(unsigned char sla,unsigned char c,  unsigned char Val)  
{
   Start_I2c();              //启动总线
   SendByte(sla);            //发送器件地址
   if(ack2==0)return(0);
   SendByte(c);              //发送控制字节
   if(ack2==0)return(0);
   SendByte(Val);            //发送DAC的数值  
   if(ack2==0)return(0);
   Stop_I2c();               //结束总线
   return(1);
}

bit ISendByte(unsigned char sla,unsigned char c)
{
	 Start_I2c();              //启动总线
   SendByte(sla);            //发送器件地址
   if(ack2==0)return(0);
   SendByte(c);              //发送数据
   if(ack2==0)return(0);
   Stop_I2c();               //结束总线
   return(1);
}
uchar IRcvByte(unsigned char sla)
{
	 uchar c;
   Start_I2c();          //启动总线
   SendByte(sla+1);      //发送器件地址
   if(ack2==0)return(0);
   c=RcvByte();          //读取数据0
   Ack_I2c(1);           //发送非就答位
   Stop_I2c();           //结束总线
   return(c);
}