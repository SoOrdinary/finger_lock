#include <REGX52.H>

/**
  * @brief  串口初始化
  * @param  无
  * @retval 无
  */
void UART_Init()		//9600bps@11.0592MHz
{
	SCON |=0x50;
	PCON |= 0x80;
	TMOD &= 0x0F;		//设置定时器模式
	TMOD |= 0x20;		//设置定时器模式
	TL1 = 0xFA;		//设定定时初值
	TH1 = 0xFA;		//设定定时器重装值
	ET1 = 0;		//禁止定时器1中断
	TR1 = 1;		//启动定时器1
	EA=1;
	ES=1;
}

/**
  * @brief  串口发送数据函数
  * @param  Byte：需要发送的数据（Hex）
  * @retval 无
  */
void UART_SendByte(unsigned char Byte)
{
	SBUF=Byte;
	while(TI==0);
	TI=0;
}
/*串口接收函数
void UART_Routine() interrupt 4
{
	if(RI==1)
	{
		
	}
}
*/
