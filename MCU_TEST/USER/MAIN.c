#include "reg52.h"
#include "SENSOR.h"
#include "Delay.h"
#include "string.h"

uchar data_come = 0; //数据帧开始传输标志
uchar data_flag = 0; //帧长度
uchar is_Start= 0;   //各个模块开始运行标志
uchar num = 0;
uchar  a[10];       //用于存储串口发送的信息
uchar index = 0;
void SerialInit()   	
{
	TMOD = 0x20;  	//T1工作模式2  8位自动重装
	TH1 = 0xfd;
	TL1 = 0xfd; 	//比特率9600
	TR1 = 1;		//启动T1定时器
	SM0 = 0;
	SM1 = 1; 		//串口工作方式1 10位异步
	REN = 1;		//串口允许接收
	EA  = 1;		//开总中断
	ES  = 1;		//串口中断打开
}
void main()
{
	uchar len = 0;
	SerialInit();
	while(1)
	{
		PraseData();
		//SENSOR_RUN();
		Delay(10);
	}
}
void Serail_Proc() interrupt 4
{
	uchar i;
	if(RI)
	{
		RI = 0;
		num = SBUF;
		Delay(5);
		SBUF = num;
		while(!TI);
		TI = 0;
		
		if(num == 0)
		{
			if(data_come && !data_flag)  //第二位数据为0  ，停止帧
			{
				data_come = 0;
				is_Start = 0;
			}
			else if(!data_come)
				data_come = 1;
		}
		else
		{
			data_flag = 1;    //通讯帧
			if(data_come)
				is_Start = 1;
		}
		
		if(is_Start)
		{
			
			if(num != 0xFF)   //不是最后一个数据
			{
				a[index++] = num;  //数据存入数组
			}
			else             //最后一个数据
			{
				index  = 0;    //数组下标置0
				data_flag = 0; //默认为停止帧
				data_come = 0; //帧开始标志置为0
				
				for(i = 0;i<5;i++)
				{
					SBUF = 0xFF;
					while(!TI);
					TI = 0;
				}
				for(i = 0;i<10;i++)  //将接收到的数据发送出
				{
					SBUF = a[i];
					while(!TI);
					TI = 0;
				}
			}
		}
	}
	
}
