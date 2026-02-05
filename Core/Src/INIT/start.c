//
// Created by nebula on 2026/1/14.
//

#include "start.h"

#include "delay.h"
#include "hx711.h"
#include "tim.h"
#include "uart1.h"
#include "uart2.h"

void init_mcu()
{
    INIT_UART1();
    INIT_UART2();

    Get_Maopi();
    delay_ms(1000);
    delay_ms(1000);
    Get_Maopi();

    HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_1);
}
