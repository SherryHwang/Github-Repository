#ifndef __LATTICE_H__
#define __LATTICE_H__
#include "DEFINE.h"
sbit LED8X8_RowSelect0  =  P2 ^ 7;          //5-C
sbit LED8X8_RowSelect1  =  P2 ^ 6;          //6-B
sbit LED8X8_RowSelect2  =  P2 ^ 5;          //7-A
void Lattice_Select_Row(uchar row);//������;//������
void Lattice_Show(uint n);//������ʾ����

#endif