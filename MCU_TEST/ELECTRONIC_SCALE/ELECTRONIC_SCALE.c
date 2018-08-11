#include "ELECTRONIC_SCALE.h"
#include "DEFINE.h"
#include "7SEG.h"
unsigned long Data;//32位，HX711输出
unsigned long first ;
unsigned long ElectronicScalse_ReadCount(void)
{
	unsigned long Count;//32位
	uchar i;
	ADSK=0;//SCK
	Count=0;
	while(ADDO);//等待A/D转换完成 转换完成变为低电平
	for (i=0;i<24;i++) //读24位
	{
		ADSK=1;  //数据上升沿
		if(ADDO) Count++; //读出数据为1 加1
		ADSK=0;
		Count=Count<<1;//左移一位
	}
	ADSK=1;
	Count=Count^0x800000;//异或
	ADSK=0;
	return  (Count);
}
//void ElectronicScalse_clear(void)
//{
//	uchar j,num;
//	unsigned long cd;
//	for(j = 0;j<20;j++)
//	{
//		Data= ElectronicScalse_ReadCount();//AD值
//		Data /= 100;
//		cd = Data;
//		cd = (cd /7.16+0.05);
//		first = cd;
//		cd -= first;
//		Display_NUM(cd);
//	}
//}

