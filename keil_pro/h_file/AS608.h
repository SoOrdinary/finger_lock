#ifndef _AS608_H_
#define _AS608_H_
#include "bytes_define.h"

u8 AS608_GetImage();//探测手指并录入图形
u8 AS608_GenChar(u8 BufferID);//根据根据图像生成特征点存于CharBuffer1或CharBuffer2
u8 AS608_Search(u8 BufferID,u16 StartPage,u16 PageNum,u16 *pageID,u16 *mathscore);//指纹比对

#endif
