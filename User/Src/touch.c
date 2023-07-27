#include "touch.h"

struct Touch touch;

/*
 * url: https://blog.csdn.net/xiaolong1126626497/article/details/118120066
 */

void delay_us(uint32_t us) {
    uint32_t delay = (HAL_RCC_GetHCLKFreq() / 4000000 * us);
    while (delay--) {};
}


void touch_init(void) {
    MX_I2C1_Init();
    uint8_t corrected = 0;
    AT24C02_read_8bit(TOUCH_DATA_BASE_ADDR + 8, &corrected);
    if (corrected) {
        touch_read_para();
    } else {
        touch_correct();
        touch_write_para();
    }
}

void touch_read_para() {
    AT24C02_read_16bit(TOUCH_DATA_BASE_ADDR + 0, &(touch.x_offset));
    AT24C02_read_16bit(TOUCH_DATA_BASE_ADDR + 2, &(touch.y_offset));
    uint16_t tmp;
    AT24C02_read_16bit(TOUCH_DATA_BASE_ADDR + 4, &tmp);
    touch.x_fraction = (float) tmp / 50000;
    AT24C02_read_16bit(TOUCH_DATA_BASE_ADDR + 6, &tmp);
    touch.y_fraction = (float) tmp / 50000;
}

void touch_write_para() {
    AT24C02_write_16bit(TOUCH_DATA_BASE_ADDR + 0, touch.x_offset);
    AT24C02_write_16bit(TOUCH_DATA_BASE_ADDR + 2, touch.y_offset);
    AT24C02_write_16bit(TOUCH_DATA_BASE_ADDR + 4, (uint16_t) (touch.x_fraction * 50000));
    AT24C02_write_16bit(TOUCH_DATA_BASE_ADDR + 6, (uint16_t) (touch.y_fraction * 50000));
    AT24C02_write_8bit(TOUCH_DATA_BASE_ADDR + 8, 1);
}

uint8_t touch_scan() {
    if (XPT2046_PEN == 0) {
        touch_read_xy();
    } else {
        if ((touch.sta & TOUCH_PRESS_DOWN)) {
            touch.sta &= ~(1 << 7);
        } else {
            touch.x0 = 0;
            touch.y0 = 0;
            touch.x = 0xffff;
            touch.y = 0xffff;
        }
    }
    return touch.sta & TOUCH_PRESS_DOWN;
}

void touch_write_byte(u8 cmd) {
    u8 i;
    for (i = 0; i < 8; i++) {
        XPT2046_SCK(0); //低电平写
        if (cmd & 0x80) {
            XPT2046_MOSI(1);
        } else {
            XPT2046_MOSI(0);
        }
        cmd <<= 1;
        XPT2046_SCK(1); //高电平读,保证数据线稳定
    }
}


u16 touch_read_data(u8 cmd) {
    u16 data = 0;
    u8 i;
    XPT2046_CS(0);  //选中XPT2046
    XPT2046_MOSI(0);
    XPT2046_SCK(0);
    touch_write_byte(cmd);

    delay_us(8); //0.008ms ,等待XPT2046转换完成。
    //消除忙信号
    XPT2046_SCK(0);
    delay_us(1);
    XPT2046_SCK(1);

    //连续读取16位的数据
    for (i = 0; i < 16; i++) {
        XPT2046_SCK(0); //通知XPT2046,主机需要数据
        XPT2046_SCK(1);
        data <<= 1;
        if (XPT2046_MISO)
            data |= 0x01;
    }
    data >>= 4; //丢弃最低4位
    XPT2046_CS(1); //取消选中
    return data;
}

u8 touch_read_xy(void) {
    uint16_t x, y;
    /*1. 得到物理坐标*/
    touch.x0 = touch_read_data(0x90);
    touch.y0 = touch_read_data(0xD0);
    touch.x = touch.x0;
    touch.y = touch.y0;

    x = touch.x_fraction * (touch.x - touch.x_offset);
    y = touch.y_fraction * (touch.y - touch.y_offset);
    x = x > LCD_WIDTH ? LCD_WIDTH : x;
    y = y > LCD_HEIGHT ? LCD_HEIGHT : y;
    x = x < 0 ? 0 : x;
    y = y < 0 ? 0 : y;

    touch.x = x;
    touch.y = y;
    return 0;
}


void touch_correct() {
    int count = 0;
    uint16_t pos[4][2];
    for (int i = 0; i < 4; ++i) {
        pos[i][0] = 0;
        pos[i][1] = 0;
    }
    bool flag;
    char c[10];

    while (count < 4) {
        LCD_Fill(20 + (LCD_WIDTH - 50) * (count % 2), 20 + (LCD_HEIGHT - 50) * (count / 2),
                 20 + (LCD_WIDTH - 50) * (count % 2) + 10, 20 + (LCD_HEIGHT - 50) * (count / 2) + 10, RED);

        flag = true;
        while (flag == true) {
            if (XPT2046_PEN == 0) {
                HAL_Delay(20);
                if (XPT2046_PEN == 0) {
                    while (XPT2046_PEN == 0) {
                        uint16_t tmp_x, tmp_y;
                        tmp_x = touch_read_data(0x90);
                        tmp_y = touch_read_data(0xD0);
                        if ((tmp_x not_eq 0) and (tmp_y not_eq 0)) {
                            pos[count][0] = tmp_x;
                            pos[count][1] = tmp_y;
                        }
                        sprintf(c, "%d   %d", pos[count][0], pos[count][1]);
                        LCD_String(50, 50 + 20 * count, c, 12, BLACK, WHITE);
                    };
                    flag = false;
                }
            }
        }
        count += 1;
        flag = true;
        while (flag == true) {
            if (XPT2046_PEN == 1) {
                HAL_Delay(20);
                if (XPT2046_PEN == 1) {
                    flag = false;
                }
            }
        }
    }

    int x_offset, y_offset;
    float x_fraction, y_fraction;
    x_fraction = (float) (LCD_WIDTH - 50.0) / (float) (((pos[2][0] + pos[0][0]) - (pos[3][0] + pos[1][0])) / 2);
    y_fraction = (float) (LCD_HEIGHT - 50.0) / (float) (((pos[1][1] + pos[0][1]) - (pos[3][1] + pos[2][1])) / 2);
    x_fraction = x_fraction > 0 ? x_fraction : -x_fraction;
    y_fraction = y_fraction > 0 ? y_fraction : -y_fraction;

    x_offset = ((pos[0][0] + pos[2][0])) / 2 - 25.0 / x_fraction;
    y_offset = ((pos[0][1] + pos[1][1])) / 2 - 25.0 / y_fraction;

    touch.x_fraction = x_fraction;
    touch.y_fraction = y_fraction;
    touch.x_offset = x_offset;
    touch.y_offset = y_offset;
}
