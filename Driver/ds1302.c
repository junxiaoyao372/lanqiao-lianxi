#include "ds1302.h"

void Write_Ds1302(unsigned char temp)
{
	unsigned char i;
	for (i=0;i<8;i++)
	{ 
		SCK = 0;
		SDA = temp&0x01;
		temp>>=1; 
		SCK=1;
	}
}

void Write_Ds1302_Byte(unsigned char address,unsigned char dat)
{
  RST=0;	_nop_();
 	SCK=0;	_nop_();
 	RST=1; 	_nop_();  
 	Write_Ds1302(address);
 	Write_Ds1302(dat);
 	RST=0; 
}

unsigned char Read_Ds1302_Byte(unsigned char address)
{
  unsigned char i,temp=0x00;
  RST=0;	_nop_();
  SCK=0;	_nop_();
  RST=1;	_nop_();
  Write_Ds1302(address);
  for (i=0;i<8;i++)
  {		
		SCK=0;
		temp>>=1;
  	if(SDA)
		temp|=0x80;	
		SCK=1;
  } 
  RST=0;	_nop_();
  SCK=0;	_nop_();
	SCK=1;	_nop_();
	SDA=0;	_nop_();
	SDA=1;	_nop_();
	return (temp);			
}

void Set_RTC(unsigned char *pucRTC)
{
	unsigned char temp;
	
	Write_Ds1302_Byte(0x8E, 0x00);
	
	temp = (((pucRTC[0]/10)<<4)|(pucRTC[0]%10));
	Write_Ds1302_Byte(0x84, temp);
	
	temp = (((pucRTC[1]/10)<<4)|(pucRTC[1]%10));
	Write_Ds1302_Byte(0x82, temp);
	
	temp = (((pucRTC[2]/10)<<4)|(pucRTC[2]%10));
	Write_Ds1302_Byte(0x80, temp);
	
	Write_Ds1302_Byte(0x8E, 0x80);
}

void Get_RTC(unsigned char *pucRTC)
{
	unsigned char temp;
	
	temp = Read_Ds1302_Byte(0x85);
	pucRTC[0] = (((temp>>4)*10)+(temp & 0x0F));
	
	temp = Read_Ds1302_Byte(0x83);
	pucRTC[1] = (((temp>>4)*10)+(temp & 0x0F));
	
	temp = Read_Ds1302_Byte(0x81);
	pucRTC[2] = (((temp>>4)*10)+(temp & 0x0F));
}
