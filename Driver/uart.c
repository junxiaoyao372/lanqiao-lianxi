#include "uart.h"

sfr T2L = 0xD7;
sfr T2H = 0xD6;

xdata unsigned char pucRx_Buf[32];
xdata unsigned char pucCmd_Buf[32];
unsigned char ucRx_Len = 0;
unsigned char ucCmd_Ready = 0;

void Uart_Init(void)
{
	SCON = 0x50;
	AUXR |= 0x01;
	AUXR |= 0x04;
	T2L = 0xC7;
	T2H = 0xFE;
	AUXR |= 0x10;
	ES = 1;
}

void Uart_Send_Char(char cDat)
{
	ES = 0;
	TI = 0;
	SBUF = cDat;
	while(!TI);
	TI = 0;
	ES = 1;
}

void Uart_Send_Str(char *pcStr)
{
	while(*pcStr != '\0')
	{
		Uart_Send_Char(*pcStr);
		pcStr++;
	}
}

unsigned char Uart_Get_Line(unsigned char *pucBuf)
{
	unsigned char i;
	
	if(ucCmd_Ready == 0)
	{
		return 0;
	}
	
	for(i=0; i<32; i++)
	{
		pucBuf[i] = pucCmd_Buf[i];
		if(pucCmd_Buf[i] == '\0')
		{
			break;
		}
	}
	ucCmd_Ready = 0;
	
	return 1;
}

void uart_isr(void) interrupt 4
{
	unsigned char ucDat;
	unsigned char i;
	
	if(RI)
	{
		RI = 0;
		ucDat = SBUF;
		
		if(ucDat == '\n' || ucDat == '\r')
		{
			if(ucRx_Len > 0)
			{
				if(ucCmd_Ready == 0)
				{
					for(i=0; i<ucRx_Len; i++)
					{
						pucCmd_Buf[i] = pucRx_Buf[i];
					}
					pucCmd_Buf[ucRx_Len] = '\0';
					ucCmd_Ready = 1;
				}
				ucRx_Len = 0;
			}
		}
		else if(ucRx_Len < 31)
		{
			pucRx_Buf[ucRx_Len] = ucDat;
			ucRx_Len++;
		}
	}
}
