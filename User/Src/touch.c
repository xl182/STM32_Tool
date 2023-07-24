#include "touch.h"

void delay_us(uint32_t us) {
    uint32_t delay = (HAL_RCC_GetHCLKFreq() / 4000000 * us);
    while (delay--) {};
}

struct XPT2046_TOUCH xpt2046_touch;

/*
函数功能:  初始化
硬件连接:
T_MOSI--PF9
T_MISO--PB2
T_SCK---PB1
T_PEN---PF10
T_CS----PF11
*/
void XPT2046_TouchInit(void) {

}


/*
函数功能:  SPI底层写一个字节
*/
void XPT2046_SPI_WriteOneByte(u8 cmd) {
    u8 i;
    for (i = 0; i < 8; i++) {
        XPT2046_SCK = 0; //低电平写
        if (cmd & 0x80)XPT2046_MOSI = 1;
        else
            XPT2046_MOSI = 0;
        cmd <<= 1;
        XPT2046_SCK = 1; //高电平读,保证数据线稳定
    }
}


/*
函数功能:  读2个字节
说明: 读取16位数据，最低4位数据无效，有效数据是高12位
*/
u16 XPT2046_ReadData(u8 cmd) {
    u16 data;
    u8 i;
    XPT2046_CS = 0;  //选中XPT2046
    XPT2046_MOSI = 0;
    XPT2046_SCK = 0;
    XPT2046_SPI_WriteOneByte(cmd);

    delay_us(8); //0.008ms ,等待XPT2046转换完成。
    //消除忙信号
    XPT2046_SCK = 0;
    delay_us(1);
    XPT2046_SCK = 1;

    //连续读取16位的数据
    for (i = 0; i < 16; i++) {
        XPT2046_SCK = 0; //通知XPT2046,主机需要数据
        XPT2046_SCK = 1;
        data <<= 1;
        if (XPT2046_MISO)
            data |= 0x01;
    }
    data >>= 4; //丢弃最低4位

    XPT2046_CS = 1; //取消选中
    return data;
}

/*
XPT2046的命令:
10010000  :测试Y的坐标 0x90
11010000  :测试X的坐标 0xD0
返回值:  0表示没有读取到坐标,1表示读取到当前坐标
//1. 得到左上角和右下角的坐标XY极限值
  x=3831,y=3934
  x=155,y=168
//2. 转换坐标值
  x坐标:3831~155 -->  3676~0
  y坐标:3934~168 -->  3766~0
//3. 计算斜率
  x坐标的斜率: 3676/320=11.4875
  y坐标的斜率: 3766/480=7.84583
//4. 得到实际的像素坐标
  x坐标:  320-(模拟量-155)/11.4875
  y坐标:  480-(模拟量-168)/7.84583
*/
u8 XPT2046_ReadXY(void) {
    if (XPT2046_PEN == 0) //判断触摸屏是否按下
    {
        /*1. 得到物理坐标*/
        xpt2046_touch.x0 = XPT2046_ReadData(0x90);
        xpt2046_touch.y0 = XPT2046_ReadData(0xD0);

        xpt2046_touch.x = xpt2046_touch.x0;
        xpt2046_touch.y = xpt2046_touch.y0;
        /*2. 得到像素坐标*/
        return 1;
    }
    return 0;
}
