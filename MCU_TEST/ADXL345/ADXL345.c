#include "ADXL345.h"
#include "IIC.h"
#include "Delay.h"
#include "LCD1602.h"
uchar BUF[8];                         //接收数据缓存区   
void ADXL345_Write_Single(uchar REG_Address,uchar REG_data)  //单字节写入
{
	Start_I2c();  //开启I2C数据传输
	SendByte(0xA6); //发送设备地址+写信号,确定I2C上的设备
	SendByte(REG_Address);  //内部寄存器地址，请参考中文pdf22页 
	SendByte(REG_data);			//内部寄存器数据，请参考中文pdf22页 
	Stop_I2c();  //停止I2C数据传输
}
void ADXL345_Init()																					//初始化ADXL345
{
	ADXL345_Write_Single(0x31,0x0B);   //测量范围,正负16g，13位模式
	ADXL345_Write_Single(0x2C,0x08);   //速率设定为12.5 参考pdf13页
	ADXL345_Write_Single(0x2D,0x08);   //选择电源模式   参考pdf24页
	ADXL345_Write_Single(0x2E,0x80);   //使能 DATA_READY 中断
	ADXL345_Write_Single(0x1E,0x00);   //X 偏移量 根据测试传感器的状态写入pdf29页
	ADXL345_Write_Single(0x1F,0x00);   //Y 偏移量 根据测试传感器的状态写入pdf29页
	ADXL345_Write_Single(0x20,0x05);   //Z 偏移量 根据测试传感器的状态写入pdf29页
}

void ADXL345_Read_Multiple(void)										//连续的读取内部寄存器数据
{   
	uchar i;
	Start_I2c();                          //起始信号
	SendByte(0xA6);            						//发送设备地址+写信号
	SendByte(0x32);                   		//发送存储单元地址，从0x32开始	
	Start_I2c();                          //起始信号
	SendByte(0xA7);         							//发送设备地址+读信号
	for(i=0; i<6; i++)                    //连续读取6个地址数据，存储在中BUF
	{
			BUF[i] = RcvByte();          			//BUF[0]存储0x32地址中的数据
			if(i == 5)
			{
				 Ack_I2c(1);                		//最后一个数据需要回NOACK
			}
			else
			{
				 Ack_I2c(0);                			//回应ACK
		  }
	}
	Stop_I2c();                           //停止信号
	Delay_ms(5);
	BUF[6] = '\0';												//字符串结尾标志
}
void ADXL345_Display_x(int x)
{
	if(x<0)
	{
		x = -x;
    lcd1602_write_char(10,0,'-');      //显示正负符号位
	}
	else
		lcd1602_write_char(10,0,' '); //显示空格
	
	x = (float)x * 3.9;
	lcd1602_write_char(8,0,'X');
	lcd1602_write_char(9,0,':'); 
	x = x%10000;   //取余运算
	lcd1602_write_char(11,0,x/1000+'0'); 
	lcd1602_write_char(12,0,'.'); 
	x = x%1000;    //取余运算
	lcd1602_write_char(13,0,x/100+'0'); 
	x = x%100;     //取余运算
	lcd1602_write_char(14,0,x/10+'0'); 
	lcd1602_write_char(15,0,' '); 
}
void ADXL345_Display_y(int y)
{
	if(y<0)
	{
		y=-y;
    lcd1602_write_char(2,1,'-');      //显示正负符号位
	}
	else 
		lcd1602_write_char(2,1,' '); //显示空格
	
	y = (float)y * 3.9;
	lcd1602_write_char(0,1,'Y');   //第1行，第0列 显示y
	lcd1602_write_char(1,1,':'); 
	y = y%10000;   //取余运算
	lcd1602_write_char(3,1,y/1000+'0'); 
	lcd1602_write_char(4,1,'.'); 
	y = y%1000;    //取余运算
	lcd1602_write_char(5,1,y/100+'0'); 
	y = y%100;     //取余运算
	lcd1602_write_char(6,1,y/10+'0');  
	lcd1602_write_char(7,1,' '); 
}
void ADXL345_Display_z(int z)
{
	if(z<0)
	{
		z=-z;
    lcd1602_write_char(10,1,'-');       //显示负符号位
	}
	else 
		lcd1602_write_char(10,1,' ');  //显示空格
	
	z = (float)z * 3.9;
	lcd1602_write_char(8,1,'Z');  //第0行，第10列 显示Z
	lcd1602_write_char(9,1,':');
	z = z%10000;     //取余运算
	lcd1602_write_char(11,1,z/1000+'0'); 
	lcd1602_write_char(12,1,'.'); 
	z = z%1000;     //取余运算
	lcd1602_write_char(13,1,z/100+'0'); 
	z = z%100;     //取余运算
	lcd1602_write_char(14,1,z/10+'0'); 
	lcd1602_write_char(15,1,' ');  
}
void ADXL345_Display_xyz()					//显示ADXL345测量数据
{
	ADXL345_Display_x((BUF[1]<<8)+BUF[0]);
	
	ADXL345_Display_y((BUF[3]<<8)+BUF[2]);

	ADXL345_Display_z((BUF[5]<<8)+BUF[4]);
}