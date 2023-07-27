//
// Created by ALIEN on 2023/7/24.
//

#include "user.h"


void init() {
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_USART1_UART_Init();

    MX_TIM2_Init();
    MX_TIM6_Init();

    HAL_TIM_Base_Start_IT(&htim6);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);

    gui_init();
    set_lcd_brightness(100);
}

int main() {
    init();

    while (1) {
        lv_task_handler();
    }
}
