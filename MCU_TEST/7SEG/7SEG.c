#include "reg52.h"
#include "7SEG.h"
#include "Delay.h"
#include <math.h>
uchar code led_data[11] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x40}; //0-A����
sbit LS138A = P3^4;  	//����138������������A����P2.0���� 
sbit LS138B = P3^5;	    //����138�������������B��P2.1����
sbit LS138C = P3^6; 	//����138�������������C��P2.2����
void Display_NUM(long n)						//��ʾ����
{
	uchar i;
	for(i = 0;i<8;i++)
	{	
		P0 = 0;
		switch(i)	  //ʹ��switch ������λѡ  Ҳ�������ò��ķ�ʽ ѧԱ���������Լ��޸�				  
	  {	    
			case 0:LS138A=1; LS138B=1; LS138C=1;  break;         
			case 1:LS138A=0; LS138B=1; LS138C=1;  break;             	
			case 2:LS138A=1; LS138B=0; LS138C=1;  break; 
			case 3:LS138A=0; LS138B=0; LS138C=1;  break; 
			case 4:LS138A=1; LS138B=1; LS138C=0;  break;
			case 5:LS138A=0; LS138B=1; LS138C=0;  break;
			case 7:LS138A=0; LS138B=0; LS138C=0;  break;
	  }
		Delay_ms(1);
		P0 = led_data[n%10];
		Delay_ms(1);
		n = n / 10;
		
	}
}