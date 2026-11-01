#include "tim.h"

void Timer1Init(void)
{
	AUXR |= 0x40;
	TMOD &= 0x0F;
	TL1 = 0x20;
	TH1 = 0xD1;
	TF1 = 0;
	TR1 = 1;
	ET1 = 1;
}
