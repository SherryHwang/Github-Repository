#include "DS18B20.h"
#include "intrins.h"
#include "Delay.h"
#include "LCD1602.h"

extern bit flag;  //引用主函数
uint d;
uchar tflag;            //温度正负标志

void TempDelay(uchar us)
{
	while(us--); //6.51us
}
/********************************************************************
*函数名：ds_reset
*函数功能：初始化DS18B20
*输入：无
*输出：无
***********************************************************************/
void ds_reset()
{
	ds=1;
	_nop_();
	ds=0;
	TempDelay(80);//将主机总线拉低520us左右,协议要求480us~960us
	ds=1;        //拉高(释放)总线，如果DS18B20作出反应将在15us~60us后将总线拉低
	TempDelay(5);//延时大于60us,确保接下来DS18B20能60~240us的存在脉冲应答
	if(ds==0)   //DS18B20发出存在脉冲，初始化成功
		flag=1;
	else
		flag=0;
	TempDelay(20);//延时达240us,让DS18B20释放总线
	ds=1;
}
/********************************************************************
*函数名：ds_read_bit
*函数功能:从DS18B20中读一位值
*输入：无
*输出：返回1bit
**************************************************************/
bit ds_read_bit()
{//数据线从逻辑高电平拉低至低电平时，产生读时间片
	bit dat;
	ds=0;
	_nop_();
	_nop_();//延时约2us,符合协议要求的至少延时1us
	ds=1;
	_nop_();
	dat=ds;
	TempDelay(12);
	return dat;
}
/********************************************************************
*函数名：ds_read_byte
*函数功能:从DS18B20中读一个字节
*输入：无
*输出：返回1Byte
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
*函数名：Write_Byte
*函数功能:向DS18B20中写一个字节
*输入：byteval，数据类型：uchar
*输出：无
***********************************************************************/
void ds_write_byte(uchar dat)
{
	uchar i,onebit;
	for(i=0;i<8;i++)
	{
		onebit=dat&0x01;
		if(onebit)	   ////写"1",将DS在15us~60us内拉高，即完成写1
		{
			ds=0;
			TempDelay(1);
			ds=1; //完成写1
			_nop_();
		}
		else  ////写"0",在15us~60us期间依旧检测到DS为低电平,即完成写0
		{
			ds=0;
			TempDelay(11);//延时约66us,在60us之后依旧检测到低电平,即完成写0
			ds=1;
			_nop_();
		}
		dat>>=1;
	}
}
void ds_change()
{
	ds_reset();          //DS18B20初始化成功
	ds_write_byte(0xcc); //跳过读序列号
	ds_write_byte(0x44); //启动温度转换
}
/********************************************************************
*函数名：get_temperature
*函数功能:读取器件温度值并转换成实际温度
*输入：无
*输出：实际温度值
***********************************************************************/
uint get_temperature()
{
	uchar TMPL,TMPH;
	float temperature;
	ds_reset();
	ds_write_byte(0xcc);    //跳过读序列号
	ds_write_byte(0xbe);    //读取温度
	TMPL=ds_read_byte();   //先读出的是温度数据低8位
	TMPH=ds_read_byte();  //读出的是温度数据高8位
	d=TMPH;
	d<<=8;
	d|=TMPL;
	temperature=d*0.0625;
	d=temperature*10+0.5;  //实际温度保留一位小数
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