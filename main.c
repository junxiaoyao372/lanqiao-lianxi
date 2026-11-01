#include <STC15F2K60S2.H>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "init.h"
#include "tim.h"
#include "key.h"
#include "led.h"
#include "seg.h"
#include "ds1302.h"
#include "uart.h"

unsigned long ulms = 0;
unsigned int key_dly = 0;
unsigned int seg_dly = 0;
unsigned int uiRTC_Dly = 0;
unsigned int uiDone_Dly = 0;

unsigned char key_value,key_value_old;

unsigned char ucLed;

unsigned char pucSeg_Buf[12];
unsigned char pucSeg_Code[8] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
unsigned char ucSeg_Pos = 0;

unsigned char pucCmd[32];

xdata unsigned char pucRTC[3] = {23, 55, 55};

unsigned char disp_mode = 0;
unsigned char run_state = 0;
unsigned char ucFocus = 12;
unsigned int uiRemain_ms = 12000;
unsigned char ucDone_Flag = 0;

void key_proc(void);
void led_proc(void);
void seg_proc(void);
void rtc_proc(void);
void timer_proc(void);
void cmd_proc(void);

void main()
{
	Cls_Peripheral();
	Timer1Init();
	Uart_Init();
	Set_RTC(pucRTC);
	EA = 1;
	
	while(1){
		key_proc();
		led_proc();
		seg_proc();
		rtc_proc();
		timer_proc();
		cmd_proc();
	}
}

void led_proc(void)
{
	unsigned char ucTemp;
	
	ucTemp = 0;
	
	if(run_state == 1)
	{
		ucTemp |= 0x01;
	}
	if(run_state == 2)
	{
		ucTemp |= 0x02;
	}
	if(ucDone_Flag)
	{
		ucTemp |= 0x04;
	}
	
	if(ucTemp != ucLed)
	{
		ucLed = ucTemp;
		Led_Disp(ucLed);
	}
}

void key_proc(void)
{
	if(key_dly < 20)
		return;
	key_dly = 0;
	
	key_value = Key_Read_KBD();
	
	if(key_value == key_value_old)
		return;
	key_value_old = key_value;
	
	if(key_value == 0)
		return;
	
	if(run_state == 3)
		return;
	
	switch(key_value)
	{
		case 4:
			disp_mode = (disp_mode + 1) % 2;
			break;
		case 5:
			if(disp_mode == 1)
			{
				if(run_state == 0)
				{
					uiRemain_ms = (unsigned int)ucFocus * 1000;
					run_state = 1;
					ucDone_Flag = 0;
					Beep_Ctrl(0);
				}
				else if(run_state == 1)
				{
					run_state = 2;
				}
				else if(run_state == 2)
				{
					run_state = 1;
				}
			}
			break;
		case 7:
			if(disp_mode == 1 && (run_state == 1 || run_state == 2))
			{
				run_state = 0;
				uiRemain_ms = (unsigned int)ucFocus * 1000;
				ucDone_Flag = 0;
				Beep_Ctrl(0);
			}
			break;
		case 8:
			if(disp_mode == 1 && run_state == 0)
			{
				if(ucFocus < 60)
					ucFocus++;
				uiRemain_ms = (unsigned int)ucFocus * 1000;
			}
			break;
		case 9:
			if(disp_mode == 1 && run_state == 0)
			{
				if(ucFocus > 1)
					ucFocus--;
				uiRemain_ms = (unsigned int)ucFocus * 1000;
			}
			break;
	}
}

void seg_proc(void)
{
	unsigned int uiSec;
	
	if(seg_dly < 100)
		return;
	seg_dly = 0;
	
	if(disp_mode == 0)
	{
		sprintf((char *)pucSeg_Buf, "%02u-%02u-%02u", (unsigned int)pucRTC[0], (unsigned int)pucRTC[1], (unsigned int)pucRTC[2]);
	}
	else
	{
		uiSec = (uiRemain_ms + 999) / 1000;
		if(uiSec > 99)
			uiSec = 99;
		sprintf((char *)pucSeg_Buf, "P     %02u", uiSec);
	}
	
	Seg_Tran(pucSeg_Buf, pucSeg_Code);
}

void rtc_proc(void)
{
	if(uiRTC_Dly < 170)
		return;
	
	uiRTC_Dly = 0;
	Get_RTC(pucRTC);
}

void timer_proc(void)
{
	if(run_state == 1 && uiRemain_ms == 0)
	{
		run_state = 3;
		ucDone_Flag = 1;
		disp_mode = 1;
		uiDone_Dly = 0;
		Beep_Ctrl(1);
	}
	
	if(run_state == 3)
	{
		if(uiDone_Dly >= 5000)
			{
			run_state = 0;
			uiRemain_ms = (unsigned int)ucFocus * 1000;
			Beep_Ctrl(0);
		}
	}
}

void cmd_proc(void)
{
	int iVal;
	
	if(Uart_Get_Line(pucCmd) == 0)
		return;
	
	if(run_state == 3)
		return;
	
	if(strncmp((char *)pucCmd, "SET,", 4) == 0)
	{
		iVal = atoi((char *)pucCmd + 4);
		if(run_state == 0 && iVal >= 1 && iVal <= 60)
		{
			ucFocus = (unsigned char)iVal;
			uiRemain_ms = (unsigned int)iVal * 1000;
			Uart_Send_Str("OK\r\n");
		}
		else
		{
			Uart_Send_Str("ERR\r\n");
		}
	}
	else if(strcmp((char *)pucCmd, "STA") == 0)
	{
		if(run_state == 0)
		{
			uiRemain_ms = (unsigned int)ucFocus * 1000;
			run_state = 1;
			ucDone_Flag = 0;
			Beep_Ctrl(0);
			Uart_Send_Str("OK\r\n");
		}
		else if(run_state == 2)
		{
			run_state = 1;
			Uart_Send_Str("OK\r\n");
		}
		else
		{
			Uart_Send_Str("ERR\r\n");
		}
	}
	else if(strcmp((char *)pucCmd, "PAS") == 0)
	{
		if(run_state == 1)
		{
			run_state = 2;
			Uart_Send_Str("OK\r\n");
		}
		else
		{
			Uart_Send_Str("ERR\r\n");
		}
	}
	else
	{
		Uart_Send_Str("ERR\r\n");
	}
}

void timer1_isr(void) interrupt 3
{
	ulms++;
	key_dly++;
	seg_dly++;
	uiRTC_Dly++;
	uiDone_Dly++;
	
	if(run_state == 1)
	{
		if(uiRemain_ms > 0)
			{
			uiRemain_ms--;
		}
	}
	
	if(ulms%2 == 0){
		ucSeg_Pos = (ucSeg_Pos + 1) % 8;
		Seg_Disp(pucSeg_Code, ucSeg_Pos);
	}
}
