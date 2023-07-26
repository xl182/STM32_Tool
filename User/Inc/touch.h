#ifndef __TOUCH_H_
#define __TOUCH_H_

#include "sys.h"
#include "main.h"
#include "gpio.h"
#include "base.h"
#include "at24c02.h"
#include "i2c.h"
#include "lcd.h"

//触摸屏引脚定义
#define XPT2046_MOSI(state) HAL_GPIO_WritePin(T_DIN_GPIO_Port, T_DIN_Pin, state)
#define XPT2046_MISO HAL_GPIO_ReadPin(T_DO_GPIO_Port, T_DO_Pin)
#define XPT2046_SCK(state) HAL_GPIO_WritePin(T_CLK_GPIO_Port, T_CLK_Pin, state)
#define XPT2046_CS(state) HAL_GPIO_WritePin(T_CS_GPIO_Port, T_CS_Pin, state)
#define XPT2046_PEN HAL_GPIO_ReadPin(T_IRQ_GPIO_Port, T_IRQ_Pin)

#define TOUCH_DATA_BASE_ADDR 0x0040
#define TOUCH_PRESS_DOWN 0x80

//函数声明
void touch_init(void);

void touch_write_byte(u8 cmd);

u8 touch_read_xy(void);

u16 touch_read_data(u8 cmd);

void touch_correct();

void touch_write_para();

void touch_read_para();

uint8_t touch_scan();

//存放触摸屏信息的结构体
struct Touch {
    u16 x0; //物理坐标x
    u16 y0; //物理坐标y
    u16 x;  //像素坐标x
    u16 y;  //像素坐标y
    u8 sta;
    float x_fraction;
    float y_fraction;
    uint16_t x_offset;
    uint16_t y_offset;
};


extern struct Touch touch;
#endif
