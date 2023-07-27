//
// Created by ALIEN on 2023/7/24.
//
#include "func.h"

void set_lcd_brightness(uint8_t lcd_brightness) {
    TIM2->CCR4 = lcd_brightness;
}

void debug_add_text(char *string) {
    lv_textarea_add_text(ui->screen_ta_1, string);
    lv_task_handler();
}

void debug_text(char *format, ...) {
    va_list va;
    va_start(va, format);
    char string[100];
    sprintf(string, format, va);
    printf("%s", string);
    va_end(va);
}
