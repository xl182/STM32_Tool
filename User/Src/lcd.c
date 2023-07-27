#include "lcd.h"

/*****************************************************************************
 ** 本地变量
 *****************************************************************************/
struct _LCD              // 结构体：用于保存信息、状态
{
    u8 InitOK;
    u16 width;             // LCD 宽度
    u16 height;             // LCD 高度

    u32 bColor;          // 背景色
} xLCD;

/*****************************************************************************
 ** 本地函数
 ****************************************************************************/
void sendOrder(u16 order);                                   // 向液晶屏发送：寄存器地址值(指令), 8位
void sendByte(u8 Data);                                    // 向液晶屏发送：数据，8位
void sendShort(u16 Data);                                    // 向液晶屏发送：数据，16位
void setCursor(u16 x_start, u16 y_start, u16 x_end, u16 y_end); // 设置显示区域

// SPI发送传送1字节
u8 spiSendByte(u8 data) {
    while ((SPI2->SR & SPI_FLAG_TXE) == RESET) {};
    SPI2->DR = data;
    while ((SPI2->SR & SPI_FLAG_RXNE) == RESET) {};
    return SPI2->DR;
}

#ifdef LCD_ST7735

void LCD_Init_Command(void) {
    sendOrder(0x11);       // 退出睡眠模式
    HAL_Delay(120);         // Delay 120ms
    //------------------------------------ST7735S Frame Rate-----------------------------------------//
    sendOrder(0xB1);
    sendByte(0x05);
    sendByte(0x3C);
    sendByte(0x3C);

    sendOrder(0xB2);
    sendByte(0x05);
    sendByte(0x3C);
    sendByte(0x3C);

    sendOrder(0xB3);
    sendByte(0x05);
    sendByte(0x3C);
    sendByte(0x3C);
    sendByte(0x05);
    sendByte(0x3C);
    sendByte(0x3C);
    //------------------------------------End ST7735S Frame Rate-----------------------------------------//
    sendOrder(0xB4); // Dot inversion
    sendByte(0x03);

    sendOrder(0xC0); // ST7735R Power Sequence
    sendByte(0x28);
    sendByte(0x08);
    sendByte(0x04);

    sendOrder(0xC1);
    sendByte(0XC0);

    sendOrder(0xC2);
    sendByte(0x0D);
    sendByte(0x00);
    sendOrder(0xC3);

    sendByte(0x8D);
    sendByte(0x2A);

    sendOrder(0xC4);
    sendByte(0x8D);
    sendByte(0xEE);
    //---------------------------------End ST7735S Power Sequence-------------------------------------//
    sendOrder(0xC5); //VCOM
    sendByte(0x1A);
    sendOrder(0x36); //MX, MY, RGB mode
    if (LCD_DIR == 1) sendByte(0xC0);  // 重要：显示方向控制，C0/00/A0/60,  C8/08/A8/68
    if (LCD_DIR == 2) sendByte(0x00);
    if (LCD_DIR == 3) sendByte(0xA0);
    if (LCD_DIR == 4) sendByte(0x60);
    //------------------------------------ST7735S Gamma Sequence-----------------------------------------//
    sendOrder(0xE0);
    sendByte(0x04);
    sendByte(0x22);
    sendByte(0x07);
    sendByte(0x0A);
    sendByte(0x2E);
    sendByte(0x30);
    sendByte(0x25);
    sendByte(0x2A);
    sendByte(0x28);
    sendByte(0x26);
    sendByte(0x2E);
    sendByte(0x3A);
    sendByte(0x00);
    sendByte(0x01);
    sendByte(0x03);
    sendByte(0x13);

    sendOrder(0xE1);
    sendByte(0x04);
    sendByte(0x16);
    sendByte(0x06);
    sendByte(0x0D);
    sendByte(0x2D);
    sendByte(0x26);
    sendByte(0x23);
    sendByte(0x27);
    sendByte(0x27);
    sendByte(0x25);
    sendByte(0x2D);
    sendByte(0x3B);
    sendByte(0x00);
    sendByte(0x01);
    sendByte(0x04);
    sendByte(0x13);

    // 后期复制增加的，不明白
    sendOrder(0x2a);
    sendByte(0x00);
    sendByte(0x00);
    sendByte(0x00);
    sendByte(0x7f);
    // 后期复制增加的，不明白
    sendOrder(0x2b);
    sendByte(0x00);
    sendByte(0x00);
    sendByte(0x00);
    sendByte(0x9f);
    //------------------------------------End ST7735S Gamma Sequence-----------------------------------------//
    sendOrder(0x3A); // 65k mode
    sendByte(0x05);

    sendOrder(0x29); // Display on
    }
#endif

#ifdef LCD_ILI9486

void LCD_Init_Command(void) {
//*************4.0inch ILI9486初始化**********//
    sendOrder(0xCF);
    sendByte(0x00);
    sendByte(0x81);
    sendByte(0x30);

    sendOrder(0xED);
    sendByte(0x64);
    sendByte(0x03);
    sendByte(0x12);
    sendByte(0x81);

    sendOrder(0xE8);
    sendByte(0x85);
    sendByte(0x10);
    sendByte(0x78);

    sendOrder(0xCB);
    sendByte(0x39);
    sendByte(0x2C);
    sendByte(0x00);
    sendByte(0x34);
    sendByte(0x02);

    sendOrder(0xF7);
    sendByte(0x20);

    sendOrder(0xEA);
    sendByte(0x00);
    sendByte(0x00);

    sendOrder(0xB1);
    sendByte(0x00);
    sendByte(0x1B);

    sendOrder(0xB6);
    sendByte(0x0A);
    sendByte(0xA2);

    sendOrder(0xC0);
    sendByte(0x35);

    sendOrder(0xC1);
    sendByte(0x11);

    sendOrder(0xC5);
    sendByte(0x45);
    sendByte(0x45);

    sendOrder(0xC7);
    sendByte(0xA2);

    sendOrder(0xF2);
    sendByte(0x00);

    sendOrder(0x26);
    sendByte(0x01);

    sendOrder(0xE0); //Set Gamma
    sendByte(0x0F);
    sendByte(0x26);
    sendByte(0x24);
    sendByte(0x0B);
    sendByte(0x0E);
    sendByte(0x09);
    sendByte(0x54);
    sendByte(0xA8);
    sendByte(0x46);
    sendByte(0x0C);
    sendByte(0x17);
    sendByte(0x09);
    sendByte(0x0F);
    sendByte(0x07);
    sendByte(0x00);
    sendOrder(0XE1); //Set Gamma
    sendByte(0x00);
    sendByte(0x19);
    sendByte(0x1B);
    sendByte(0x04);
    sendByte(0x10);
    sendByte(0x07);
    sendByte(0x2A);
    sendByte(0x47);
    sendByte(0x39);
    sendByte(0x03);
    sendByte(0x06);
    sendByte(0x06);
    sendByte(0x30);
    sendByte(0x38);
    sendByte(0x0F);

    sendOrder(0x36);
    if (LCD_DIR == 1) sendByte(0xC0);  // 重要：显示方向控制，C0/00/A0/60,  C8/08/A8/68
    if (LCD_DIR == 2) sendByte(0x00);
    if (LCD_DIR == 3) sendByte(0xA0);
    if (LCD_DIR == 4) sendByte(0x60);

    sendOrder(0X2A);
    sendByte(0x00);
    sendByte(0x00);
    sendByte(0x00);
    sendByte(0xEF);

    sendOrder(0X2B);
    sendByte(0x00);
    sendByte(0x00);
    sendByte(0x01);
    sendByte(0x3F);

    sendOrder(0x3A);
    sendByte(0x55);

    sendOrder(0x11);
    HAL_Delay(120);
    sendOrder(0x29);
}

#endif

void LCD_Init(void) {
    xLCD.InitOK = 0;
    xLCD.bColor = BLACK;

    // 显示方向像素匹配
    if ((LCD_DIR == 1) || (LCD_DIR == 3)) {
        xLCD.width = LCD_WIDTH;       // 屏宽度像素，超过此值驱动芯片会自动换行，注意：如果屏幕右边有花屏，就加大这个值
        xLCD.height = LCD_HEIGHT;        // 屏高度像素屏高度像素， 注意：如果屏幕下面有花屏，就加大这个值
    } else {
        xLCD.width = LCD_HEIGHT;
        xLCD.height = LCD_WIDTH;
    }

    MX_GPIO_Init();
    MX_SPI2_Init();
    __HAL_SPI_ENABLE(&hspi2);

    LCD_RES_LOW;           // LCD_RST=0	  //SPI接口复位
    HAL_Delay(100);          // delay 20 ms
    LCD_RES_HIGH;        // LCD_RST=1
    HAL_Delay(50);
    LCD_Init_Command();
    xLCD.InitOK = 1;
}


void sendOrder(u16 order) {
    LCD_CS_LOW;            // SPI设备片选拉低，开始通信
    LCD_RS_LOW;            // RS高: 数据值， RS低: 寄存器地址值
    spiSendByte(order);    // 发送 寄存器地址值
    LCD_CS_HIGH;           // SPI设备片选拉高，结束通信
}

void sendByte(u8 data) {
    LCD_CS_LOW;            // SPI设备片选拉低，开始通信
    LCD_RS_HIGH;           // RS高: 数据值， RS低: 寄存器地址值
    spiSendByte(data);     // 发送1字节
    LCD_CS_HIGH;           // SPI设备片选拉高，结束通信
}

#ifdef LCD_ST7735
void sendShort(u16 data) {
    LCD_CS_LOW;            // SPI设备片选拉低，开始通信
    LCD_RS_HIGH;           // RS高: 数据， RS低: 指令
    spiSendByte((data >> 8) & 0xF8);
    spiSendByte((data >> 3) & 0xFC);
    spiSendByte(data << 3);
    LCD_CS_HIGH;           // SPI设备片选拉高，结束通信
}
#endif

#ifdef LCD_ILI9486

void sendShort(u16 data) {
    LCD_CS_LOW;            // SPI设备片选拉低，开始通信
    LCD_RS_HIGH;           // RS高: 数据， RS低: 指令
    spiSendByte(data >> 8);
    spiSendByte(data);
    LCD_CS_HIGH;           // SPI设备片选拉高，结束通信
}

#endif

void LCD_Display(u8 sw) {
    if (sw == 0)
        LCD_BL_LOW;
    else
        LCD_BL_HIGH;
}

void setCursor(u16 xStart, u16 yStart, u16 xEnd, u16 yEnd) {
    sendOrder(0x2A);
    sendByte(xStart >> 8);   // 起始位置x高位，因为1.8寸屏是128*160, 不大于255, 直接写0省事
    sendByte(xStart); // 起始位置x低位，值传递时自动舍弃了高8位，也省得运算了
    sendByte(xEnd >> 8);   // 起始位置y高位
    sendByte(xEnd);   // 起始位置x位位

    sendOrder(0x2B);
    sendByte(yStart >> 8);
    sendByte(yStart);
    sendByte(yEnd >> 8);
    sendByte(yEnd);

    sendOrder(0x2c);  // 发送写数据命令
}

void drawPoint(u16 x, u16 y, u16 color) {
    setCursor(x, y, x, y);      //设置光标位置
    sendShort(color);
}

void LCD_Fill(u16 xStart, u16 yStart, u16 xEnd, u16 yEnd, u16 color) {
//    spiInit();                                      // 防止SPI参数被其它设备修改了
    u32 pixel = (xEnd - xStart + 1) * (yEnd - yStart + 1);    // 填充的像素数量

    setCursor(xStart, yStart, xEnd, yEnd);        // 设定填充范围
    while (pixel-- > 0)                                // 发送颜色值
        sendShort(color);
}

void LCD_Format(u16 x, u16 y, u8 size, u32 fColor, u32 bColor, char *format, ...) {
    va_list va;
    va_start(va, format);
    char string[100];
    sprintf(string, format, va);
    LCD_String(x, y, string, size, fColor, bColor);
    va_end(va);
}

#ifndef USE_LVGL
/******************************************************************
 * 函数名： LCD_Chinese16ForFile
 * 功  能： 显示1个16号汉字,字库数据在font文件中，只适合少量汉字固定输出
 *          PCtoLCD2002取模：阴码+逐列式+顺向+C51格式
 * 参  数： u16 x,y     左上角起始坐标
 *          u8  index   字库数据在缓存中的位置
 *          u32 fColor  字色
 *          u32 bColor  底色
 *****************************************************************/
//void LCD_Chinese(u32 x, u32 y, u8 index, uint8_t size, u16 fColor, u16 bColor) {
////    spiInit();                                      // 防止SPI参数被其它设备修改了
//
//    const u8 *temp = bitmap_bytes[index];              // 获取字体数据在数组中的的起始位置
//    const u8 len = bitmap_bytes_len[index];
//    for (u8 j = 0; j < size; j++) {
//        setCursor(x, y + j, x + size * len, y + j + size);
//        for (int c = 0; c < len; c++) {
//            for (u8 k = 0; k < size >> 3; k++) {
//                for (u8 i = 0; i < 8; i++) {
//                    if ((*temp & (128 >> i)) != 0)
//                        sendShort(fColor);
//                    else
//                        sendShort(bColor);
//                }
//                temp++;
//            }
//        }
//    }
//}

/******************************************************************
 * 函数名： LCD_Chinese32ForFile
 * 功  能： 显示1个32号汉字,字库数据在font文件中，只适合少量汉字固定输出
 *          PCtoLCD2002取模：阴码+逐列式+顺向+C51格式
 * 参  数： u16 x,y     左上角起始坐标
 *          u8  index   字库数据在缓存中的位置
 *          u32 fColor  字体颜色
 *          u32 bColor  背景颜色
 *****************************************************************/
//void LCD_Chinese32ForFile(u32 x, u32 y, u8 index, u16 fColor, u16 bColor) {
////    spiInit();                                  // 防止SPI参数被其它设备修改了
//
//    const u8 *temp = hanzi32 + index * 128;       // 获取字体数据在数组中的的起始位置
//
//    for (u8 j = 0; j < 32; j++)                      // 一行一行地向下写
//    {
//        setCursor(x, y + j, x + 32, y + j + 32);          // 只要定位了首个坐标，后面发送的数据，就会向后排列，直至收到新的命令
//        for (u8 k = 0; k < 4; k++) {
//            for (u8 i = 0; i < 8; i++) {
//                if ((*temp & (1 << i)) != 0)
//                    sendShort(fColor);
//                else
//                    sendShort(bColor);
//            }
//            temp++;
//        }
//    }
//}

/******************************************************************
 * 函数名： drawAscii
 * 功  能： 在指定位置显示一个字符
 * 参  数： u16 x,y     起始坐标
 *          u8  num     要显示的字符:" "--->"~"
 *          u8  size    字体大小 12/16/24/32
 *          u32 fColor  字体颜色
 *          u32 bColor  背景颜色
 * 备  注： 参考原子哥和野火大神的代码后修改  资料存放Q群：262901124
 *****************************************************************/
void drawAscii(u16 x, u16 y, u8 num, u8 size, u32 fColor, u32 bColor) {
//    spiInit();                                        // 防止SPI参数被其它设备修改了

    if (xLCD.InitOK == 0) return;

    u8 temp;
    u16 y0 = y;
    size = 12;
    u8 csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size / 2);           // 得到字体一个字符对应点阵集所占的字节数
    num = num - ' ';                                       // 得到偏移后的值（ASCII字库是从空格开始取模，所以-' '就是对应字符的字库）
    for (u8 t = 0; t < csize; t++) {
        if (size == 12) temp = asc2_1206[num][t];   // 调用1206字体
//        else if (size == 16) temp = asc2_1608[num][t];   // 调用1608字体
//        else if (size == 24) temp = asc2_2412[num][t];   // 调用2412字体
//        else if (size == 32) temp = asc2_3216[num][t];   // 调用3216字体
        else return;                                   // 没有的字库

        for (u8 t1 = 0; t1 < 8; t1++) {
            if (temp & 0x80) {
                drawPoint(x, y, fColor);  // 字体 画点
            } else {
                drawPoint(x, y, bColor);
            } // 背景 画点
            temp <<= 1;
            y++;
            if (y >= xLCD.height) return;               // 超出屏幕高度(底)
            if ((y - y0) == size) {
                y = y0;
                x++;
                if (x >= xLCD.width) return;              // 超出屏幕宽度(宽)
                break;
            }
        }
    }
}

/*
// code 字符指针开始
// 从字库中查找出字模
// code 字符串的开始地址,GBK码
// mat  数据存放地址 (size/8+((size%8)?1:0))*(size) bytes大小
// size:字体大小
void GetGBK(u8 *code, u8 *mat, u8 size) {
    u8 qh, ql;
    u32 foffset;
    u8 csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size);    // 计算汉字点阵大小，单位字节数

    qh = *code;
    ql = *(++code);

    if (qh < 0x81 || ql < 0x40 || ql == 0xff || qh == 0xff)    // 非常用汉字，将用填充显示整个位置
    {
        for (u8 i = 0; i < csize; i++) *mat++ = 0x00;  // 填充满格
        return;                                 // 结束访问
    }

    if (ql < 0x7f)ql -= 0x40;                        // 计算要提取的汉字在字库中的偏移位置
    else ql -= 0x41;
    qh -= 0x81;
    foffset = ((unsigned long) 190 * qh + ql) * csize;    // 得到汉字在字库中的偏移位置

    switch (size) {                                                                                 // 按字体的不同，在不同字库读取字体点阵
        case 12:
            W25qxx_ReadBuffer(foffset + GBK_STORAGE_ADDR, mat, csize);    // 12号字体
            break;
        case 16:
            W25qxx_ReadBuffer(foffset + GBK_STORAGE_ADDR + 0x0008c460, mat, csize);    // 12号字体
            break;
        case 24:
            W25qxx_ReadBuffer(foffset + GBK_STORAGE_ADDR + 0x001474E0, mat, csize);    // 24号字体
            break;
        case 32:
            W25qxx_ReadBuffer(foffset + GBK_STORAGE_ADDR + 0x002EC200, mat, csize);    // 32号字体
            break;
    }
}

// 显示汉字
void drawGBK(u16 x, u16 y, u8 *font, u8 size, u32 fColor, u32 bColor) {
    u8 temp;
    u16 y0 = y;
    u8 GBK[128];
    u8 csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size);    // 得到字体一个字符对应点阵集所占的字节数
    GetGBK(font, GBK, size);                    // 得到相应大小的点阵数据

    spiInit();                                  // 防止SPI参数被其它设备修改了
    for (u8 t = 0; t < csize; t++) {
        temp = GBK[t];                            // 得到GBK点阵数据
        for (u8 t1 = 0; t1 < 8; t1++) {
            if (temp & 0x80) drawPoint(x, y, fColor);
            else drawPoint(x, y, bColor);
            temp <<= 1;
            y++;
            if ((y - y0) == size) {
                y = y0;
                x++;
                break;
            }
        }
    }
}
 */

/******************************************************************************
 * 函  数： LCD_String
 * 功  能： 在屏幕上显示字符串，支持英文、中文
            重要提示：注意字库的格式、位置， 本函数字库在W25Q128中
 *
 * 参  数：
 * 返回值：　
 * 备  注： 魔女开发板团队  资料存放Q群：262901124        最后修改_2020年05月1８日
 ******************************************************************************/
void LCD_String(u16 x, u16 y, char *pFont, u8 size, u32 fColor, u32 bColor) {
    if (xLCD.InitOK == 0)
        return;

    u16 xStart = x;

    // 字体大小控制
    if (size != 12 && size != 16 && size != 24 && size != 32)
        size = 24;

    while (*pFont != 0) {
        // 位置控制
        if (x > (xLCD.width - size))       // 如果这一行不够位置，就下一行
        {
            x = xStart;
            y = y + size;
        }
        if (y > (xLCD.height - size))    // 如果到了屏幕底部，就返回，不再输出
            return;

        // 判断文字是ASCII还是汉字
        if (*pFont < 127)              // ASCII字符
        {
            drawAscii(x, y, *pFont, size, fColor, bColor);
            pFont++;
            x += size / 2;
        } else                          // 汉字显示
        {
            // 重要: 如果用的不是魔女开发板的字库, 就要修改或注释下面这一行, 这样就不影响ASCII英文字符的输出
            // drawGBK(x, y, (u8 *) pFont, size, fColor, bColor);
            pFont = pFont + 2;          // 下一个要显示的数据在内存中的位置
            x = x + size;                 // 下一个要显示的数据在屏幕上的X位置
        }
    }
}
#endif
