#include "ELECTRONIC_SCALE.h"
#include "DEFINE.h"
#include "7SEG.h"
unsigned long Data;//32λ��HX711���
unsigned long first ;
unsigned long ElectronicScalse_ReadCount(void)
{
	unsigned long Count;//32λ
	uchar i;
	ADSK=0;//SCK
	Count=0;
	while(ADDO);//�ȴ�A/Dת����� ת����ɱ�Ϊ�͵�ƽ
	for (i=0;i<24;i++) //��24λ
	{
		ADSK=1;  //����������
		if(ADDO) Count++; //��������Ϊ1 ��1
		ADSK=0;
		Count=Count<<1;//����һλ
	}
	ADSK=1;
	Count=Count^0x800000;//���
	ADSK=0;
	return  (Count);
}
//void ElectronicScalse_clear(void)
//{
//	uchar j,num;
//	unsigned long cd;
//	for(j = 0;j<20;j++)
//	{
//		Data= ElectronicScalse_ReadCount();//ADֵ
//		Data /= 100;
//		cd = Data;
//		cd = (cd /7.16+0.05);
//		first = cd;
//		cd -= first;
//		Display_NUM(cd);
//	}
//}

