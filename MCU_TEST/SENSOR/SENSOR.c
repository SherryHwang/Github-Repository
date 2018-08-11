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
extern uchar  a[12];       //���ڴ洢���ڷ��͵���Ϣ

extern uchar D;  //����������
extern uint d;   //����DS18B20

extern unsigned long first;
extern bit flag;             //�������Ƿ������־
extern bit timer_start;
extern bit bluetooth;
extern bit refresh;
extern uchar BUF[8];                         //�������ݻ�����   
uchar MotorStep = 0;
void SendChar_PC(uchar sd)
{
	lcd1602_write_string(0,0,"PC DISPLAY      ");
	if(!((a[0]>>1)%2)) //ͨ�����ڷ���
	{
		SBUF = sd;    //���ͼ������
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
	if(!((a[0]>>1)%2)) //ͨ�����ڷ���
	{
		SBUF = sd>>8;    //���͸߰�λ
		while(!TI);
		TI = 0; 
		SBUF = sd & 0X00FF; //���͵Ͱ�λ
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
	if(!((a[0]>>1)%2)) //ͨ�����ڷ���
	{
		uchar i;
		for(i = 0;sd[i] != '\0';i++)   //�������ַ���ͨ�����ڷ��ͳ�ȥ
		{
			SBUF = sd[i];    //���ͼ������
			while(!TI);
			TI = 0; 
		}
	}
	else
	{
		Wifi_Send_String(sd);					//�������ַ���ͨ��wifi���ͳ�ȥ
	}
}
void SendLong_PC(unsigned long sd)
{
	lcd1602_write_string(0,0,"PC DISPLAY      ");
	if(!((a[0]>>1)%2)) //ͨ�����ڷ���
	{
		SBUF = sd>>24;    //���ͼ������
		while(!TI);
		TI = 0; 
		SBUF = sd>>16;    //���ͼ������
		while(!TI);
		TI = 0; 
		SBUF = sd>>8;    //���ͼ������
		while(!TI);
		TI = 0; 
		SBUF = sd & 0X000000FF;    //���ͼ������
		while(!TI);
		TI = 0; 
	}
	else
	{
		Wifi_Send_Long(sd);					//�������ַ���ͨ��wifi���ͳ�ȥ
	}
}
void _7SEG_LED()									//1.�߶������
{
	Display_NUM(a[a[1]+2]*100 + a[a[1]+3]); //��ʾ����
}
void LED_RUN()										//2.LED
{
	P1 = a[a[1]+2];				//��ѡ������һ������LED1����
	SBUF = a[a[1]+2];
}
void LATTICE_RUN()      					//3.����
{
	Lattice_Show(a[a[1]+2]);  //��ʾ��ѡ������һ������
}
void STATIC_DIGITAL_RUN()					//4.��̬�����
{
	Display_NUM((a[a[1]+2]*100)<<8 + a[a[1]+3]);
}
void BUZZER_RUN()									//5.������
{
	BUZZER = 0;
	Delay_ms(10);
	BUZZER = 1;
}
void DCMOTOR_RUN()								//6.ֱ�����
{
	DCMOTOR = 1;
	Delay_ms(10);
	DCMOTOR = 0;
}
void STEPMOTOR_RUN()							//7.�������
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
void FAN_RUN()										//8.����
{
	FAN = 1;
	Delay_ms(10);
	FAN = 0;
}
void DS18B20_RUN()       					//9.�¶ȴ�����
{
	//ˢ��LCD
	if(refresh)  //ˢ��
	{
		refresh = 0;
		lcd1602_write_cmd(0x01);		
		if(!(a[0]%2))  //MCU��ʾ
			lcd1602_write_string(0,0,"Temperature:");
		else
			lcd1602_write_string(0,0,"PC DISPLAY");
	}
	ds_change();
	d = get_temperature();
	if(!(a[0]%2))     //MCU����ʾ
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


void ULTRASONIC_RUN()    					//10.������
{
	//ˢ��LCD
	if(refresh)  //ˢ��
	{
		refresh = 0;
		lcd1602_write_cmd(0x01);
		if(!(a[0]%2))  //MCU��ʾ
			lcd1602_write_string(0,0,"Distance:");
		else
			lcd1602_write_string(0,0,"PC DISPLAY");
	}
	trig  = 0;   
	//���������źţ���ʼ������
	trig = 1;
	Delay_ms(20);//������10us�ĸߵ�ƽ
	trig = 0;
	echo = 0;
	while(!echo);//echoΪ�ߵ�ƽʱ��ʼ���յ������������źţ�������ʱT0
	flag = 0;//����Ƿ������־
	ET0 = TR0 = 1; //�򿪶�ʱ��0�Ͷ�ʱ���ж�
	while(echo);//echoΪ�͵�ƽʱֹͣ���������T1�����жϣ����޷���⵽����
	ET0 = TR0 = 0;  //�ص���ʱ��0�Ͷ�ʱ���ж�
  
	if(flag == 0)//�����о���
	{
		d = ( (256 * TH0 + TL0) * 172) / 10000 + 2;
	}
	if(!(a[0]%2))     //MCU����ʾ
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



void AD_SENSOR_RUN() 							//11.�������������𶯡�ʪ�����蹲�õĴ���ģ��  ���������ֵ��ʾ��LCD��
{
	//ˢ��LCD
	if(refresh)  //ˢ��
	{
		refresh = 0;
		lcd1602_write_cmd(0x01);
	
		//ѡ��ͨ��
		switch(num)
		{
			case 23:
				ISendByte(PCF8591,0x40);  //AIN1
				if(!(a[0]%2))  //MCU��ʾ
					lcd1602_write_string(0,0,"light:");  //����
				else
					lcd1602_write_string(0,0,"PC DISPLAY");
				break;
			case 24:
				ISendByte(PCF8591,0x41);  //AIN2
				if(!(a[0]%2))  //MCU��ʾ
					lcd1602_write_string(0,0,"Humidity:");//ʪ��
				else
					lcd1602_write_string(0,0,"PC DISPLAY");
				break;
			case 25:
				ISendByte(PCF8591,0x42);  //AIN3
				if(!(a[0]%2))  //MCU��ʾ
					lcd1602_write_string(0,0,"Heat:");     //����
				else
					lcd1602_write_string(0,0,"PC DISPLAY");
				break;
			case 26:
				ISendByte(PCF8591,0x43);  //AIN4
				if(!(a[0]%2))  //MCU��ʾ
					lcd1602_write_string(0,0,"Vibration:");//��
				else
					lcd1602_write_string(0,0,"PC DISPLAY");
				break;
			}
	}
	D=IRcvByte(PCF8591);
	if(!(a[0]%2))     //MCU����ʾ
	{
		//ˢ��LCD
		
		//lcd1602_write_string(0,0,"The TEMP is: ");//��ʼ��
		lcd1602_write_char(7,1,D/100+'0');//��ʮλ��ʾ�ڵڶ��е�7��
		lcd1602_write_char(8,1,D%100/10+'0');//����λ��ʾ�ڵڶ��е�9��
		lcd1602_write_char(9,1,D%10+'0');//����λ��ʾ�ڵڶ��е�8��
	}
	else
	{
		//lcd1602_write_string(0,0,"PC DISPLAY");
		SendChar_PC(D);
	}
}

void DIGITAL_LIGHT_INTENSITY_RUN()//12.���ֹ�ǿ
{
	//ˢ��LCD
	if(refresh)  //ˢ��
	{
		refresh = 0;
		lcd1602_write_cmd(0x01);
		if(!(a[0]%2))  //MCU��ʾ
			lcd1602_write_string(0,0,"Light intensity:");
		else
			lcd1602_write_string(0,0,"PC DISPLAY");
	}
	BH1750_Init();									//��ʼ��BH1750
	BH1750_Read_Multiple();
	if(!(a[0]%2))     //MCU����ʾ
	{
		BH1750_Display();
	}
	else
	{
		//lcd1602_write_string(0,0,"PC DISPLAY");
		SendString_PC(BUF);
	}
}
void BLUETOOTH_RUN()							//13.����
{
	bluetooth = 1; //����ģ���������
}

void INFRARED_REC_RUN() 					//14.�������
{  
	EX0 = 1; //�����ⲿ�ж�0
}
void IR_IN() interrupt 0 using 0  //14.����������ж�
{
  uchar j,k,N=0;
	//ˢ��LCD
	if(refresh)  //ˢ��
	{
		refresh = 0;
		lcd1602_write_cmd(0x01);
		if(!(a[0]%2))  //MCU��ʾ
			lcd1602_write_string(0,0,"Infrared receive:");
		else
			lcd1602_write_string(0,0,"PC DISPLAY");
	}
	
	EX0 = 0;    //�ص��ⲿ�ж�0
	Delay_us(200);
	if(IRIN==1) //û�к��ⷢ���ź�
	{ 
		//EX0 =1;
	  return;
	}     
   
	 //ȷ��IR�źų���
  while(!IRIN)            //��IR��Ϊ�ߵ�ƽ������9ms��ǰ���͵�ƽ�źš�
  {
		Delay_us(13);
	}

	for (j=0;j<4;j++)         //�ռ��������� ������͵����黥Ϊ���� 
	{ 
		for (k=0;k<8;k++)        //ÿ��������8λ
		{
			while(IRIN)            //�� IR ��Ϊ�͵�ƽ������4.5ms��ǰ���ߵ�ƽ�źš�
				Delay_us(13);
			
			while(!IRIN)          //�� IR ��Ϊ�ߵ�ƽ
				Delay_us(13);
			
			while(IRIN)           //����IR�ߵ�ƽʱ�� 
			{
				Delay_us(13);
				N++;           
				if(N>=30)   					//0.14ms���������Զ��뿪��
				{    
					//EX0=1;
					return;
				}                 
			}                          //�ߵ�ƽ�������                
			BUF[j]=BUF[j] >> 1;                  //�������λ����0��
			if (N>=8) {BUF[j] = BUF[j] | 0x80;}  //�������λ����1��
			N=0;
		}//end for k
	}//end for j
   
	if(BUF[2]!=~BUF[3])   //����ʧ��
	{ 
		//EX0=1;
		return;
	}

	BUF[5]=BUF[2] & 0x0F;     //ȡ����ĵ���λ
	BUF[6]=BUF[2] >> 4;       //����4�Σ�����λ��Ϊ����λ

	if(BUF[5]>9)
		BUF[5]=BUF[5]+0x37;
	else
		BUF[5]=BUF[5]+0x30;

	if(BUF[6]>9)
	  BUF[6]=BUF[6]+0x37;
	else
		BUF[6]=BUF[6]+0x30;
	BUF[7] = '\0';		//�ַ�����β
	if(!(a[0]%2))     //MCU����ʾ
	{
		lcd1602_write_char(2,0,BUF[6]);        //��һλ����ʾ         
		lcd1602_write_char(3,0,BUF[5]);        //�ڶ�λ����ʾ
	}
	else
	{
		//lcd1602_write_string(0,0,"PC DISPLAY");
		SendString_PC(BUF);	//PC����ʾ
	}
} 

void ADXL345_RUN()  							//15.������ٶ�
{
	//ˢ��LCD
	if(refresh)  //ˢ��
	{
		refresh = 0;
		lcd1602_write_cmd(0x01);
		if(!(a[0]%2))  //MCU��ʾ
			lcd1602_write_string(0,0,"ADXL345 ");
		else
			lcd1602_write_string(0,0,"PC DISPLAY");
	}
	ADXL345_Init();                 //��ʼ��ADXL345
	ADXL345_Read_Multiple();       	//�����������ݣ��洢��BUF��
	if(!(a[0]%2))     //MCU����ʾ
	{
		ADXL345_Display_xyz();					//��ʾADXL345��������
	}
	else
	{
		//lcd1602_write_string(0,0,"PC DISPLAY");
		SendString_PC(BUF);
	}
}
void ELECTRONIC_SCALE_RUN() 			//16.���ӳ�
{
	unsigned long ed;
	unsigned long cd;
	//ˢ��LCD
	if(refresh)  //ˢ��
	{
		refresh = 0;
		lcd1602_write_cmd(0x01);
		if(!(a[0]%2))  //MCU��ʾ
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
	
	if(!(a[0]%2))     //MCU����ʾ
	{
		cd = cd%100000;
		lcd1602_write_char(0,1,cd/10000 + '0'); //��ʾ���� wan
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

void MQ_RUN()											//17 18 19 20 21 ���塢��������
{
	//ˢ��LCD
	if(refresh)  //ˢ��
	{
		refresh = 0;
		lcd1602_write_cmd(0x01);
		//ѡ��ͨ��
		switch(num)
		{
			case 17:
				//ISendByte(PCF8591,0x40);  //AIN1
				if(!(a[0]%2))  //MCU��ʾ
					lcd1602_write_string(0,0,"**MQ2   ");
				else
					lcd1602_write_string(0,0,"PC DISPLAY");
				break;
			case 18:
				//ISendByte(PCF8591,0x41); //AIN2
				if(!(a[0]%2))  //MCU��ʾ
					lcd1602_write_string(0,0,"**MQ3   ");
				else
					lcd1602_write_string(0,0,"PC DISPLAY");
				break;
			case 19:
				//ISendByte(PCF8591,0x42); //AIN3
				if(!(a[0]%2))  //MCU��ʾ
					lcd1602_write_string(0,0,"**MQ4   ");
				else
					lcd1602_write_string(0,0,"PC DISPLAY");
				break;
			case 20:
				//ISendByte(PCF8591,0x43); //AIN4
				if(!(a[0]%2))  //MCU��ʾ
					lcd1602_write_string(0,0,"**MQ5   ");
				else
					lcd1602_write_string(0,0,"PC DISPLAY");
				break;
			case 21:
				//ISendByte(PCF8591,0x40);  //AIN1
				if(!(a[0]%2))  //MCU��ʾ
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
	D=IRcvByte(PCF8591);  //��ȡģ��ת������
	if(!(a[0]%2))     //MCU����ʾ
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
			LED = 1;   //LED��
			BUZZER = 0;//��������
		}
		else
		{
			lcd1602_write_string(0,1,"        ");
			LED = 0;
			BUZZER = 1;
		}
	}
	else					 //PC����ʾ
	{
		//cd1602_write_string(0,0,"PC DISPLAY");
		SendChar_PC(D); //���ݷ���PC��
	}
}

void HC_HR501_RUN()								//22.�������
{
	Delay_ms(10);
	if(hc_hr501 == 0)  //����
	{
		hc_hr501 = 1;
		BUZZER = 0;  //��������
		Delay_ms(20);
		BUZZER = 1;
		Delay_ms(20);
	}
	else
	{
		BUZZER = 1;
	}
}

void LM393_RUN()									//23.����������
{
	uchar i,get_first;
	//ˢ��LCD
	if(refresh)  //ˢ��
	{
		refresh = 0;
		lcd1602_write_cmd(0x01);
		if(!(a[0]%2))  //MCU��ʾ
			lcd1602_write_string(0,0,"Voice:");
		else
			lcd1602_write_string(0,0,"PC DISPLAY");
	}
	for(i = 0;i<10;i++)   //������������ʼ��
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
	if(!(a[0]%2))     //MCU����ʾ
	{
		
		if(D != ~get_first)
		{	
			lcd1602_write_string(6,0,BUF);//��ʾ������ֵ
		}
		else
		{
			lcd1602_write_string(0,0,"QUIET     ");
		}
	}
	else					 //PC����ʾ
	{
		//lcd1602_write_string(0,0,"PC DISPLAY");
		SendChar_PC(D); //���ݷ���PC��
	}
}


