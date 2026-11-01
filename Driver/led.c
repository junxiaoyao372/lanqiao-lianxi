#include "led.h"

void Led_Disp(unsigned char ucLed)
{
	P0 = ~ucLed;
	P2 = P2 & 0x1F | 0x80;
	P2 &= 0x1F;
}

void Beep_Ctrl(unsigned char ucBeep)
{
	if(ucBeep)
	{
		P0 = 0x40;
	}
	else
	{
		P0 = 0x00;
	}
	P2 = P2 & 0x1F | 0xA0;
	P2 &= 0x1F;
}
