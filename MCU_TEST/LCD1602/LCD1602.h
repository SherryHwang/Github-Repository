#ifndef __LCD1602_H__
#define __LCD1602_H__

#define uchar unsigned char
#define uint unsigned int
void lcd1602_init();//初始化
void lcd1602_write_cmd(unsigned char cmd);//写命令
void lcd1602_write_data(unsigned char dat);//写数据
void lcd1602_write_char(unsigned char x, unsigned char y, unsigned char dat);//坐标定位，写字符
void lcd1602_write_string(unsigned char x, unsigned char y, unsigned char *dat); //坐标定位，确定行，x确定列，写字符串
	
#endif //__LCD1602_H__