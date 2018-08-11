#include "reg52.h"
#include "DEFINE.h"
#include "SENSOR.h"
#include "Delay.h"
#include "string.h"
#include "LCD1602.h"
#include "WIFI.h"


uchar data_come = 0; //数据帧开始传输标志
uchar is_Start= 0;   //各个模块开始运行标志
uchar  a[12]="\0\0\0\0\0\0\0\0\0\0\0\0";       //用于存储串口发送的信息
uchar index = 0;
uchar num;
bit flag = 0;
bit bluetooth = 0;
bit refresh = 1;
extern code uchar send1[];
extern code uchar send2[];
void Serial_Timer_Init();

code void (*FUNC[27])() = { &Serial_Timer_Init,
														&_7SEG_LED, 					//1.七段数码管
													  &LED_RUN, 						//2.LED
														&LATTICE_RUN, 				//3.点阵
														&STATIC_DIGITAL_RUN,  //4.静态数码管
														&BUZZER_RUN, 					//5.蜂鸣器
														&DCMOTOR_RUN, 				//6.直流电机
														&STEPMOTOR_RUN, 			//7.步进电机
														&FAN_RUN, 						//8.风扇
														&DS18B20_RUN,    			//9.DS18B20
														&ULTRASONIC_RUN, 			//10.超声波
														&LM393_RUN,						//11.声音传感器
														&DIGITAL_LIGHT_INTENSITY_RUN,	//12.数字光强
														&BLUETOOTH_RUN, 			//13.蓝牙
														&INFRARED_REC_RUN, 		//14.红外接收器
														&ADXL345_RUN, 				//15.加速度传感器
														&ELECTRONIC_SCALE_RUN,//16.压力传感器-电子称
														&MQ_RUN, 							//17.MQ2
														&MQ_RUN, 							//18.MQ3
														&MQ_RUN,							//19.MQ4
														&MQ_RUN, 							//20.MQ5
														&MQ_RUN, 							//21.MQ135
														&HC_HR501_RUN,				//22.人体红外
														&AD_SENSOR_RUN, 			//23.光敏 光敏传感器、湿敏、热敏、振动
														&AD_SENSOR_RUN,				//24.湿敏
														&AD_SENSOR_RUN,				//25.热敏
														&AD_SENSOR_RUN,				//26.振动
														};  //存入code区

void main()
{
	uchar i;
	
	LED = 0;
	BUZZER = 1;
	FAN = 0;
	DCMOTOR = 0;
	hc_hr501 = 1;
	
	lcd1602_init();
	Serial_Timer_Init();
	
	while(1)
	{		
		//AD_SENSOR_RUN();
		//LM393_RUN();
//		num = 12;
//		refresh = 1;
//		DIGITAL_LIGHT_INTENSITY_RUN();
//		Delay_ms(1000);
//		refresh = 1;
//		num = 15;
//		ADXL345_RUN();
//		Delay_ms(1000);
//		refresh = 1;
//		ELECTRONIC_SCALE_RUN(); //电子称
//		Delay_ms(1000);
//		refresh = 1;
//		ULTRASONIC_RUN();
//		Delay_ms(1000);
		//		num = 12;
//		num = 17;
//		refresh = 1;
//		MQ_RUN();
//		Delay_ms(1000);
		
	//	refresh = 1;
		num = 18;
		MQ_RUN();
	//	Delay_ms(1000);
		
//		refresh = 1;
//		num = 19;
//		MQ_RUN(); //电子称
//		Delay_ms(1000);
		
//		refresh = 1;
//		num = 20;
//		MQ_RUN();
//		Delay_ms(1000);
//		if(!((a[0]>>2)%2))  //通信帧
//		{
//			//执行模块
//			for(i = 2;i<a[1]+2;i++)  //一次模块执行
//			{
//				num = a[i];      //保存编号
//				(*FUNC[a[i]])(); //执行函数
//				Delay_ms(5);
//			}
//		}
	}
}

void Serail_Proc() interrupt 4
{
	uchar dat;
	if(RI)
	{
		RI = 0;
		dat = SBUF;
		if(bluetooth) //蓝牙接收
		{
			bluetooth = 0;
			lcd1602_write_string(0,0,"BLUETOOTH RECVE:");  //显示蓝牙模块接收的字符
			lcd1602_write_char(0,1,dat);
			P1 = dat - '0';
		}
		
		
		if(dat == 0)
		{
			refresh = 1;
			data_come = 1;   //标志数据帧的开始
		}
		else if(data_come)
		{
			if(dat == 0XFF)
			{
				index = 0;			//数组下标置0
				data_come = 0; //标志数据帧的结束
			}
			else
			{
				if((index == 0) && (dat>>2)%2) //测试帧
				{
					//刷新LCD
					if(refresh)  //刷新
					{
						refresh = 0;
						lcd1602_write_cmd(0x01);
					}
					if((dat>>1)%2)  //wifi通信 需先初始化
					{
						lcd1602_write_string(0,0,"WIFI TEST");
						Send_String(send1); //发送一个数据
					}
					else
					{
						lcd1602_write_string(0,0,"SERAIL TEST");
					}
					SBUF = '#';
					while(!TI);TI = 0;
					//lcd1602_write_string(0,1,"SUCCESS");
				}
				a[index++] = dat; //数据存入数组
			}
		}
	}
}

void T0_proc() interrupt 1  //超出检测范围或者没有检测到距离则停止计时
{
	flag = 1;
	TH0 = TL0 = 0;
	ET0 = 0;
	TR0 = 0;
}
void Serial_Timer_Init()   	
{
	TMOD = 0x21;  	//T1工作模式2 8位自动重装 T0工作在模式1 
	
	//T1设置
	TH1 = 0xFD;
	TL1 = 0xFD; 	//比特率1200
	TR1 = 1;		  //启动T1定时器
	PCON = 0x00;
	
	//T0设置
	TH0 = TL0 = 0; //定时器0初始化
	
	//串口设置
	SCON = 0x50;
	
	//外部中断设置
	IT0 = 1;    //边沿触发
	
	//中断开关
	EA = 1;
	EX0 = 0; //关掉外部中断0
	ES = 1;
}