#ifndef __ADXL345_H__
#define __ADXL345_H__
#include "DEFINE.h"
void ADXL345_Write_Single(uchar REG_Address,uchar REG_data); //���ֽ�д��
void ADXL345_Init();																				 //��ʼ��ADXL345
void ADXL345_Read_Multiple();                               //�����Ķ�ȡ�ڲ��Ĵ�������
void ADXL345_Display_xyz();																				//��ʾADXL345��������
#endif