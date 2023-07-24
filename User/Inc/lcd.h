#ifndef __LCD_H__
#define __LCD_H__

#ifndef USE_LVGL

#include "font.h"

#endif

#include "main.h"
#include "gpio.h"
#include "spi.h"
#include "settings.h"

#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t


#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t

// 屏幕参数
#define LCD_WIDTH         480               // 设置屏幕的像素大小，
#define LCD_HEIGHT        320               // 注意：实际像素好像要比标示值大1~2像素，
#define LCD_DIR           4                 // 四种显示方向，参数：1、2、3、4
//  结束 移植修改 ************************************************************
/*****************************************************************************
 ** 颜色定义， 移植时不用修改
****************************************************************************/
#define WHITE             0xFFFF
#define BLACK             0x0000
#define BLUE             0x001F
#define BRED             0XF81F
#define GRED             0XFFE0
#define GBLUE             0X07FF
#define RED             0xF800
#define MAGENTA         0xF81F
#define GREEN             0x07E0
#define CYAN             0x7FFF
#define YELLOW             0xFFE0
#define BROWN             0XBC40 //棕色
#define BRRED             0XFC07 //棕红色
#define GRAY             0X8430 //灰色
#define DARKBLUE         0X01CF    //深蓝色
#define LIGHTBLUE         0X7D7C    //浅蓝色
#define GRAYBLUE         0X5458 //灰蓝色
#define LIGHTGREEN         0X841F //浅绿色
#define LIGHTGRAY        0XEF5B //浅灰色(PANNEL)
#define LGRAY             0XC618 //浅灰色(PANNEL),窗体背景色
#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)

//extern uint16_t colors[255];

// LCD_CS
#define LCD_CS_LOW LCD_CS_GPIO_Port->BSRR = LCD_CS_Pin << 16
#define LCD_CS_HIGH LCD_CS_GPIO_Port->BSRR = LCD_CS_Pin
//
#define LCD_BL_LOW LCD_BL_GPIO_Port->BSRR = LCD_BL_Pin << 16
#define LCD_BL_HIGH LCD_BL_GPIO_Port->BSRR = LCD_BL_Pin

#define LCD_RS_LOW LCD_DC_GPIO_Port->BSRR = LCD_DC_Pin << 16
#define LCD_RS_HIGH LCD_DC_GPIO_Port->BSRR = LCD_DC_Pin
// LCD_RST
#define LCD_RES_LOW LCD_RES_GPIO_Port->BSRR = LCD_RES_Pin << 16
#define LCD_RES_HIGH LCD_RES_GPIO_Port->BSRR = LCD_RES_Pin

/*****************************************************************************
 ** 声明  全局函数； 移植时不用修改
****************************************************************************/
void LCD_Init(
        void);

void LCD_Display(u8 sw);

void LCD_Fill(u16 sx, u16 sy, u16 ex, u16 ey, u16 color);

void LCD_String(u16 x, u16 y, char *pFont, u8 size, u32 fColor, u32 bColor);

void LCD_Format(u16 x, u16 y, u8 size, u32 fColor, u32 bColor, char *format, ...);

void drawPoint(u16 x, u16 y, u16 color);

void LCD_Chinese(u32 x, u32 y, u8 index, u8 size, u16 fColor, u16 bColor);

void setCursor(u16 xStart, u16 yStart, u16 xEnd, u16 yEnd);

void sendShort(u16 data);

u8 spiSendByte(u8 data);

void sendOrder(u16 order);

void sendByte(u8 data);

#endif
