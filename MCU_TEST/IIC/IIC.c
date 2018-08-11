
/*************************此部分为I2C总线的驱动程序*************************************/

#include<reg52.h>
#include <intrins.h>
#include "IIC.h"

#define  NOP()   _nop_()   /* 定义空指令 */
#define  _Nop()  _nop_()   /*定义空指令*/

bit ack;                 /*应答标志位*/
extern uchar num; 

/*******************************************************************
                     起动总线函数               
函数原型: void  Start_I2c();  
功能:     启动I2C总线,即发送I2C起始条件.  
********************************************************************/
void Start_I2c()
{
	if(num == 12)  //BH1750 数字光强
	{
		SDA_BH1750=1;         /*发送起始条件的数据信号*/
		_Nop();
		SCL_BH1750=1;
		_Nop();        /*起始条件建立时间大于4.7us,延时*/
		_Nop();
		_Nop();
		_Nop();
		_Nop();    
		SDA_BH1750=0;         /*发送起始信号*/
		_Nop();        /* 起始条件锁定时间大于4μs*/
		_Nop();
		_Nop();
		_Nop();
		_Nop();       
		SCL_BH1750=0;       /*钳住I2C总线，准备发送或接收数据 */
		_Nop();
		_Nop();
	}
	else if(num == 15)  //ADXL345 三轴加速度
	{
		SDA_ADXL345=1;         /*发送起始条件的数据信号*/
		_Nop();
		SCL_ADXL345=1;
		_Nop();        /*起始条件建立时间大于4.7us,延时*/
		_Nop();
		_Nop();
		_Nop();
		_Nop();    
		SDA_ADXL345=0;         /*发送起始信号*/
		_Nop();        /* 起始条件锁定时间大于4μs*/
		_Nop();
		_Nop();
		_Nop();
		_Nop();       
		SCL_ADXL345=0;       /*钳住I2C总线，准备发送或接收数据 */
		_Nop();
		_Nop();
	}
	else
	{
		SDA=1;         /*发送起始条件的数据信号*/
		_Nop();
		SCL=1;
		_Nop();        /*起始条件建立时间大于4.7us,延时*/
		_Nop();
		_Nop();
		_Nop();
		_Nop();    
		SDA=0;         /*发送起始信号*/
		_Nop();        /* 起始条件锁定时间大于4μs*/
		_Nop();
		_Nop();
		_Nop();
		_Nop();       
		SCL=0;       /*钳住I2C总线，准备发送或接收数据 */
		_Nop();
		_Nop();
	}
}

/*******************************************************************
                      结束总线函数               
函数原型: void  Stop_I2c();  
功能:     结束I2C总线,即发送I2C结束条件.  
********************************************************************/
void Stop_I2c()
{
	if(num == 12)				//BH1750 数字光强
	{
		SDA_BH1750=0;      /*发送结束条件的数据信号*/
		_Nop();       /*发送结束条件的时钟信号*/
		SCL_BH1750=1;      /*结束条件建立时间大于4μs*/
		_Nop();
		_Nop();
		_Nop();
		_Nop();
		_Nop();
		SDA_BH1750=1;      /*发送I2C总线结束信号*/
		_Nop();
		_Nop();
		_Nop();
		_Nop();
	}
	else if(num == 15)		//ADXL345 三轴加速度
	{
		SDA_ADXL345=0;      /*发送结束条件的数据信号*/
		_Nop();       /*发送结束条件的时钟信号*/
		SCL_ADXL345=1;      /*结束条件建立时间大于4μs*/
		_Nop();
		_Nop();
		_Nop();
		_Nop();
		_Nop();
		SDA_ADXL345=1;      /*发送I2C总线结束信号*/
		_Nop();
		_Nop();
		_Nop();
		_Nop();
	}
	else
	{
		SDA=0;      /*发送结束条件的数据信号*/
		_Nop();       /*发送结束条件的时钟信号*/
		SCL=1;      /*结束条件建立时间大于4μs*/
		_Nop();
		_Nop();
		_Nop();
		_Nop();
		_Nop();
		SDA=1;      /*发送I2C总线结束信号*/
		_Nop();
		_Nop();
		_Nop();
		_Nop();
	}
}

/*******************************************************************
                 字节数据发送函数               
函数原型: void  SendByte(UCHAR c);
功能:     将数据c发送出去,可以是地址,也可以是数据,发完后等待应答,并对
          此状态位进行操作.(不应答或非应答都使ack=0)     
           发送数据正常，ack=1; ack=0表示被控器无应答或损坏。
********************************************************************/
void  SendByte(uchar c)
{
 uchar BitCnt;
 if(num == 12)
 {
	 for(BitCnt=0;BitCnt<8;BitCnt++)  /*要传送的数据长度为8位*/
		{
		 if((c<<BitCnt)&0x80) SDA_BH1750=1;   /*判断发送位*/
			 else  SDA_BH1750=0;                
		 _Nop();
		 SCL_BH1750=1;               /*置时钟线为高，通知被控器开始接收数据位*/
			_Nop(); 
			_Nop();             /*保证时钟高电平周期大于4μs*/
			_Nop();
			_Nop();
			_Nop();         
		 SCL_BH1750=0; 
		}
		
		_Nop();
		_Nop();
		SDA_BH1750=1;                /*8位发送完后释放数据线，准备接收应答位*/
		_Nop();
		_Nop();   
		SCL_BH1750=1;
		_Nop();
		_Nop();
		_Nop();
		if(SDA_BH1750==1)ack=0;     
			 else ack=1;        /*判断是否接收到应答信号,全局变量*/
		SCL_BH1750=0;
		_Nop();
		_Nop();
 }
 else if(num == 15)
 {
	 for(BitCnt=0;BitCnt<8;BitCnt++)  /*要传送的数据长度为8位*/
		{
		 if((c<<BitCnt)&0x80) SDA_ADXL345=1;   /*判断发送位*/
			 else  SDA_ADXL345=0;                
		 _Nop();
		 SCL_ADXL345=1;               /*置时钟线为高，通知被控器开始接收数据位*/
			_Nop(); 
			_Nop();             /*保证时钟高电平周期大于4μs*/
			_Nop();
			_Nop();
			_Nop();         
		 SCL_ADXL345=0; 
		}
		
		_Nop();
		_Nop();
		SDA_ADXL345=1;                /*8位发送完后释放数据线，准备接收应答位*/
		_Nop();
		_Nop();   
		SCL_ADXL345=1;
		_Nop();
		_Nop();
		_Nop();
		if(SDA_ADXL345==1)ack=0;     
			 else ack=1;        /*判断是否接收到应答信号,全局变量*/
		SCL_ADXL345=0;
		_Nop();
		_Nop();
 }
 else
 {
	 for(BitCnt=0;BitCnt<8;BitCnt++)  /*要传送的数据长度为8位*/
		{
		 if((c<<BitCnt)&0x80) SDA=1;   /*判断发送位*/
			 else  SDA=0;                
		 _Nop();
		 SCL=1;               /*置时钟线为高，通知被控器开始接收数据位*/
			_Nop(); 
			_Nop();             /*保证时钟高电平周期大于4μs*/
			_Nop();
			_Nop();
			_Nop();         
		 SCL=0; 
		}
		
		_Nop();
		_Nop();
		SDA=1;                /*8位发送完后释放数据线，准备接收应答位*/
		_Nop();
		_Nop();   
		SCL=1;
		_Nop();
		_Nop();
		_Nop();
		if(SDA==1)ack=0;     
			 else ack=1;        /*判断是否接收到应答信号,全局变量*/
		SCL=0;
		_Nop();
		_Nop();
	}
}

/*******************************************************************
                 字节数据接收函数               
函数原型: UCHAR  RcvByte();
功能:        用来接收从器件传来的数据,并判断总线错误(不发应答信号)，
          发完后请用应答函数应答从机。  
********************************************************************/    
uchar   RcvByte()
{
  unsigned char  retc;
  unsigned char  BitCnt;
  if(num == 12)
	{
		retc=0; 
		SDA_BH1750=1;                     /*置数据线为输入方式*/
		for(BitCnt=0;BitCnt<8;BitCnt++)
		{
			_Nop();           
			SCL_BH1750=0;                  /*置时钟线为低，准备接收数据位*/
			_Nop();
			_Nop();                 /*时钟低电平周期大于4.7μs*/
			_Nop();
			_Nop();
			_Nop();
			SCL_BH1750=1;                  /*置时钟线为高使数据线上数据有效*/
			_Nop();
			_Nop();
			retc=retc<<1;
			if(SDA_BH1750==1)retc=retc+1;  /*读数据位,接收的数据位放入retc中 */
			_Nop();
			_Nop(); 
		}
		SCL_BH1750=0;    
		_Nop();
		_Nop();
		return(retc);
	}
	else if(num == 15)
	{
		retc=0; 
		SDA_ADXL345=1;                     /*置数据线为输入方式*/
		for(BitCnt=0;BitCnt<8;BitCnt++)
		{
			_Nop();           
			SCL_ADXL345=0;                  /*置时钟线为低，准备接收数据位*/
			_Nop();
			_Nop();                 /*时钟低电平周期大于4.7μs*/
			_Nop();
			_Nop();
			_Nop();
			SCL_ADXL345=1;                  /*置时钟线为高使数据线上数据有效*/
			_Nop();
			_Nop();
			retc=retc<<1;
			if(SDA_ADXL345==1)retc=retc+1;  /*读数据位,接收的数据位放入retc中 */
			_Nop();
			_Nop(); 
		}
		SCL_ADXL345=0;    
		_Nop();
		_Nop();
		return(retc);
	}
	else
	{
		retc=0; 
		SDA=1;                     /*置数据线为输入方式*/
		for(BitCnt=0;BitCnt<8;BitCnt++)
		{
			_Nop();           
			SCL=0;                  /*置时钟线为低，准备接收数据位*/
			_Nop();
			_Nop();                 /*时钟低电平周期大于4.7μs*/
			_Nop();
			_Nop();
			_Nop();
			SCL=1;                  /*置时钟线为高使数据线上数据有效*/
			_Nop();
			_Nop();
			retc=retc<<1;
			if(SDA==1)retc=retc+1;  /*读数据位,接收的数据位放入retc中 */
			_Nop();
			_Nop(); 
		}
		SCL=0;    
		_Nop();
		_Nop();
		return(retc);
	}
}

/********************************************************************
                     应答子函数
函数原型:  void Ack_I2c(bit a);
功能:      主控器进行应答信号(可以是应答或非应答信号，由位参数a决定)
********************************************************************/
void Ack_I2c(bit a)
{
	if(num == 12)
	{
		if(a==0)SDA_BH1750=0;              /*在此发出应答或非应答信号 */
		else SDA_BH1750=1;
		_Nop();
		_Nop();
		_Nop();      
		SCL_BH1750=1;
		_Nop();
		_Nop();                    /*时钟低电平周期大于4μs*/
		_Nop();
		_Nop();
		_Nop();  
		SCL_BH1750=0;                     /*清时钟线，钳住I2C总线以便继续接收*/
		_Nop();
		_Nop();  
	}
	else if(num == 15)
	{
		if(a==0)SDA_ADXL345=0;              /*在此发出应答或非应答信号 */
		else SDA_ADXL345=1;
		_Nop();
		_Nop();
		_Nop();      
		SCL_ADXL345=1;
		_Nop();
		_Nop();                    /*时钟低电平周期大于4μs*/
		_Nop();
		_Nop();
		_Nop();  
		SCL_ADXL345=0;                     /*清时钟线，钳住I2C总线以便继续接收*/
		_Nop();
		_Nop();
	}
	else
	{
		if(a==0)SDA=0;              /*在此发出应答或非应答信号 */
		else SDA=1;
		_Nop();
		_Nop();
		_Nop();      
		SCL=1;
		_Nop();
		_Nop();                    /*时钟低电平周期大于4μs*/
		_Nop();
		_Nop();
		_Nop();  
		SCL=0;                     /*清时钟线，钳住I2C总线以便继续接收*/
		_Nop();
		_Nop();  
	}		
}
