#include "key.h"

unsigned char Key_Read_KBD(void)
{
	unsigned int Key_New = 0;
	
	P44 = 0;
	P42 = 1;
	P35 = 1;
	P34 = 1;
	Key_New = (P3 & 0x0F);
	
	P44 = 1;
	P42 = 0;
	P35 = 1;
	P34 = 1;
	Key_New = (Key_New<<4)|(P3 & 0x0F);
	
	P44 = 1;
	P42 = 1;
	P35 = 0;
	P34 = 1;
	Key_New = (Key_New<<4)|(P3 & 0x0F);
	
	P44 = 1;
	P42 = 1;
	P35 = 1;
	P34 = 1;
	Key_New = (Key_New<<4)|(P3 & 0x0F);
	
	switch(~Key_New)
	{
		case 0x8000:
			return 4;
		case 0x4000:
			return 5;
		case 0x2000:
			return 6;
		case 0x1000:
			return 7;
			
		case 0x0800:
			return 8;
		case 0x0400:
			return 9;
		case 0x0200:
			return 10;
		case 0x0100:
			return 11;
			
		case 0x0080:
			return 12;
		case 0x0040:
			return 13;
		case 0x0020:
			return 14;
		case 0x0010:
			return 15;
			
		case 0x0008:
			return 16;
		case 0x0004:
			return 17;
		case 0x0002:
			return 18;
		case 0x0001:
			return 19;
		default:
			return 0;
	}
}
