#ifndef __DS18B20_H__
#define __DS18B20_H__
#include "DEFINE.h"
sbit ds   = P3^7;

void TempDelay(uchar us);
void ds_reset();
bit ds_read_bit();
uchar ds_read_byte();
void ds_write_byte(uchar dat);
void ds_change();
uint get_temperature();
void Display_Temperature(uint d);

#endif