//
// Created by ALIEN on 2023/7/24.
//
#include "func.h"

void set_lcd_brightness(uint8_t lcd_brightness) {
    TIM2->CCR4 = lcd_brightness;
}
