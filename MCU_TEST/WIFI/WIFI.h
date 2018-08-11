#ifndef __WIFI_H__
#define __WIFI_H__
#include <reg52.h>
#include "DEFINE.h"
void Send_String(unsigned char * string); //发送字符串
void Wifi_init();//wifi模块初始化，启动wifi模块
void Wifi_Send_Char(uchar dat); //wifi模块发送一个字节
void Wifi_Send_Int(uint dat);		//wifi模块发送一个整数
void Wifi_Send_Long(unsigned long dat);//wifi模块发送一个长整数
void Wifi_Send_String(uchar dat[]); //wifi模块发送一个字符串
#endif