/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    uart2.c
 * @brief   UART1 工具函数实现文件
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
#include "uart1.h"
#include "stm32f1xx_hal_uart.h"

/* Private variables ---------------------------------------------------------*/
static uint8_t tx_data[256]; // 发送缓冲区
static uint8_t rx_data[256]; // 接收缓冲区

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief 通过DMA发送字符串
 * @param str 要发送的字符串
 * @retval HAL status
 */
HAL_StatusTypeDef UART1_TransmitString_DMA(const char *str) {
  /* USER CODE BEGIN UART1_TransmitString_DMA */
  if (str == NULL) {
    return HAL_ERROR;
  }

  uint16_t len = strlen(str);
  if (len >= sizeof(tx_data)) {
    return HAL_ERROR;
  }

  // 复制字符串到发送缓冲区
  memcpy(tx_data, str, len);

  // 使用DMA发送数据
  return HAL_UART_Transmit_DMA(&huart1, tx_data, len);
  /* USER CODE END UART1_TransmitString_DMA */
}

void INIT_UART1(void) {
  // 重新启动接收，使用Ex函数，接收不定长数据
  HAL_UARTEx_ReceiveToIdle_DMA(&huart1, rx_data, sizeof(rx_data));
  // 关闭DMA传输过半中断（HAL库默认开启，但我们只需要接收完成中断）
  __HAL_DMA_DISABLE_IT(huart1.hdmarx, DMA_IT_HT);
}

// 不定长数据接收完成回调函数
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {
  if (huart->Instance == USART2) {
    // 使用DMA将接收到的数据发送回去
    HAL_UART_Transmit_DMA(&huart1, rx_data, Size);
    // 重新启动接收，使用Ex函数，接收不定长数据
    HAL_UARTEx_ReceiveToIdle_DMA(&huart1, rx_data, sizeof(rx_data));
    // 关闭DMA传输过半中断（HAL库默认开启，但我们只需要接收完成中断）
    __HAL_DMA_DISABLE_IT(huart1.hdmarx, DMA_IT_HT);
  }
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */