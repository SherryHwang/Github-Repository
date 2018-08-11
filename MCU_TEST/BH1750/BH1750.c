#include "BH1750.h"
#include "IIC.h"
#include "Delay.h"
#include "lcd1602.h"
extern uchar BUF[8];

void BH1750_Write_Single(uchar REG_Address)				//单字节写入
{
  Start_I2c();  //开启I2C数据传输
	SendByte(0x46); //发送设备地址+写信号,确定I2C上的设备
	SendByte(REG_Address);  //内部寄存器地址，请参考中文pdf22页 
	Stop_I2c();  //停止I2C数据传输
}

void BH1750_Init()																	//初始化BH1750
{
	BH1750_Write_Single(0x01);// power on
	BH1750_Write_Single(0x10);// H- resolution mode
}

void BH1750_Read_Multiple()												//连续的读取内部寄存器数据
{
	uchar i;	
	Start_I2c();                          //起始信号
	SendByte(0x47);         //发送设备地址+读信号
	for (i=0; i<3; i++)                      //连续读取2个地址数据，存储中BUF
	{
		BUF[i] = RcvByte();         					 //BUF[0]存储0x32地址中的数据
		if (i == 3)
		{
			 Ack_I2c(1);                //最后一个数据需要回NOACK
		}
		else
		{		
			 Ack_I2c(0);                //回应ACK
		}
	}
	BUF[3] = '\0';
	Stop_I2c();                          //停止信号
	Delay_ms(5);
}
void BH1750_Display()
{
	int dis_data = (BUF[0]<<8)+BUF[1];
	dis_data=(float)dis_data/1.2;
	
	dis_data = dis_data%100000;
	lcd1602_write_char(0,1,dis_data/10000 + '0'); //显示数据 wan
	dis_data = dis_data%10000;
	lcd1602_write_char(1,1,dis_data/1000 + '0');  //qian
	dis_data = dis_data%1000;	
	lcd1602_write_char(2,1,dis_data/100 + '0');   //bai
	dis_data = dis_data%100;
	lcd1602_write_char(3,1,dis_data/10 + '0');   //shi
	lcd1602_write_char(4,1,dis_data%10 + '0');         //ge

	lcd1602_write_char(5,1,'L'); ////显示数单位
	lcd1602_write_char(6,1,'X');  
}