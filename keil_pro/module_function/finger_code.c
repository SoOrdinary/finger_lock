#include <REGX52.H>
#include "Delay.h"
#include "LCD1602.H"
#include "MatrixKey.h"
#include "AS608.H"
#include "Timer0.h"

#define CharBuffer1 0x01
#define CharBuffer2 0x02
#define FigerStore_length 8

unsigned int figer_flag=1;
extern unsigned int PWM_Count;

/**
  * @brief  指纹锁初始化
  * @param  无
  * @retval 无
  */
void figer_Init()
{
	LCD_Init();
	LCD_ShowString(1,1,"Figer:");
	LCD_ShowNum(1,14,340,3);
	figer_flag=2;
}
/**
  * @brief  指纹锁步骤
  * @param  无
  * @retval 无
  */
void figer()
{
	static u16 FigerID;
	static u16 FigerScore;
	static u8 ensure=0xff;
	static unsigned int i=0;
	ensure=AS608_GetImage();//获取图像
	if(ensure==0x00)
	{
		P2_0=0;
//		Delay(200);
		ensure=AS608_GenChar(CharBuffer1);//生成特征
		if(ensure==0x00)
		{
			P2_1=0;
//			Delay(200);
			ensure=AS608_Search(CharBuffer1,0,FigerStore_length,&FigerID,&FigerScore);//指纹比对
			if(ensure==0x00)
			{
				Timer0_OPEN();
				P2_2=0;
				LCD_ShowString(2,1,"OK");
//				Delay(300);
				if(FigerID>=0&&FigerID<=2)
					LCD_ShowString(2,1,"name");
				if(FigerID>=3&&FigerID<=5)
					LCD_ShowString(2,1,"name");
				if(FigerID>=6&&FigerID<=8)
					LCD_ShowString(2,1,"name");
				PWM_Count=1;
				while(i<8)
				{
					P2_0=~P2_0;
					P2_1=~P2_1;
					P2_2=~P2_2;
					i++;
					Delay(300);
				}
				i=0;
				LCD_ShowString(2,1,"                ");
				PWM_Count=5;
				Delay(1000);
				Timer0_CLOSE();
			}
		}
		P2_0=1;
		P2_1=1;
		P2_2=1;
	}
}	
