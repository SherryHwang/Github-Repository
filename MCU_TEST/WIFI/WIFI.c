#include "WIFI.h"
#include "DELAY.h"
code uchar  send1[] = "AT+CIPSEND=0,1\r\n";
code uchar  send2[] = "AT+CIPSEND=0,2\r\n";
code uchar  start1[] = "AT+CIPMUX=1\r\n";
code uchar  start2[] = "AT+CIPSERVER=1,8080\r\n";  //����code��
void Send_String(uchar * string)  //��wifiģ�鷢���ַ���
{
	while(*string != '\0')
	{
		SBUF = *string;
		while(!TI);
		TI = 0;
		string++;
	}
}
void Wifi_init()//wifiģ���ʼ��������wifiģ��
{
		Delay_ms(700);
		Send_String(start1);  
		Delay_ms(1000);
		Send_String(start2);
		Delay_ms(500);
}

void Wifi_Send_Char(uchar dat)
{
	Send_String(send1);
	SBUF = dat;
	while(!TI);
	TI = 0; 
}
void Wifi_Send_Int(uint dat)
{
	Send_String(send2);  //һ�η��������ֽ�
	SBUF = dat>>8;  //���͸߰�λ
	while(!TI);
	TI = 0; 
	
	SBUF = dat& 0X00FF;  //���͵Ͱ�λ
	while(!TI);
	TI = 0; 
}
void Wifi_Send_Long(unsigned long dat)
{	
	Send_String(send2);  //һ�η��������ֽ�
	SBUF = dat>>24;  //���͸߰�λ
	while(!TI);
	TI = 0; 
	SBUF = dat>>16;  //���͵Ͱ�λ
	while(!TI);
	TI = 0; 
	Send_String(send2);  //һ�η��������ֽ�
	SBUF = dat>>8;  //���͸߰�λ
	while(!TI);
	TI = 0; 
	SBUF = dat&0X000000FF;  //���͵Ͱ�λ
	while(!TI);
	TI = 0; 
}
void Wifi_Send_String(uchar dat[])
{
	uchar i;
	for(i = 0 ; dat[i] != '\0';i++)
	{
		Send_String(send1);
		SBUF = dat[i];  //���͸߰�λ
		while(!TI);
		TI = 0;
	}
}