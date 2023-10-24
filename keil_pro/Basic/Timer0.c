#include <REGX52.H>

/**
  * @brief  定时器0初始化
  * @param  无
  * @retval 无
  */
void Timer0_Init()
{
//	TMOD=0x01;
	TMOD &= 0xF0;		//设置定时器模式
	TMOD |= 0x01;		//设置定时器模式
	TL0 = 0x33;		//设置定时初值
	TH0 = 0xFE;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	ET0=1;
	EA=1;
	PT0=0;
}
/**
  * @brief  打开定时器0
  * @param  无
  * @retval 无
  */
void Timer0_OPEN()
{
	TR0 = 1;		//定时器0开始计时
}
/**
  * @brief  关闭定时器0
  * @param  无
  * @retval 无
  */
void Timer0_CLOSE()
{
	TR0 = 0;		//定时器0开始计时
}
/*定时器中断函数模板
void Timer0_Routine() interrupt 1
{
	static unsigned int T0Count;
	TL0 = 0x66;		//设置定时初值
	TH0 = 0xFC;		//设置定时初值
	T0Count++;
	if(T0Count>=1000)
	{
		T0Count=0;
		
	}
}
*/
