#ifndef __BH1750_H__
#define __BH1750_H__
#include "DEFINE.h"
void BH1750_Write_Single(uchar REG_Address);				//���ֽ�д��
void BH1750_Init();																	//��ʼ��BH1750
void BH1750_Read_Multiple();												//�����Ķ�ȡ�ڲ��Ĵ�������
void BH1750_Display();
#endif