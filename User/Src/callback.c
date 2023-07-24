//
// Created by ALIEN on 2023/7/24.
//

#include "callback.h"

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM6) {
        static int led_count = 0;
        if (led_count >= 100) {
            HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
            led_count = 0;
        }
        led_count += 1;
    }
}
