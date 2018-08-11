#include "Delay.h"
#include "intrins.h"

void Delay_ms(uint z)
{
	uint x = 0, y = 0;
	for(x = z; x > 0; x--)
		for(y = 112; y > 0; y--);
}
void Delay_us(uchar n)
{
	while(n--)
		_nop_();
}
