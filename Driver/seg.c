#include "seg.h"

void Seg_Tran(unsigned char *pucSeg_Buf, unsigned char *pucSeg_Code)
{
	unsigned char i,j;
	
	for(i=0,j=0; i<=7; i++,j++)
	{
		switch(pucSeg_Buf[j])
		{
			case '0':
				pucSeg_Code[i] = 0xC0;
				break;
			case '1':
				pucSeg_Code[i] = 0xF9;
				break;
			case '2':
				pucSeg_Code[i] = 0xA4;
				break;
			case '3':
				pucSeg_Code[i] = 0xB0;
				break;
			case '4':
				pucSeg_Code[i] = 0x99;
				break;
			case '5':
				pucSeg_Code[i] = 0x92;
				break;
			case '6':
				pucSeg_Code[i] = 0x82;
				break;
			case '7':
				pucSeg_Code[i] = 0xF8;
				break;
			case '8':
				pucSeg_Code[i] = 0x80;
				break;
			case '9':
				pucSeg_Code[i] = 0x90;
				break;
				
			case 'A':
				pucSeg_Code[i] = 0x88;
				break;
			case 'C':
				pucSeg_Code[i] = 0xC6;
				break;
			case 'H':
				pucSeg_Code[i] = 0x89;
				break;
			case 'F':
				pucSeg_Code[i] = 0x8E;
				break;
			case 'P':
				pucSeg_Code[i] = 0x8C;
				break;
			case 'E':
				pucSeg_Code[i] = 0x86;
				break;
			case '-':
				pucSeg_Code[i] = 0xBF;
				break;
			case ' ':
				pucSeg_Code[i] = 0xFF;
				break;
		}
		if(pucSeg_Buf[j+1] == '.')
		{
			pucSeg_Code[i] &= 0x7F;
			j++;
		}
	}
}

void Seg_Disp(unsigned char *pucSeg_Code, unsigned char ucSeg_Pos)
{
	P0 = 0xFF;
	P2 = P2 & 0x1F | 0xE0;
	P2 &= 0x1F;
	
	P0 = 1<<ucSeg_Pos;
	P2 = P2 & 0x1F | 0xC0;
	P2 &= 0x1F;
	
	P0 = pucSeg_Code[ucSeg_Pos];
	P2 = P2 & 0x1F | 0xE0;
	P2 &= 0x1F;
}
