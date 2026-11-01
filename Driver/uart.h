#include "STC15F2K60S2.H"

void Uart_Init(void);
void Uart_Send_Char(char cDat);
void Uart_Send_Str(char *pcStr);
unsigned char Uart_Get_Line(unsigned char *pucBuf);
