#ifndef __WIFI_H__
#define __WIFI_H__
#include <reg52.h>
#include "DEFINE.h"
void Send_String(unsigned char * string); //�����ַ���
void Wifi_init();//wifiģ���ʼ��������wifiģ��
void Wifi_Send_Char(uchar dat); //wifiģ�鷢��һ���ֽ�
void Wifi_Send_Int(uint dat);		//wifiģ�鷢��һ������
void Wifi_Send_Long(unsigned long dat);//wifiģ�鷢��һ��������
void Wifi_Send_String(uchar dat[]); //wifiģ�鷢��һ���ַ���
#endif