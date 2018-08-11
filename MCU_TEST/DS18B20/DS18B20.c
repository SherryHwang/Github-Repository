#include "DS18B20.h"
#include "intrins.h"
#include "Delay.h"
#include "LCD1602.h"

extern bit flag;  //����������
uint d;
uchar tflag;            //�¶�������־

void TempDelay(uchar us)
{
	while(us--); //6.51us
}
/********************************************************************
*��������ds_reset
*�������ܣ���ʼ��DS18B20
*���룺��
*�������
***********************************************************************/
void ds_reset()
{
	ds=1;
	_nop_();
	ds=0;
	TempDelay(80);//��������������520us����,Э��Ҫ��480us~960us
	ds=1;        //����(�ͷ�)���ߣ����DS18B20������Ӧ����15us~60us����������
	TempDelay(5);//��ʱ����60us,ȷ��������DS18B20��60~240us�Ĵ�������Ӧ��
	if(ds==0)   //DS18B20�����������壬��ʼ���ɹ�
		flag=1;
	else
		flag=0;
	TempDelay(20);//��ʱ��240us,��DS18B20�ͷ�����
	ds=1;
}
/********************************************************************
*��������ds_read_bit
*��������:��DS18B20�ж�һλֵ
*���룺��
*���������1bit
**************************************************************/
bit ds_read_bit()
{//�����ߴ��߼��ߵ�ƽ�������͵�ƽʱ��������ʱ��Ƭ
	bit dat;
	ds=0;
	_nop_();
	_nop_();//��ʱԼ2us,����Э��Ҫ���������ʱ1us
	ds=1;
	_nop_();
	dat=ds;
	TempDelay(12);
	return dat;
}
/********************************************************************
*��������ds_read_byte
*��������:��DS18B20�ж�һ���ֽ�
*���룺��
*���������1Byte
***********************************************************************/
uchar ds_read_byte()
{
	uchar i,j,value;
	for(i=0;i<8;i++)
	{
		j=ds_read_bit();
		value=(j<<7)|(value>>1);
	}
	return value;
}
/********************************************************************
*��������Write_Byte
*��������:��DS18B20��дһ���ֽ�
*���룺byteval���������ͣ�uchar
*�������
***********************************************************************/
void ds_write_byte(uchar dat)
{
	uchar i,onebit;
	for(i=0;i<8;i++)
	{
		onebit=dat&0x01;
		if(onebit)	   ////д"1",��DS��15us~60us�����ߣ������д1
		{
			ds=0;
			TempDelay(1);
			ds=1; //���д1
			_nop_();
		}
		else  ////д"0",��15us~60us�ڼ����ɼ�⵽DSΪ�͵�ƽ,�����д0
		{
			ds=0;
			TempDelay(11);//��ʱԼ66us,��60us֮�����ɼ�⵽�͵�ƽ,�����д0
			ds=1;
			_nop_();
		}
		dat>>=1;
	}
}
void ds_change()
{
	ds_reset();          //DS18B20��ʼ���ɹ�
	ds_write_byte(0xcc); //���������к�
	ds_write_byte(0x44); //�����¶�ת��
}
/********************************************************************
*��������get_temperature
*��������:��ȡ�����¶�ֵ��ת����ʵ���¶�
*���룺��
*�����ʵ���¶�ֵ
***********************************************************************/
uint get_temperature()
{
	uchar TMPL,TMPH;
	float temperature;
	ds_reset();
	ds_write_byte(0xcc);    //���������к�
	ds_write_byte(0xbe);    //��ȡ�¶�
	TMPL=ds_read_byte();   //�ȶ��������¶����ݵ�8λ
	TMPH=ds_read_byte();  //���������¶����ݸ�8λ
	d=TMPH;
	d<<=8;
	d|=TMPL;
	temperature=d*0.0625;
	d=temperature*10+0.5;  //ʵ���¶ȱ���һλС��
	return d;
} 
void Display_Temperature(uint d)
{	
	if(tflag==0)
	{
		lcd1602_write_char(1,1,'+');
	}
	else
	{
		lcd1602_write_char(1,1,'-');
	}
	lcd1602_write_char(2,0,'0' + d / 1000);
	lcd1602_write_char(3,0,'0' + (d % 1000) / 100);
	lcd1602_write_char(4,0,'0' + (d % 100) / 10);
	lcd1602_write_char(5,0,'.');
	lcd1602_write_char(6,0,'0' + d % 10);
}