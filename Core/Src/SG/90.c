//
// Created by nebula on 2026/1/14.
//

#include "90.h"

#include "tim.h"

void DuoJi_Angle_TIM2(int angle, int time)
{
    int PWM;
    PWM = 10 * angle / 9;
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, PWM);
    HAL_Delay(time);
}
