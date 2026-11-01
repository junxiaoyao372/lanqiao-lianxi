#include "STC15F2K60S2.H"
#include "intrins.h"

sbit SCK = P1^7;
sbit SDA = P2^3;
sbit RST = P1^3;

void Set_RTC(unsigned char *pucRTC);
void Get_RTC(unsigned char *pucRTC);
