#include "DELAY.h"
#include "reg52.h"
#include "PCF8591.h"
#include "LCD1602.h"
#include "SENSOR.h"

sbit LED = P1 ^ 0;				 		
sbit BUZZER = P1 ^ 1;	
sbit FAN = P1 ^ 2;	         //����
sbit Dcmotor = P1 ^ 3;         //ֱ�����

void DCMOTOR_RUN()           //ֱ�����ת��
{
	Dcmotor = 1;
}

void MQ2_RUN()  //����MQ2���ݲ���ʾ  ����
{
	uchar D = 0;
	D=IRcvByte(PCF8591);  //��ȡģ��ת������
	if(D>60)
	{
		lcd1602_write_string(0,1,"WARNING         ");
		lcd1602_write_string(0,1,"FROM Q2");
		LED = 1;//LED��
		FAN=1;  //����ת
		
		BUZZER = 0;//��������
		Delay(100);
		BUZZER = 1;//����������
		Delay(100);
		BUZZER = 0;//��������
		Delay(100);
		BUZZER = 1;//����������		
	}
	else
	{
		FAN=0;     //���Ȳ�ת
		LED = 0;   //LED�Ʋ���
		BUZZER = 1; //����������
		lcd1602_write_string(0,1,"THERE IS SAFE");
	}
	SBUF = D;
	while(!TI);
	TI = 0;   //��ֵ������PC
}

void MQ3_RUN()  //����MQ3���ݲ���ʾ  �ƾ�
{
	uchar D = 0;
	D=IRcvByte(PCF8591);  //��ȡģ��ת������
	if(D>40)
	{
		lcd1602_write_string(0,0,"WARNING         ");
		lcd1602_write_string(0,1,"FROM Q3");
		LED = 1;//LED��
		BUZZER = 0;//��������
		Delay(100);
		BUZZER = 1;//����������
		Delay(100);
		BUZZER = 0;//��������
		Delay(100);
		BUZZER = 1;//����������
	}
	else
	{
		LED = 0;
		BUZZER = 1;
		lcd1602_write_string(0,0,"THERE IS OK");
	}
	
	//��ֵ������PC
	SBUF = D;
	while(!TI);
	TI = 0;  
}

void MQ4_RUN()  //����MQ4���ݲ���ʾ  ��Ȼ��
{
	uchar D = 0;
	D=IRcvByte(PCF8591);  //��ȡģ��ת������
	if(D>40)
	{
		lcd1602_write_string(0,0,"WARNING         ");
		lcd1602_write_string(0,1,"FROM Q3");
		LED = 1;//LED��
		BUZZER = 0;//��������
		Delay(100);
		BUZZER = 1;//����������
		Delay(100);
		BUZZER = 0;//��������
		Delay(100);
		BUZZER = 1;//����������
	}
	else
	{
		LED = 0;
		BUZZER = 1;
		lcd1602_write_string(0,0,"THERE IS OK");
	}
	
	//��ֵ������PC
	SBUF = D;
	while(!TI);
	TI = 0;  
}

void MQ5_RUN()  //����MQ5���ݲ���ʾ  Һ��
{
	uchar D = 0;
	D=IRcvByte(PCF8591);  //��ȡģ��ת������
	if(D>40)
	{
		lcd1602_write_string(0,0,"WARNING         ");
		lcd1602_write_string(0,1,"FROM Q3");
		LED = 1;//LED��
		BUZZER = 0;//��������
		Delay(100);
		BUZZER = 1;//����������
		Delay(100);
		BUZZER = 0;//��������
		Delay(100);
		BUZZER = 1;//����������
	}
	else
	{
		LED = 0;
		BUZZER = 1;
		lcd1602_write_string(0,0,"THERE IS OK");
	}
	
	//��ֵ������PC
	SBUF = D;
	while(!TI);
	TI = 0;  
}

void MQ135_RUN()  //����MQ135���ݲ���ʾ Һ��
{
	uchar D = 0;
	D=IRcvByte(PCF8591);  //��ȡģ��ת������
	if(D>40)
	{
		lcd1602_write_string(0,0,"WARNING         ");
		lcd1602_write_string(0,1,"FROM Q3");
		LED = 1;//LED��
		BUZZER = 0;//��������
		Delay(100);
		BUZZER = 1;//����������
		Delay(100);
		BUZZER = 0;//��������
		Delay(100);
		BUZZER = 1;//����������
	}
	else
	{
		LED = 0;
		BUZZER = 1;
		lcd1602_write_string(0,0,"THERE IS OK");
	}
	
	//��ֵ������PC
	SBUF = D;
	while(!TI);
	TI = 0;  
}


void SENSOR_RUN()
{
	DCMOTOR_RUN();//ֱ�����
	MQ2_RUN();    //MQ2
	MQ3_RUN();    //MQ3
	MQ4_RUN();    //MQ4
	MQ5_RUN();    //MQ5
	MQ135_RUN();  //MQ135
}
