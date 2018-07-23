#include "reg52.h"
#include "SENSOR.h"
#include "Delay.h"
#include "string.h"

uchar data_come = 0; //����֡��ʼ�����־
uchar data_flag = 0; //֡����
uchar is_Start= 0;   //����ģ�鿪ʼ���б�־
uchar num = 0;
uchar  a[10];       //���ڴ洢���ڷ��͵���Ϣ
uchar index = 0;
void SerialInit()   	
{
	TMOD = 0x20;  	//T1����ģʽ2  8λ�Զ���װ
	TH1 = 0xfd;
	TL1 = 0xfd; 	//������9600
	TR1 = 1;		//����T1��ʱ��
	SM0 = 0;
	SM1 = 1; 		//���ڹ�����ʽ1 10λ�첽
	REN = 1;		//�����������
	EA  = 1;		//�����ж�
	ES  = 1;		//�����жϴ�
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
			if(data_come && !data_flag)  //�ڶ�λ����Ϊ0  ��ֹͣ֡
			{
				data_come = 0;
				is_Start = 0;
			}
			else if(!data_come)
				data_come = 1;
		}
		else
		{
			data_flag = 1;    //ͨѶ֡
			if(data_come)
				is_Start = 1;
		}
		
		if(is_Start)
		{
			
			if(num != 0xFF)   //�������һ������
			{
				a[index++] = num;  //���ݴ�������
			}
			else             //���һ������
			{
				index  = 0;    //�����±���0
				data_flag = 0; //Ĭ��Ϊֹͣ֡
				data_come = 0; //֡��ʼ��־��Ϊ0
				
				for(i = 0;i<5;i++)
				{
					SBUF = 0xFF;
					while(!TI);
					TI = 0;
				}
				for(i = 0;i<10;i++)  //�����յ������ݷ��ͳ�
				{
					SBUF = a[i];
					while(!TI);
					TI = 0;
				}
			}
		}
	}
	
}
