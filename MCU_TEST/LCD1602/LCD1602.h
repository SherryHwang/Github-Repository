#ifndef __LCD1602_H__
#define __LCD1602_H__
#include"DEFINE.h"

void lcd1602_init();//��ʼ��
void lcd1602_write_cmd(uchar cmd);//д����
void lcd1602_write_data(uchar dat);//д����
void lcd1602_write_char(uchar x, uchar y, uchar dat);//���궨λ��д�ַ�
void lcd1602_write_string(uchar x, uchar y, uchar *dat); //���궨λ��ȷ���У�xȷ���У�д�ַ���
void lcd1602_display_data(uchar str,uchar dat);  //��ʾuchar
	
#endif //__LCD1602_H__