#ifndef __LCD1602_H__
#define __LCD1602_H__

#define uchar unsigned char
#define uint unsigned int
void lcd1602_init();//��ʼ��
void lcd1602_write_cmd(unsigned char cmd);//д����
void lcd1602_write_data(unsigned char dat);//д����
void lcd1602_write_char(unsigned char x, unsigned char y, unsigned char dat);//���궨λ��д�ַ�
void lcd1602_write_string(unsigned char x, unsigned char y, unsigned char *dat); //���궨λ��ȷ���У�xȷ���У�д�ַ���
	
#endif //__LCD1602_H__