#include "BH1750.h"
#include "IIC.h"
#include "Delay.h"
#include "lcd1602.h"
extern uchar BUF[8];

void BH1750_Write_Single(uchar REG_Address)				//���ֽ�д��
{
  Start_I2c();  //����I2C���ݴ���
	SendByte(0x46); //�����豸��ַ+д�ź�,ȷ��I2C�ϵ��豸
	SendByte(REG_Address);  //�ڲ��Ĵ�����ַ����ο�����pdf22ҳ 
	Stop_I2c();  //ֹͣI2C���ݴ���
}

void BH1750_Init()																	//��ʼ��BH1750
{
	BH1750_Write_Single(0x01);// power on
	BH1750_Write_Single(0x10);// H- resolution mode
}

void BH1750_Read_Multiple()												//�����Ķ�ȡ�ڲ��Ĵ�������
{
	uchar i;	
	Start_I2c();                          //��ʼ�ź�
	SendByte(0x47);         //�����豸��ַ+���ź�
	for (i=0; i<3; i++)                      //������ȡ2����ַ���ݣ��洢��BUF
	{
		BUF[i] = RcvByte();         					 //BUF[0]�洢0x32��ַ�е�����
		if (i == 3)
		{
			 Ack_I2c(1);                //���һ��������Ҫ��NOACK
		}
		else
		{		
			 Ack_I2c(0);                //��ӦACK
		}
	}
	BUF[3] = '\0';
	Stop_I2c();                          //ֹͣ�ź�
	Delay_ms(5);
}
void BH1750_Display()
{
	int dis_data = (BUF[0]<<8)+BUF[1];
	dis_data=(float)dis_data/1.2;
	
	dis_data = dis_data%100000;
	lcd1602_write_char(0,1,dis_data/10000 + '0'); //��ʾ���� wan
	dis_data = dis_data%10000;
	lcd1602_write_char(1,1,dis_data/1000 + '0');  //qian
	dis_data = dis_data%1000;	
	lcd1602_write_char(2,1,dis_data/100 + '0');   //bai
	dis_data = dis_data%100;
	lcd1602_write_char(3,1,dis_data/10 + '0');   //shi
	lcd1602_write_char(4,1,dis_data%10 + '0');         //ge

	lcd1602_write_char(5,1,'L'); ////��ʾ����λ
	lcd1602_write_char(6,1,'X');  
}