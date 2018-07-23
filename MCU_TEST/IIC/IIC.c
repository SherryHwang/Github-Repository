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
  SDA=1;         /*发送起始条件的数据信号*/
  _nop_();
  SCL=1;
  Delay_us(5);   /*起始条件建立时间大于4.7us,延时*/
  SDA=0;         /*发送起始信号*/
	Delay_us(5);	 /* 起始条件锁定时间大于4μs*/    
  SCL=0;         /*钳住I2C总线，准备发送或接收数据 */
  _nop_();
  _nop_();
}

void Stop_I2c()
{
  SDA=0;      	/*发送结束条件的数据信号*/
  _nop_();       /*发送结束条件的时钟信号*/
  SCL=1;      
  Delay_us(5);	/*结束条件建立时间大于4μs*/
  SDA=1;      	/*发送I2C总线结束信号*/
  Delay_us(4);
}

void  SendByte(unsigned char  c)  //发送一个字节
{
	uchar BitCnt;
	for(BitCnt=0;BitCnt<8;BitCnt++)  /*要传送的数据长度为8位*/
	{
		if((c<<BitCnt)&0x80) SDA=1;   /*判断发送位*/
		else SDA=0;                
		_nop_();
		SCL=1;               /*置时钟线为高，通知被控器开始接收数据位*/
		Delay_us(5);				 /*保证时钟高电平周期大于4μs*/        
		SCL=0; 
	}
	
	Delay_us(2);
	SDA=1;                /*8位发送完后释放数据线，准备接收应答位*/
	Delay_us(2);  
	SCL=1;
	Delay_us(3);

	if(SDA==1)ack1=0;     
	else ack1=1;           /*判断是否接收到应答信号,全局变量*/
	SCL=0;
	Delay_us(2);
}

uchar RcvByte()
{
  uchar  retc;
  uchar BitCnt;
  retc=0; 
  SDA=1;                     /*置数据线为输入方式*/
  for(BitCnt=0;BitCnt<8;BitCnt++)
	{
		_nop_();           
		SCL=0;                  /*置时钟线为低，准备接收数据位*/
		Delay_us(5);						/*时钟低电平周期大于4.7μs*/
		SCL=1;                  /*置时钟线为高使数据线上数据有效*/
		Delay_us(2);
		retc=retc<<1;
		if(SDA==1)retc=retc+1;  /*读数据位,接收的数据位放入retc中 */
		Delay_us(2);
	}
  SCL=0;    
  Delay_us(2);
  return(retc);
}

void Ack_I2c(bit a)
{
  if(a==0)SDA=0;              /*在此发出应答或非应答信号 */
  else SDA=1;
  Delay_us(3);     
  SCL=1;
	Delay_us(5);								/*时钟低电平周期大于4μs*/
  SCL=0;                      /*清时钟线，钳住I2C总线以便继续接收*/
  Delay_us(2);   
}