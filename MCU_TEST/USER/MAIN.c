#include "reg52.h"
#include "DEFINE.h"
#include "SENSOR.h"
#include "Delay.h"
#include "string.h"
#include "LCD1602.h"
#include "WIFI.h"


uchar data_come = 0; //����֡��ʼ�����־
uchar is_Start= 0;   //����ģ�鿪ʼ���б�־
uchar  a[12]="\0\0\0\0\0\0\0\0\0\0\0\0";       //���ڴ洢���ڷ��͵���Ϣ
uchar index = 0;
uchar num;
bit flag = 0;
bit bluetooth = 0;
bit refresh = 1;
extern code uchar send1[];
extern code uchar send2[];
void Serial_Timer_Init();

code void (*FUNC[27])() = { &Serial_Timer_Init,
														&_7SEG_LED, 					//1.�߶������
													  &LED_RUN, 						//2.LED
														&LATTICE_RUN, 				//3.����
														&STATIC_DIGITAL_RUN,  //4.��̬�����
														&BUZZER_RUN, 					//5.������
														&DCMOTOR_RUN, 				//6.ֱ�����
														&STEPMOTOR_RUN, 			//7.�������
														&FAN_RUN, 						//8.����
														&DS18B20_RUN,    			//9.DS18B20
														&ULTRASONIC_RUN, 			//10.������
														&LM393_RUN,						//11.����������
														&DIGITAL_LIGHT_INTENSITY_RUN,	//12.���ֹ�ǿ
														&BLUETOOTH_RUN, 			//13.����
														&INFRARED_REC_RUN, 		//14.���������
														&ADXL345_RUN, 				//15.���ٶȴ�����
														&ELECTRONIC_SCALE_RUN,//16.ѹ��������-���ӳ�
														&MQ_RUN, 							//17.MQ2
														&MQ_RUN, 							//18.MQ3
														&MQ_RUN,							//19.MQ4
														&MQ_RUN, 							//20.MQ5
														&MQ_RUN, 							//21.MQ135
														&HC_HR501_RUN,				//22.�������
														&AD_SENSOR_RUN, 			//23.���� ������������ʪ������������
														&AD_SENSOR_RUN,				//24.ʪ��
														&AD_SENSOR_RUN,				//25.����
														&AD_SENSOR_RUN,				//26.��
														};  //����code��

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
//		ELECTRONIC_SCALE_RUN(); //���ӳ�
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
//		MQ_RUN(); //���ӳ�
//		Delay_ms(1000);
		
//		refresh = 1;
//		num = 20;
//		MQ_RUN();
//		Delay_ms(1000);
//		if(!((a[0]>>2)%2))  //ͨ��֡
//		{
//			//ִ��ģ��
//			for(i = 2;i<a[1]+2;i++)  //һ��ģ��ִ��
//			{
//				num = a[i];      //������
//				(*FUNC[a[i]])(); //ִ�к���
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
		if(bluetooth) //��������
		{
			bluetooth = 0;
			lcd1602_write_string(0,0,"BLUETOOTH RECVE:");  //��ʾ����ģ����յ��ַ�
			lcd1602_write_char(0,1,dat);
			P1 = dat - '0';
		}
		
		
		if(dat == 0)
		{
			refresh = 1;
			data_come = 1;   //��־����֡�Ŀ�ʼ
		}
		else if(data_come)
		{
			if(dat == 0XFF)
			{
				index = 0;			//�����±���0
				data_come = 0; //��־����֡�Ľ���
			}
			else
			{
				if((index == 0) && (dat>>2)%2) //����֡
				{
					//ˢ��LCD
					if(refresh)  //ˢ��
					{
						refresh = 0;
						lcd1602_write_cmd(0x01);
					}
					if((dat>>1)%2)  //wifiͨ�� ���ȳ�ʼ��
					{
						lcd1602_write_string(0,0,"WIFI TEST");
						Send_String(send1); //����һ������
					}
					else
					{
						lcd1602_write_string(0,0,"SERAIL TEST");
					}
					SBUF = '#';
					while(!TI);TI = 0;
					//lcd1602_write_string(0,1,"SUCCESS");
				}
				a[index++] = dat; //���ݴ�������
			}
		}
	}
}

void T0_proc() interrupt 1  //������ⷶΧ����û�м�⵽������ֹͣ��ʱ
{
	flag = 1;
	TH0 = TL0 = 0;
	ET0 = 0;
	TR0 = 0;
}
void Serial_Timer_Init()   	
{
	TMOD = 0x21;  	//T1����ģʽ2 8λ�Զ���װ T0������ģʽ1 
	
	//T1����
	TH1 = 0xFD;
	TL1 = 0xFD; 	//������1200
	TR1 = 1;		  //����T1��ʱ��
	PCON = 0x00;
	
	//T0����
	TH0 = TL0 = 0; //��ʱ��0��ʼ��
	
	//��������
	SCON = 0x50;
	
	//�ⲿ�ж�����
	IT0 = 1;    //���ش���
	
	//�жϿ���
	EA = 1;
	EX0 = 0; //�ص��ⲿ�ж�0
	ES = 1;
}