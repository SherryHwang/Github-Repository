#include "IIC.h"
#include "intrins.h"
#include "Delay.h"

bit SDA,SCL,ack1;

void IDefine_Bus(bit SDA_NEW,SCL_NEW)
{
	SDA = SDA_NEW;
	SCL = SCL_NEW;
}

void Start_I2c()
{
  SDA=1;         /*������ʼ�����������ź�*/
  _nop_();
  SCL=1;
  Delay_us(5);   /*��ʼ��������ʱ�����4.7us,��ʱ*/
  SDA=0;         /*������ʼ�ź�*/
	Delay_us(5);	 /* ��ʼ��������ʱ�����4��s*/    
  SCL=0;         /*ǯסI2C���ߣ�׼�����ͻ�������� */
  _nop_();
  _nop_();
}

void Stop_I2c()
{
  SDA=0;      	/*���ͽ��������������ź�*/
  _nop_();       /*���ͽ���������ʱ���ź�*/
  SCL=1;      
  Delay_us(5);	/*������������ʱ�����4��s*/
  SDA=1;      	/*����I2C���߽����ź�*/
  Delay_us(4);
}

void  SendByte(unsigned char  c)  //����һ���ֽ�
{
	uchar BitCnt;
	for(BitCnt=0;BitCnt<8;BitCnt++)  /*Ҫ���͵����ݳ���Ϊ8λ*/
	{
		if((c<<BitCnt)&0x80) SDA=1;   /*�жϷ���λ*/
		else SDA=0;                
		_nop_();
		SCL=1;               /*��ʱ����Ϊ�ߣ�֪ͨ��������ʼ��������λ*/
		Delay_us(5);				 /*��֤ʱ�Ӹߵ�ƽ���ڴ���4��s*/        
		SCL=0; 
	}
	
	Delay_us(2);
	SDA=1;                /*8λ��������ͷ������ߣ�׼������Ӧ��λ*/
	Delay_us(2);  
	SCL=1;
	Delay_us(3);

	if(SDA==1)ack1=0;     
	else ack1=1;           /*�ж��Ƿ���յ�Ӧ���ź�,ȫ�ֱ���*/
	SCL=0;
	Delay_us(2);
}

uchar RcvByte()
{
  uchar  retc;
  uchar BitCnt;
  retc=0; 
  SDA=1;                     /*��������Ϊ���뷽ʽ*/
  for(BitCnt=0;BitCnt<8;BitCnt++)
	{
		_nop_();           
		SCL=0;                  /*��ʱ����Ϊ�ͣ�׼����������λ*/
		Delay_us(5);						/*ʱ�ӵ͵�ƽ���ڴ���4.7��s*/
		SCL=1;                  /*��ʱ����Ϊ��ʹ��������������Ч*/
		Delay_us(2);
		retc=retc<<1;
		if(SDA==1)retc=retc+1;  /*������λ,���յ�����λ����retc�� */
		Delay_us(2);
	}
  SCL=0;    
  Delay_us(2);
  return(retc);
}

void Ack_I2c(bit a)
{
  if(a==0)SDA=0;              /*�ڴ˷���Ӧ����Ӧ���ź� */
  else SDA=1;
  Delay_us(3);     
  SCL=1;
	Delay_us(5);								/*ʱ�ӵ͵�ƽ���ڴ���4��s*/
  SCL=0;                      /*��ʱ���ߣ�ǯסI2C�����Ա��������*/
  Delay_us(2);   
}