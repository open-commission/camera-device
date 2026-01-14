/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    uart2.c
 * @brief   UART3 工具函数实现文件
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "uart3.h"
#include "stm32f1xx_hal_uart.h"

/* Private variables ---------------------------------------------------------*/
static uint8_t tx_data[256]; // 发送缓冲区
static uint8_t rx_data[256]; // 接收缓冲区

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

void INIT_UART3(void) {
  // 重新启动接收，使用Ex函数，接收不定长数据
  HAL_UARTEx_ReceiveToIdle_DMA(&huart3, rx_data, sizeof(rx_data));
  // 关闭DMA传输过半中断（HAL库默认开启，但我们只需要接收完成中断）
  __HAL_DMA_DISABLE_IT(huart3.hdmarx, DMA_IT_HT);
}

// 不定长数据接收完成回调函数
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {
  if (huart->Instance == USART2) {
    // 使用DMA将接收到的数据发送回去
    HAL_UART_Transmit_DMA(&huart3, rx_data, Size);
    // 重新启动接收，使用Ex函数，接收不定长数据
    HAL_UARTEx_ReceiveToIdle_DMA(&huart3, rx_data, sizeof(rx_data));
    // 关闭DMA传输过半中断（HAL库默认开启，但我们只需要接收完成中断）
    __HAL_DMA_DISABLE_IT(huart3.hdmarx, DMA_IT_HT);
  }
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */