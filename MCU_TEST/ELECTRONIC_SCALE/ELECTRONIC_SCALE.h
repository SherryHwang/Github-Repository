#ifndef __ELECTRONIC_SCALE_h__
#define __ELECTRONIC_SCALE_h__
#include "reg52.h"
sbit ADDO=P1^7;//DOUT
sbit ADSK=P1^6;//SCK
unsigned long ElectronicScalse_ReadCount(void);
#endif