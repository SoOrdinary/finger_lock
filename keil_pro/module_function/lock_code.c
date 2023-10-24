#include <REGX52.H>
#include "Delay.h"
#include "LCD1602.H"
#include "MatrixKey.h"
#include "Timer0.h"

extern unsigned int PWM_Count;
unsigned int lock_flag=0;
extern unsigned int figer_flag;

/**
  * @brief  密码锁初始化
  * @param  无
  * @retval 无
  */
void Lock_Init()
{
	LCD_Init();
	LCD_ShowString(1,1,"Password:");
	LCD_ShowNum(2,1,0000,4);
	lock_flag=2;
}

/**
  * @brief  密码锁步骤
  * @param  无
  * @retval 无
  */
void Lock()
{
	static unsigned int KeyNum=10,Password=0,Count=0;
	KeyNum=MatrixKey();
	if(KeyNum!=10)
	{
		if(KeyNum<=10&&Count<4)  //输入
		{
			Password*=10;
			Password+=KeyNum%10;
			Count++;
		}
		else if(KeyNum==11&&Count>0)  //撤回
		{
			Password/=10;
			Count--;
		}
		else if(KeyNum==12)  //清除
		{
			Password=0;
			Count=0;
		}
		else if(KeyNum==14)  //确认
		{
			Password+=10000;
			if(Password==10111||Password==10222||Password==10333||Password==10444||Password==10555)//正确密码
			{
				Timer0_OPEN();
				LCD_ShowString(1,14,"OK");
				PWM_Count=1;
				Delay(2000);
				Password=0;
				Count=0;
				LCD_ShowNum(2,1,Password,4);
				LCD_ShowString(1,14,"    ");
				PWM_Count=5;
				lock_flag=0;
				figer_flag=1;
				Delay(1000);
				Timer0_CLOSE();
			}
			else
			{
				LCD_ShowString(1,14,"ERR");
				Delay(1000);
				Password=0;
				Count=0;
				LCD_ShowNum(2,1,Password,4);
				LCD_ShowString(1,14,"    ");
				PWM_Count=5;
				lock_flag=0;
				figer_flag=1;
			}
		}
		if(Count==0)  //显示
		{
			LCD_ShowNum(2,1,Password,4);
		}
		else
		{
			LCD_ShowString(2,1,"    ");
			LCD_ShowNum(2,5-Count,Password,Count);
		}
	}
}
