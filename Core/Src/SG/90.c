//
// Created by nebula on 2026/1/14.
//

#include "90.h"

#include "tim.h"


void set_pwm_param(TIM_HandleTypeDef htim, uint32_t Channel, uint32_t freq, uint16_t duty)
{
    uint16_t prescaler = 72-1;
    uint64_t tim_clk_freq = 72000000;
    //计算PWM频率，所对应的自动重装载值   ---> ARR = 主频 / (预分频+1) / 预期PWM频率(Hz) - 1
    float pwm_freq_arr  = (tim_clk_freq * 1.0) / (prescaler+1) / freq * 1.0 - 1;
    //计算PWM占空比，所对应比较寄存器的值 ---> CCR = 预期占空比 * (自动重装载值+1)
    //占空比则由捕获/比较寄存器（TIMx_CRx）寄存器决定。占空比:duty = Pluse / (ARR+1)
    float pwm_duty_pulse = duty * 1.0 / 100 * (pwm_freq_arr + 1);

    //配置PSC预分频值
    __HAL_TIM_SET_PRESCALER(&htim, prescaler);
    //配置PWM频率 ARR
    __HAL_TIM_SetAutoreload(&htim, (uint16_t)pwm_freq_arr);
    //配置PWM占空比
    __HAL_TIM_SetCompare(&htim, Channel, (uint16_t)pwm_duty_pulse);
}

void DuoJi_Angle_OPEN()
{
set_pwm_param(htim4, TIM_CHANNEL_3, 50, 3);

    HAL_Delay(1000);
}

void DuoJi_Angle_CLOSE()
{
set_pwm_param(htim4, TIM_CHANNEL_3, 50, 8);
    HAL_Delay(1000);
}