//
// Created by ALIEN on 2023/7/24.
//
#include "func.h"

void set_lcd_brightness(uint8_t lcd_brightness) {
    TIM2->CCR4 = lcd_brightness;
}

uint8_t reverse_bit8(uint8_t data) {
    uint8_t res = 0;
    for (uint8_t i = 0; i < 8; i++) {
        res <<= 1;
        res |= data & 0x01;
        data >>= 1;
    }
    return res;
}
