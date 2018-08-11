#include "DELAY.h"
#include "reg52.h"
#include "PCF8591.h"
#include "LCD1602.h"
#include "SENSOR.h"
#include "7SEG.h"
#include "WIFI.h"
#include "DS18B20.h"
#include "ELECTRONIC_SCALE.h"
#include "ADXL345.h"
#include "LATTICE.h"
#include "BH1750.h"

extern uchar num;
extern uchar  a[12];       //用于存储串口发送的信息

extern uchar D;  //引用主函数
extern uint d;   //引用DS18B20

extern unsigned long first;
extern bit flag;             //检测距离是否溢出标志
extern bit timer_start;
extern bit bluetooth;
extern bit refresh;
extern uchar BUF[8];                         //接收数据缓存区   
uchar MotorStep = 0;
void SendChar_PC(uchar sd)
{
	lcd1602_write_string(0,0,"PC DISPLAY      ");
	if(!((a[0]>>1)%2)) //通过串口发送
	{
		SBUF = sd;    //发送检测数据
		while(!TI);
		TI = 0; 
	}
	else
	{
		Wifi_Send_Char(sd);
	}
}
void SendInt_PC(uint sd)
{
	lcd1602_write_string(0,0,"PC DISPLAY      ");
	if(!((a[0]>>1)%2)) //通过串口发送
	{
		SBUF = sd>>8;    //发送高八位
		while(!TI);
		TI = 0; 
		SBUF = sd & 0X00FF; //发送低八位
		while(!TI);
		TI = 0; 
	}
	else
	{
		Wifi_Send_Int(sd);
	}
}
void SendString_PC(uchar sd[])
{
	lcd1602_write_string(0,0,"PC DISPLAY      ");
	if(!((a[0]>>1)%2)) //通过串口发送
	{
		uchar i;
		for(i = 0;sd[i] != '\0';i++)   //将数据字符串通过串口发送出去
		{
			SBUF = sd[i];    //发送检测数据
			while(!TI);
			TI = 0; 
		}
	}
	else
	{
		Wifi_Send_String(sd);					//将数据字符串通过wifi发送出去
	}
}
void SendLong_PC(unsigned long sd)
{
	lcd1602_write_string(0,0,"PC DISPLAY      ");
	if(!((a[0]>>1)%2)) //通过串口发送
	{
		SBUF = sd>>24;    //发送检测数据
		while(!TI);
		TI = 0; 
		SBUF = sd>>16;    //发送检测数据
		while(!TI);
		TI = 0; 
		SBUF = sd>>8;    //发送检测数据
		while(!TI);
		TI = 0; 
		SBUF = sd & 0X000000FF;    //发送检测数据
		while(!TI);
		TI = 0; 
	}
	else
	{
		Wifi_Send_Long(sd);					//将数据字符串通过wifi发送出去
	}
}
void _7SEG_LED()									//1.七段数码管
{
	Display_NUM(a[a[1]+2]*100 + a[a[1]+3]); //显示数据
}
void LED_RUN()										//2.LED
{
	P1 = a[a[1]+2];				//可选参数第一个控制LED1灯亮
	SBUF = a[a[1]+2];
}
void LATTICE_RUN()      					//3.点阵
{
	Lattice_Show(a[a[1]+2]);  //显示可选参数第一个数据
}
void STATIC_DIGITAL_RUN()					//4.静态数码管
{
	Display_NUM((a[a[1]+2]*100)<<8 + a[a[1]+3]);
}
void BUZZER_RUN()									//5.蜂鸣器
{
	BUZZER = 0;
	Delay_ms(10);
	BUZZER = 1;
}
void DCMOTOR_RUN()								//6.直流电机
{
	DCMOTOR = 1;
	Delay_ms(10);
	DCMOTOR = 0;
}
void STEPMOTOR_RUN()							//7.步进电机
{
	Delay_us(90);
	switch(MotorStep)
	{
		case 0:			    
			A = 0;		    //0xf1  
			b = 1;
			C = 1;
			DD = 1;
			MotorStep = 1;
			break;
		case 1:		 // AB
			A = 0;		   //0xf3 
			b = 0;
			C = 1;
			DD = 1;
			MotorStep = 2;
			break;
		case 2:	   //B
			A = 1;
			b = 0;		  //0xf2 
			C = 1;
			DD = 1;
			MotorStep = 3;
			break;
		case 3:		//BC
			A = 1;
			b = 0;		    //0xf6
			C = 0;
			DD = 1;
			MotorStep = 4;
			break;
		case 4:		 //C
			A = 1;
			b = 1;		    //0xf4
			C = 0;
			DD = 1;
			MotorStep = 5;
			break;
		case 5:			  //CD
			A = 1;
			b = 1;		    //0xfc
			C = 0;
			DD = 0;
			MotorStep = 6;
			break;
		case 6:			  //D
			A = 1;
			b = 1;		    //0xf8
			C = 1;
			DD = 0;
			MotorStep = 7;
			break;
		case 7:			//DA
			A = 0;
			b = 1;		    //0xf9
			C = 1;
			DD = 0;
			MotorStep = 0;
			break;
	}
}
void FAN_RUN()										//8.风扇
{
	FAN = 1;
	Delay_ms(10);
	FAN = 0;
}
void DS18B20_RUN()       					//9.温度传感器
{
	//刷新LCD
	if(refresh)  //刷新
	{
		refresh = 0;
		lcd1602_write_cmd(0x01);		
		if(!(a[0]%2))  //MCU显示
			lcd1602_write_string(0,0,"Temperature:");
		else
			lcd1602_write_string(0,0,"PC DISPLAY");
	}
	ds_change();
	d = get_temperature();
	if(!(a[0]%2))     //MCU端显示
	{
		Display_Temperature(d);
		if(d>350)
			BUZZER = 0;
		else
			BUZZER = 1;
	}
	else
	{
		//lcd1602_write_string(0,0,"PC DISPLAY");
		SendInt_PC(d);
	}
}


void ULTRASONIC_RUN()    					//10.超声波
{
	//刷新LCD
	if(refresh)  //刷新
	{
		refresh = 0;
		lcd1602_write_cmd(0x01);
		if(!(a[0]%2))  //MCU显示
			lcd1602_write_string(0,0,"Distance:");
		else
			lcd1602_write_string(0,0,"PC DISPLAY");
	}
	trig  = 0;   
	//发出触发信号，开始检测距离
	trig = 1;
	Delay_ms(20);//给最少10us的高电平
	trig = 0;
	echo = 0;
	while(!echo);//echo为高电平时开始接收到超声波返回信号，启动定时T0
	flag = 0;//测距是否溢出标志
	ET0 = TR0 = 1; //打开定时器0和定时器中断
	while(echo);//echo为低电平时停止计数，如果T1发生中断，则无法检测到距离
	ET0 = TR0 = 0;  //关掉定时器0和定时器中断
  
	if(flag == 0)//检测出有距离
	{
		d = ( (256 * TH0 + TL0) * 172) / 10000 + 2;
	}
	if(!(a[0]%2))     //MCU端显示
	{
		lcd1602_write_char(1,1,'0' + d / 100);
		lcd1602_write_char(2,1,'0' + (d % 100) / 10);
		lcd1602_write_char(3,1,'0' + d % 10);
	}
	else
	{
		SendInt_PC(d);
	}	
}



void AD_SENSOR_RUN() 							//11.热敏、光敏、震动、湿敏电阻共用的代码模块  将测出的数值显示在LCD上
{
	//刷新LCD
	if(refresh)  //刷新
	{
		refresh = 0;
		lcd1602_write_cmd(0x01);
	
		//选择通道
		switch(num)
		{
			case 23:
				ISendByte(PCF8591,0x40);  //AIN1
				if(!(a[0]%2))  //MCU显示
					lcd1602_write_string(0,0,"light:");  //光敏
				else
					lcd1602_write_string(0,0,"PC DISPLAY");
				break;
			case 24:
				ISendByte(PCF8591,0x41);  //AIN2
				if(!(a[0]%2))  //MCU显示
					lcd1602_write_string(0,0,"Humidity:");//湿度
				else
					lcd1602_write_string(0,0,"PC DISPLAY");
				break;
			case 25:
				ISendByte(PCF8591,0x42);  //AIN3
				if(!(a[0]%2))  //MCU显示
					lcd1602_write_string(0,0,"Heat:");     //热敏
				else
					lcd1602_write_string(0,0,"PC DISPLAY");
				break;
			case 26:
				ISendByte(PCF8591,0x43);  //AIN4
				if(!(a[0]%2))  //MCU显示
					lcd1602_write_string(0,0,"Vibration:");//振动
				else
					lcd1602_write_string(0,0,"PC DISPLAY");
				break;
			}
	}
	D=IRcvByte(PCF8591);
	if(!(a[0]%2))     //MCU端显示
	{
		//刷新LCD
		
		//lcd1602_write_string(0,0,"The TEMP is: ");//初始化
		lcd1602_write_char(7,1,D/100+'0');//将十位显示在第二行第7列
		lcd1602_write_char(8,1,D%100/10+'0');//将各位显示在第二行第9列
		lcd1602_write_char(9,1,D%10+'0');//将各位显示在第二行第8列
	}
	else
	{
		//lcd1602_write_string(0,0,"PC DISPLAY");
		SendChar_PC(D);
	}
}

void DIGITAL_LIGHT_INTENSITY_RUN()//12.数字光强
{
	//刷新LCD
	if(refresh)  //刷新
	{
		refresh = 0;
		lcd1602_write_cmd(0x01);
		if(!(a[0]%2))  //MCU显示
			lcd1602_write_string(0,0,"Light intensity:");
		else
			lcd1602_write_string(0,0,"PC DISPLAY");
	}
	BH1750_Init();									//初始化BH1750
	BH1750_Read_Multiple();
	if(!(a[0]%2))     //MCU端显示
	{
		BH1750_Display();
	}
	else
	{
		//lcd1602_write_string(0,0,"PC DISPLAY");
		SendString_PC(BUF);
	}
}
void BLUETOOTH_RUN()							//13.蓝牙
{
	bluetooth = 1; //蓝牙模块接收数据
}

void INFRARED_REC_RUN() 					//14.红外接收
{  
	EX0 = 1; //开启外部中断0
}
void IR_IN() interrupt 0 using 0  //14.红外接收器中断
{
  uchar j,k,N=0;
	//刷新LCD
	if(refresh)  //刷新
	{
		refresh = 0;
		lcd1602_write_cmd(0x01);
		if(!(a[0]%2))  //MCU显示
			lcd1602_write_string(0,0,"Infrared receive:");
		else
			lcd1602_write_string(0,0,"PC DISPLAY");
	}
	
	EX0 = 0;    //关掉外部中断0
	Delay_us(200);
	if(IRIN==1) //没有红外发射信号
	{ 
		//EX0 =1;
	  return;
	}     
   
	 //确认IR信号出现
  while(!IRIN)            //等IR变为高电平，跳过9ms的前导低电平信号。
  {
		Delay_us(13);
	}

	for (j=0;j<4;j++)         //收集四组数据 第三组和第四组互为反码 
	{ 
		for (k=0;k<8;k++)        //每组数据有8位
		{
			while(IRIN)            //等 IR 变为低电平，跳过4.5ms的前导高电平信号。
				Delay_us(13);
			
			while(!IRIN)          //等 IR 变为高电平
				Delay_us(13);
			
			while(IRIN)           //计算IR高电平时长 
			{
				Delay_us(13);
				N++;           
				if(N>=30)   					//0.14ms计数过长自动离开。
				{    
					//EX0=1;
					return;
				}                 
			}                          //高电平计数完毕                
			BUF[j]=BUF[j] >> 1;                  //数据最高位补“0”
			if (N>=8) {BUF[j] = BUF[j] | 0x80;}  //数据最高位补“1”
			N=0;
		}//end for k
	}//end for j
   
	if(BUF[2]!=~BUF[3])   //解码失败
	{ 
		//EX0=1;
		return;
	}

	BUF[5]=BUF[2] & 0x0F;     //取键码的低四位
	BUF[6]=BUF[2] >> 4;       //右移4次，高四位变为低四位

	if(BUF[5]>9)
		BUF[5]=BUF[5]+0x37;
	else
		BUF[5]=BUF[5]+0x30;

	if(BUF[6]>9)
	  BUF[6]=BUF[6]+0x37;
	else
		BUF[6]=BUF[6]+0x30;
	BUF[7] = '\0';		//字符串结尾
	if(!(a[0]%2))     //MCU端显示
	{
		lcd1602_write_char(2,0,BUF[6]);        //第一位数显示         
		lcd1602_write_char(3,0,BUF[5]);        //第二位数显示
	}
	else
	{
		//lcd1602_write_string(0,0,"PC DISPLAY");
		SendString_PC(BUF);	//PC端显示
	}
} 

void ADXL345_RUN()  							//15.三轴加速度
{
	//刷新LCD
	if(refresh)  //刷新
	{
		refresh = 0;
		lcd1602_write_cmd(0x01);
		if(!(a[0]%2))  //MCU显示
			lcd1602_write_string(0,0,"ADXL345 ");
		else
			lcd1602_write_string(0,0,"PC DISPLAY");
	}
	ADXL345_Init();                 //初始化ADXL345
	ADXL345_Read_Multiple();       	//连续读出数据，存储在BUF中
	if(!(a[0]%2))     //MCU端显示
	{
		ADXL345_Display_xyz();					//显示ADXL345测量数据
	}
	else
	{
		//lcd1602_write_string(0,0,"PC DISPLAY");
		SendString_PC(BUF);
	}
}
void ELECTRONIC_SCALE_RUN() 			//16.电子称
{
	unsigned long ed;
	unsigned long cd;
	//刷新LCD
	if(refresh)  //刷新
	{
		refresh = 0;
		lcd1602_write_cmd(0x01);
		if(!(a[0]%2))  //MCU显示
			lcd1602_write_string(0,0,"Weight:");
		else
			lcd1602_write_string(0,0,"PC DISPLAY");
	}
	ed = ElectronicScalse_ReadCount();
	cd = ed;
	ed /= 100;
	cd = ed/7.16+0.05;
	cd -= first; 
	//cd -= 11864;
//	unsigned long ed;
//	unsigned long cd;
//	ed = ElectronicScalse_ReadCount();
//	cd = ed;
//	ed /= 100;
//	cd = ed/7.16+0.05;
//	cd -= first;
//	Display_NUM(cd);
//	Display_NUM(cd);
//	Display_NUM(cd);
//	Display_NUM(cd);
//	Display_NUM(cd);
//	Display_NUM(cd);
//	Display_NUM(cd);
//	Display_NUM(cd);
	
	if(!(a[0]%2))     //MCU端显示
	{
		cd = cd%100000;
		lcd1602_write_char(0,1,cd/10000 + '0'); //显示数据 wan
		cd = cd%10000;
		lcd1602_write_char(1,1,cd/1000 + '0');  //qian
		cd = cd%1000;	
		lcd1602_write_char(2,1,cd/100 + '0');   //bai
		cd = cd%100;
		lcd1602_write_char(3,1,cd/10 + '0');   //shi
		lcd1602_write_char(4,1,cd%10 + '0');    //ge
	}
	else
	{
		//lcd1602_write_string(0,0,"PC DISPLAY");
		SendLong_PC(cd);
	}
}

void MQ_RUN()											//17 18 19 20 21 气体、烟雾传感器
{
	//刷新LCD
	if(refresh)  //刷新
	{
		refresh = 0;
		lcd1602_write_cmd(0x01);
		//选择通道
		switch(num)
		{
			case 17:
				//ISendByte(PCF8591,0x40);  //AIN1
				if(!(a[0]%2))  //MCU显示
					lcd1602_write_string(0,0,"**MQ2   ");
				else
					lcd1602_write_string(0,0,"PC DISPLAY");
				break;
			case 18:
				//ISendByte(PCF8591,0x41); //AIN2
				if(!(a[0]%2))  //MCU显示
					lcd1602_write_string(0,0,"**MQ3   ");
				else
					lcd1602_write_string(0,0,"PC DISPLAY");
				break;
			case 19:
				//ISendByte(PCF8591,0x42); //AIN3
				if(!(a[0]%2))  //MCU显示
					lcd1602_write_string(0,0,"**MQ4   ");
				else
					lcd1602_write_string(0,0,"PC DISPLAY");
				break;
			case 20:
				//ISendByte(PCF8591,0x43); //AIN4
				if(!(a[0]%2))  //MCU显示
					lcd1602_write_string(0,0,"**MQ5   ");
				else
					lcd1602_write_string(0,0,"PC DISPLAY");
				break;
			case 21:
				//ISendByte(PCF8591,0x40);  //AIN1
				if(!(a[0]%2))  //MCU显示
					lcd1602_write_string(0,0,"**MQ135   ");
				else
					lcd1602_write_string(0,0,"PC DISPLAY");
				break;
		}
	}
	switch(num)
	{
		case 17:
			ISendByte(PCF8591,0x40);  //AIN1
			break;
		case 18:
			ISendByte(PCF8591,0x41); //AIN2
			break;
		case 19:
			ISendByte(PCF8591,0x42); //AIN3
			break;
		case 20:
			ISendByte(PCF8591,0x43); //AIN4
			break;
		case 21:
			ISendByte(PCF8591,0x40);  //AIN1
			break;
	}
	D=IRcvByte(PCF8591);  //读取模拟转换数据
	if(!(a[0]%2))     //MCU端显示
	{ 
		BUF[0]=D/100+'0';
		BUF[1]=(D%100)/10+'0';
		BUF[2]=D%10+'0';
		BUF[3] = '\0';
		lcd1602_write_string(8,0,"DATA:");
		lcd1602_write_string(13,0,BUF);
		
		if(D>40)
		{
			lcd1602_write_string(0,1,"WARNING         ");
			LED = 1;   //LED亮
			BUZZER = 0;//蜂鸣器响
		}
		else
		{
			lcd1602_write_string(0,1,"        ");
			LED = 0;
			BUZZER = 1;
		}
	}
	else					 //PC端显示
	{
		//cd1602_write_string(0,0,"PC DISPLAY");
		SendChar_PC(D); //数据发至PC端
	}
}

void HC_HR501_RUN()								//22.人体红外
{
	Delay_ms(10);
	if(hc_hr501 == 0)  //有人
	{
		hc_hr501 = 1;
		BUZZER = 0;  //蜂鸣器响
		Delay_ms(20);
		BUZZER = 1;
		Delay_ms(20);
	}
	else
	{
		BUZZER = 1;
	}
}

void LM393_RUN()									//23.声音传感器
{
	uchar i,get_first;
	//刷新LCD
	if(refresh)  //刷新
	{
		refresh = 0;
		lcd1602_write_cmd(0x01);
		if(!(a[0]%2))  //MCU显示
			lcd1602_write_string(0,0,"Voice:");
		else
			lcd1602_write_string(0,0,"PC DISPLAY");
	}
	for(i = 0;i<10;i++)   //声音传感器初始化
	{
		ISendByte(PCF8591,0x41);  //AIN1
		get_first = IRcvByte(PCF8591);
	}
	D = IRcvByte(PCF8591);
	for(i = 0;i<4;i++)
	{
		BUF[i] = D%10+'0';
		D /= 10;
	}
	if(!(a[0]%2))     //MCU端显示
	{
		
		if(D != ~get_first)
		{	
			lcd1602_write_string(6,0,BUF);//显示声音的值
		}
		else
		{
			lcd1602_write_string(0,0,"QUIET     ");
		}
	}
	else					 //PC端显示
	{
		//lcd1602_write_string(0,0,"PC DISPLAY");
		SendChar_PC(D); //数据发至PC端
	}
}


