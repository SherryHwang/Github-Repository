#include "DELAY.h"
#include "reg52.h"
#include "PCF8591.h"
#include "LCD1602.h"
#include "SENSOR.h"

sbit LED = P1 ^ 0;				 		
sbit BUZZER = P1 ^ 1;	
sbit FAN = P1 ^ 2;	         //风扇
sbit Dcmotor = P1 ^ 3;         //直流电机

void DCMOTOR_RUN()           //直流电机转动
{
	Dcmotor = 1;
}

void MQ2_RUN()  //处理MQ2数据并显示  烟雾
{
	uchar D = 0;
	D=IRcvByte(PCF8591);  //读取模拟转换数据
	if(D>60)
	{
		lcd1602_write_string(0,1,"WARNING         ");
		lcd1602_write_string(0,1,"FROM Q2");
		LED = 1;//LED亮
		FAN=1;  //风扇转
		
		BUZZER = 0;//蜂鸣器响
		Delay(100);
		BUZZER = 1;//蜂鸣器不响
		Delay(100);
		BUZZER = 0;//蜂鸣器响
		Delay(100);
		BUZZER = 1;//蜂鸣器不响		
	}
	else
	{
		FAN=0;     //风扇不转
		LED = 0;   //LED灯不亮
		BUZZER = 1; //蜂鸣器不响
		lcd1602_write_string(0,1,"THERE IS SAFE");
	}
	SBUF = D;
	while(!TI);
	TI = 0;   //将值发送至PC
}

void MQ3_RUN()  //处理MQ3数据并显示  酒精
{
	uchar D = 0;
	D=IRcvByte(PCF8591);  //读取模拟转换数据
	if(D>40)
	{
		lcd1602_write_string(0,0,"WARNING         ");
		lcd1602_write_string(0,1,"FROM Q3");
		LED = 1;//LED亮
		BUZZER = 0;//蜂鸣器响
		Delay(100);
		BUZZER = 1;//蜂鸣器不响
		Delay(100);
		BUZZER = 0;//蜂鸣器响
		Delay(100);
		BUZZER = 1;//蜂鸣器不响
	}
	else
	{
		LED = 0;
		BUZZER = 1;
		lcd1602_write_string(0,0,"THERE IS OK");
	}
	
	//将值发送至PC
	SBUF = D;
	while(!TI);
	TI = 0;  
}

void MQ4_RUN()  //处理MQ4数据并显示  天然气
{
	uchar D = 0;
	D=IRcvByte(PCF8591);  //读取模拟转换数据
	if(D>40)
	{
		lcd1602_write_string(0,0,"WARNING         ");
		lcd1602_write_string(0,1,"FROM Q3");
		LED = 1;//LED亮
		BUZZER = 0;//蜂鸣器响
		Delay(100);
		BUZZER = 1;//蜂鸣器不响
		Delay(100);
		BUZZER = 0;//蜂鸣器响
		Delay(100);
		BUZZER = 1;//蜂鸣器不响
	}
	else
	{
		LED = 0;
		BUZZER = 1;
		lcd1602_write_string(0,0,"THERE IS OK");
	}
	
	//将值发送至PC
	SBUF = D;
	while(!TI);
	TI = 0;  
}

void MQ5_RUN()  //处理MQ5数据并显示  液体
{
	uchar D = 0;
	D=IRcvByte(PCF8591);  //读取模拟转换数据
	if(D>40)
	{
		lcd1602_write_string(0,0,"WARNING         ");
		lcd1602_write_string(0,1,"FROM Q3");
		LED = 1;//LED亮
		BUZZER = 0;//蜂鸣器响
		Delay(100);
		BUZZER = 1;//蜂鸣器不响
		Delay(100);
		BUZZER = 0;//蜂鸣器响
		Delay(100);
		BUZZER = 1;//蜂鸣器不响
	}
	else
	{
		LED = 0;
		BUZZER = 1;
		lcd1602_write_string(0,0,"THERE IS OK");
	}
	
	//将值发送至PC
	SBUF = D;
	while(!TI);
	TI = 0;  
}

void MQ135_RUN()  //处理MQ135数据并显示 液体
{
	uchar D = 0;
	D=IRcvByte(PCF8591);  //读取模拟转换数据
	if(D>40)
	{
		lcd1602_write_string(0,0,"WARNING         ");
		lcd1602_write_string(0,1,"FROM Q3");
		LED = 1;//LED亮
		BUZZER = 0;//蜂鸣器响
		Delay(100);
		BUZZER = 1;//蜂鸣器不响
		Delay(100);
		BUZZER = 0;//蜂鸣器响
		Delay(100);
		BUZZER = 1;//蜂鸣器不响
	}
	else
	{
		LED = 0;
		BUZZER = 1;
		lcd1602_write_string(0,0,"THERE IS OK");
	}
	
	//将值发送至PC
	SBUF = D;
	while(!TI);
	TI = 0;  
}


void SENSOR_RUN()
{
	DCMOTOR_RUN();//直流电机
	MQ2_RUN();    //MQ2
	MQ3_RUN();    //MQ3
	MQ4_RUN();    //MQ4
	MQ5_RUN();    //MQ5
	MQ135_RUN();  //MQ135
}
