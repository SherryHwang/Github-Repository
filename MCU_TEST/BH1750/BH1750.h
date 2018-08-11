#ifndef __BH1750_H__
#define __BH1750_H__
#include "DEFINE.h"
void BH1750_Write_Single(uchar REG_Address);				//单字节写入
void BH1750_Init();																	//初始化BH1750
void BH1750_Read_Multiple();												//连续的读取内部寄存器数据
void BH1750_Display();
#endif