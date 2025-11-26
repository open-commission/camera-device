#include "stm32f1xx_hal.h"

void delay_us(uint32_t us) {
  // 计算LOAD寄存器值
  uint32_t reload = us * 9 * 8; // 假设主频为72MHz，AHB/8 = 9MHz
  if (reload > 0x00FFFFFF) {
    return; // 超出24位计数器范围
  }

  // 配置SYSTICK
  SysTick->LOAD = reload - 1; // 设置计数值
  SysTick->VAL = 0;           // 清零当前计数值
  SysTick->CTRL = 0x00000005; // 时钟源为AHB/8，禁止中断，使能SYSTICK

  // 等待计数到0
  do {
    __asm("nop"); // 空操作，防止优化
  } while (!(SysTick->CTRL & 0x00010000)); // 等待COUNTFLAG置位

  // 关闭SYSTICK
  SysTick->CTRL = 0;
  SysTick->LOAD = 0;
  SysTick->VAL = 0;
}

void delay_ms(uint32_t ms) {
  uint32_t i;
  for (i = 0; i < ms; i++) {
    delay_us(1000); // 调用微秒级延时函数
  }
}