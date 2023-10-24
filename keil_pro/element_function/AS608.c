#include <REGX52.H>
#include <string.h>
#include "bytes_define.h"

u32 AS608Addr = 0XFFFFFFFF; //默认地址
extern xdata u8 UART_RX_SBUF[50];  //串口接收字符串
extern u16 UART_RX_NUM;          //单次总共接收数据大小

/**
  * @brief  串口发送一个字节
  * @param  Data 十六进制数，长度：一个字节
  * @retval 无
  */
void SendData(u8 Data)
{
	SBUF=Data;
	while(TI==0);
	TI=0;
}
/**
  * @brief  AS608包头
  * @param  无
  * @retval 无
  */
void SendHead()
{
	SendData(0xEF);
	SendData(0x01);
}
/**
  * @brief  发送地址
  * @param  无
  * @retval 无
  */
void SendAddr()
{
	SendData(AS608Addr>>24);
	SendData(AS608Addr>>16);
	SendData(AS608Addr>>8);
	SendData(AS608Addr);
}
/**
  * @brief  发送包标识
  * @param  无
  * @retval 无
  */
void SendFlag(u8 flag)
{
	SendData(flag);
}
/**
  * @brief  发送包长度
  * @param  无
  * @retval 无
  */
void SendLength(int length)
{
	SendData(length>>8);
	SendData(length);
}
/**
  * @brief  发送指令码
  * @param  无
  * @retval 无
  */
void Sendcmd(u8 cmd)
{
	SendData(cmd);
}
/**
  * @brief  发送校验和
  * @param  无
  * @retval 无
  */
void SendCheck(u16 check)
{
	SendData(check>>8);
	SendData(check);
}
/**
  * @brief  探测手指并录入图形
  * @param  无
  * @retval 返回确认字ensure
			0x00 录入成功
			0x01 收容包有错
			0x02 传感区无手指
			0x03 录入失败
			0xff 接收错误
  */
u8 AS608_GetImage()
{
	u16 temp;
	u8 ensure;
	//发送录入指令
	SendHead();
	SendAddr();
	SendFlag(0x01);//命令包标识
	SendLength(0x03);
	Sendcmd(0x01);
	temp =  0x01+0x03+0x01;
	SendCheck(temp);
	//返回接收数据
	while(1)
	{
		if(UART_RX_NUM==12)
		{
			if(UART_RX_SBUF[0]==0XEF&&UART_RX_SBUF[1]==0X01&&UART_RX_SBUF[6]==0X07)
			{
				ensure=UART_RX_SBUF[9];
				UART_RX_NUM=0;
				return ensure;
			}
			else
			{
				ensure=0xff;
				UART_RX_NUM=0;
				return ensure;
			}
		}	
	}
}
/**
  * @brief  根据图像生成特征点存于CharBuffer1或CharBuffer2
  * @param  BufferID 特征缓冲区号CharBuffer:0x01   CharBuffer:0x02
  * @retval 返回确认字ensure
			0x00 生成特征成功
			0x01 收包有错
			0x06/0x07/0x15 生成特征失败
			0xff 接收错误
  */
u8 AS608_GenChar(u8 BufferID)
{
	u16 temp;
	u8 ensure;
	//发送生成指令
	SendHead();
	SendAddr();
	SendFlag(0x01);//命令包标识
	SendLength(0x04);
	Sendcmd(0x02);
	SendData(BufferID);
	temp =  0x01+0x04+0x02+BufferID;
	SendCheck(temp);
	//返回接收数据
	while(1)
	{
		if(UART_RX_NUM==12)
		{
			if(UART_RX_SBUF[0]==0XEF&&UART_RX_SBUF[1]==0X01&&UART_RX_SBUF[6]==0X07)
			{
				ensure=UART_RX_SBUF[9];
				UART_RX_NUM=0;
				return ensure;
			}
			else
			{
				ensure=0xff;
				UART_RX_NUM=0;
				return ensure;
			}
		}	
	}
}
/**
  * @brief  指纹比对
  * @param  BufferID 缓冲区号CharBuffer:0x01   CharBuffer:0x02
  * @param  StartPage 指纹库起始页
  * @param  PageNum 指纹库页数
  * @param  *pageID 返回的指纹ID数所存的指针
  * @param  *mathscore 返回的指纹匹配得分所存的地址
  * @retval 返回确认字ensure
			0x00 搜索到
			0x01 收包有错
			0x09 未搜索到(返回pageID/mathscore均为0)
			0xff 接收错误
  */
u8 AS608_Search(u8 BufferID,u16 StartPage,u16 PageNum,u16 *pageID,u16 *mathscore)
{
	u16 temp;
	u8 ensure;
	//发送比对指令
	SendHead();
	SendAddr();
	SendFlag(0x01);//命令包标识
	SendLength(0x08);
	Sendcmd(0x1b);
	SendData(BufferID);
	SendData(StartPage>>8);
	SendData(StartPage);
	SendData(PageNum>>8);
	SendData(PageNum);
	temp = 0x01+0x08+0x1b+BufferID
			+(StartPage>>8)+(u8)StartPage
			+(PageNum>>8)+(u8)PageNum;
	SendCheck(temp);
	//返回接收数据
	while(1)
	{
		if(UART_RX_NUM==16)
		{
			if(UART_RX_SBUF[0]==0XEF&&UART_RX_SBUF[1]==0X01&&UART_RX_SBUF[6]==0X07)
			{
				ensure=UART_RX_SBUF[9];
				*pageID 	=(UART_RX_SBUF[10]<<8) +UART_RX_SBUF[11];
				*mathscore  =(UART_RX_SBUF[12]<<8) +UART_RX_SBUF[13];
				UART_RX_NUM=0;
				return ensure;
			}
			else
			{
				ensure=0xff;
				UART_RX_NUM=0;
				return ensure;
			}
		}	
	}
}