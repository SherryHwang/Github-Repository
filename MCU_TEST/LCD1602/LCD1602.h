#ifndef __LCD1602_H__
#define __LCD1602_H__
#include"DEFINE.h"

void lcd1602_init();//初始化
void lcd1602_write_cmd(uchar cmd);//写命令
void lcd1602_write_data(uchar dat);//写数据
void lcd1602_write_char(uchar x, uchar y, uchar dat);//坐标定位，写字符
void lcd1602_write_string(uchar x, uchar y, uchar *dat); //坐标定位，确定行，x确定列，写字符串
void lcd1602_display_data(uchar str,uchar dat);  //显示uchar
	
#endif //__LCD1602_H__