//
// Created by ALIEN on 2023/7/24.
//

#include "user.h"


void init() {
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_TIM2_Init();
    MX_TIM6_Init();

    HAL_TIM_Base_Start_IT(&htim6);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);

    LCD_Init();
    LCD_Fill(0, 0, LCD_WIDTH, LCD_HEIGHT, WHITE);
    set_lcd_brightness(100);
}

int main() {
    init();

    int count = 0;
    uint16_t pos[4][2];
    for (int i = 0; i < 4; ++i) {
        pos[i][0] = 0;
        pos[i][1] = 0;
    }
    bool flag;
    while (count < 4) {
        LCD_Fill(20 + (LCD_WIDTH - 50) * (count / 2), 20 + (LCD_HEIGHT - 50) * (count % 2),
                 20 + (LCD_WIDTH - 50) * (count / 2) + 10, 20 + (LCD_HEIGHT - 50) * (count % 2) + 10, RED);

        flag = true;
        while (flag == true) {
            if (XPT2046_PEN == 0) {
                HAL_Delay(20);
                if (XPT2046_PEN == 0) {
                    while (XPT2046_PEN == 0) {
                        pos[count][0] = XPT2046_ReadData(0x90);
                        pos[count][1] = XPT2046_ReadData(0xD0);
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
                    while (XPT2046_PEN == 1) {};
                    flag = false;
                }
            }
        }
    }

    char c[10];
    for (int i = 0; i < 4; i++) {
        sprintf(c, "%d   %d", pos[i][0], pos[i][1]);
        LCD_String(50, 50 + 20 * i, c, 12, BLACK, WHITE);
    }


    while (1) {
        LCD_Fill(100, 300, 200, 400, WHITE);
        if (XPT2046_PEN == 0) {
            XPT2046_ReadXY();
            sprintf(c, "%d %d", xpt2046_touch.x, xpt2046_touch.y);
            LCD_String(100, 300, c, 12, BLACK, WHITE);
        } else {
            sprintf(c, "%d %d", 0, 0);
            LCD_String(100, 300, c, 12, BLACK, WHITE);
        }
        HAL_Delay(200);
    }
}
