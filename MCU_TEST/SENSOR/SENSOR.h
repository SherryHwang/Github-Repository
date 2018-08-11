#ifndef __SENSOR_H__
#define __SENSOR_H__
#include "DEFINE.h"
sbit LED = P1 ^ 0;			  	 		
sbit BUZZER = P1 ^ 1;	
sbit FAN = P1 ^ 2;	         //风扇
sbit DCMOTOR = P1 ^ 3;       //直流电机
sbit STEPMOTOR_CP = P1^4;    //步进电机脉冲输出
sbit STEPMOTOR_DIR = P1^5;   //步进电机方向控制

sbit A = P1^4;	  
sbit b = P1^5;
sbit C = P1^6;
sbit DD = P1^7;

sbit hc_hr501 = P3^6;        //人体红外

//超声波模块引脚定义
sbit trig = P3^5;//触发信号
sbit echo = P3^4;//接收信号

sbit IRIN = P3^2;         //红外接收器数据线 外部中断0

void SendChar_PC(uchar sd);
void SendInt_PC(uint sd);
void SendLong_PC(unsigned long sd);
void SendString_PC(uchar sd[]);
void _7SEG_LED();						//1.七段数码管
void LED_RUN();							//2.LED
void LATTICE_RUN();      		//3.点阵
void STATIC_DIGITAL_RUN();  //4.静态数码管
void BUZZER_RUN();					//5.蜂鸣器
void DCMOTOR_RUN();					//6.直流电机
void STEPMOTOR_RUN();				//7.步进电机
void FAN_RUN();							//8.风扇
void DS18B20_RUN();					//9.温度传感器
void ULTRASONIC_RUN();			//10.超声波
void AD_SENSOR_RUN();       //11.光敏传感器、湿敏、热敏、振动
void DIGITAL_LIGHT_INTENSITY_RUN();//12.数字光强
void BLUETOOTH_RUN();				//13.蓝牙
void INFRARED_REC_RUN();		//14.红外接收
void ADXL345_RUN();  			  //15.三轴加速度
void ELECTRONIC_SCALE_RUN();//16.电子称
void MQ_RUN();              //17 18 19 20 21 气体、烟雾传感器
void HC_HR501_RUN();				//22.人体红外
void LM393_RUN();						//23.声音传感器
void Display(uint distance);//显示函数  -LCD显示超声波检测数据

#endif 