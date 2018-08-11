#include "LCD1602.h"
#include "reg52.h"
#include "Delay.h"

#define lcd1602_io P0

sbit lcd1602_en = P2 ^ 3;
sbit lcd1602_rs = P2 ^ 5;
sbit lcd1602_rw = P2 ^ 4;

void lcd1602_write_cmd(unsigned char cmd)  //写命令
{
    lcd1602_io = cmd;
    lcd1602_rs = 0;
    lcd1602_en = 0;
    Delay_ms(10);
    lcd1602_en = 1;
    Delay_ms(10);
    lcd1602_en = 0;
}

void lcd1602_write_data(unsigned char dat) //写数据
{
    lcd1602_io = dat;
    lcd1602_rs = 1;
    lcd1602_en = 0;
    Delay_ms(10);
    lcd1602_en = 1;
    Delay_ms(10);
    lcd1602_en = 0;

}

void lcd1602_init()
{
    lcd1602_rw = 0;
    lcd1602_write_cmd(0x38);   //显示模式设置：16×2显示，5×7点阵，8位数据接口
    Delay_ms(20);
    lcd1602_write_cmd(0x0e);   //显示模式设置
    Delay_ms(20);
    lcd1602_write_cmd(0x06);   //显示模式设置：光标右移，字符不移
    Delay_ms(20);
    lcd1602_write_cmd(0x01);   //清屏幕指令，将以前的显示内容清除
    Delay_ms(20);
}

void lcd1602_write_char(unsigned char x, unsigned char y, unsigned char dat) //坐标定位
{
    unsigned char address;
    if(y == 0)
        address = 0x80 + x; //y=0,写第一行
    else
        address = 0xc0 + x; //y=1,写第二行
    lcd1602_write_cmd(address);
    lcd1602_write_data(dat);
}

void lcd1602_write_string(unsigned char x, unsigned char y, unsigned char *dat) //坐标定位
{
    unsigned char address;
    if(y == 0)
        address = 0x80 + x; //y=0,写第一行
    else if(y == 1)
        address = 0xc0 + x; //y=1,写第二行
    lcd1602_write_cmd(address);
    while(*dat > 0)
    {
        lcd1602_write_data(*dat);
        dat++;
        Delay_ms(1);
    }
}

void lcd1602_display_data(uchar str[],uchar dat)
{
	lcd1602_write_cmd(0x01);
  lcd1602_write_string(0,0,str);
	lcd1602_write_char(0,1,'0' + dat / 100);
	lcd1602_write_char(1,1,'0' + (dat % 100) / 10);
	lcd1602_write_char(2,1,'0' + dat % 10);
}