#include <REGX52.H>
#include "bytes_define.h"
#include "finger_code.h"
#include "lock_code.h"
#include "Timer0.h"
#include "UART.h"
#include "MatrixKey.h"

xdata u8 UART_RX_SBUF[50];  //串口接收字符串
u16 UART_RX_NUM=0;          //单次总共接收数据大小
sbit PWM=P2^4;           //舵机PWM控制变量
unsigned int PWM_Count=5;
extern unsigned int lock_flag;
extern unsigned int figer_flag;

void main()
{
	Timer0_Init();
	UART_Init();
	Timer0_CLOSE();
	while(1)
	{
		if(figer_flag==1)figer_Init();
		if(figer_flag==2)figer();
		if(MatrixKey()==99)lock_flag=1;
		while(lock_flag)
		{
			if(lock_flag==1)Lock_Init();
			if(lock_flag==2)Lock();
		}
	}
}

void Timer0_Routine() interrupt 1  //舵机转动
{
	static unsigned int T0Count;
	TL0 = 0x33;		//设置定时初值
	TH0 = 0xFE;		//设置定时初值
	T0Count++;
	T0Count%=40;
	if(T0Count<PWM_Count)
	{
		PWM=1;
	}
	else
	{
		PWM=0;
	}
}

void UART_RxData() interrupt 4    //串口接收数据
{
	if(RI==1)
	{
		RI=0;
		UART_RX_SBUF[UART_RX_NUM++]=SBUF;//读取接收数据		
	}	
}

