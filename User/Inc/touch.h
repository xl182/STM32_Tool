#ifndef XPT2046_TOUCH_H
#define XPT2046_TOUCH_H
#include "sys.h"
#include "main.h"
#include "base.h"

//触摸屏引脚定义
#define XPT2046_MOSI PCout(5)
#define XPT2046_MISO PAin(7)
#define XPT2046_SCK PEout(7)
#define XPT2046_CS  PBout(2)
#define XPT2046_PEN PAin(5)

//函数声明
void XPT2046_TouchInit(void);
void XPT2046_SPI_WriteOneByte(u8 cmd);
u8 XPT2046_ReadXY(void);
u16 XPT2046_ReadData(u8 cmd);
//存放触摸屏信息的结构体
struct XPT2046_TOUCH
{
    u16 x0; //物理坐标x
    u16 y0; //物理坐标y
    u16 x;  //像素坐标x
    u16 y;  //像素坐标y
};
extern struct XPT2046_TOUCH xpt2046_touch;
#endif
