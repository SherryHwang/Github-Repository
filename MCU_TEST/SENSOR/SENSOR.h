#ifndef __SENSOR_H__
#define __SENSOR_H__
#include "DEFINE.h"
sbit LED = P1 ^ 0;			  	 		
sbit BUZZER = P1 ^ 1;	
sbit FAN = P1 ^ 2;	         //����
sbit DCMOTOR = P1 ^ 3;       //ֱ�����
sbit STEPMOTOR_CP = P1^4;    //��������������
sbit STEPMOTOR_DIR = P1^5;   //��������������

sbit A = P1^4;	  
sbit b = P1^5;
sbit C = P1^6;
sbit DD = P1^7;

sbit hc_hr501 = P3^6;        //�������

//������ģ�����Ŷ���
sbit trig = P3^5;//�����ź�
sbit echo = P3^4;//�����ź�

sbit IRIN = P3^2;         //��������������� �ⲿ�ж�0

void SendChar_PC(uchar sd);
void SendInt_PC(uint sd);
void SendLong_PC(unsigned long sd);
void SendString_PC(uchar sd[]);
void _7SEG_LED();						//1.�߶������
void LED_RUN();							//2.LED
void LATTICE_RUN();      		//3.����
void STATIC_DIGITAL_RUN();  //4.��̬�����
void BUZZER_RUN();					//5.������
void DCMOTOR_RUN();					//6.ֱ�����
void STEPMOTOR_RUN();				//7.�������
void FAN_RUN();							//8.����
void DS18B20_RUN();					//9.�¶ȴ�����
void ULTRASONIC_RUN();			//10.������
void AD_SENSOR_RUN();       //11.������������ʪ������������
void DIGITAL_LIGHT_INTENSITY_RUN();//12.���ֹ�ǿ
void BLUETOOTH_RUN();				//13.����
void INFRARED_REC_RUN();		//14.�������
void ADXL345_RUN();  			  //15.������ٶ�
void ELECTRONIC_SCALE_RUN();//16.���ӳ�
void MQ_RUN();              //17 18 19 20 21 ���塢��������
void HC_HR501_RUN();				//22.�������
void LM393_RUN();						//23.����������
void Display(uint distance);//��ʾ����  -LCD��ʾ�������������

#endif 