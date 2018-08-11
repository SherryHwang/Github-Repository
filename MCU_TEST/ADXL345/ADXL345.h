#ifndef __ADXL345_H__
#define __ADXL345_H__
#include "DEFINE.h"
void ADXL345_Write_Single(uchar REG_Address,uchar REG_data); //单字节写入
void ADXL345_Init();																				 //初始化ADXL345
void ADXL345_Read_Multiple();                               //连续的读取内部寄存器数据
void ADXL345_Display_xyz();																				//显示ADXL345测量数据
#endif