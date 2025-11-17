/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    uart2.c
 * @brief   UART2 工具函数实现文件
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
#include "uart2.h"

/* Private variables ---------------------------------------------------------*/
static uint8_t rx_data[256]; // 接收缓冲区
static uint8_t tx_data[256]; // 发送缓冲区
static bool is_rx_complete = false;
static uint16_t rx_size = 0;

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief 通过DMA发送字符串
 * @param str 要发送的字符串
 * @retval HAL status
 */
HAL_StatusTypeDef UART2_TransmitString_DMA(const char *str) {
  /* USER CODE BEGIN UART2_TransmitString_DMA */
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
  return HAL_UART_Transmit_DMA(&huart2, tx_data, len);
  /* USER CODE END UART2_TransmitString_DMA */
}

/**
 * @brief 通过DMA接收数据
 * @param pData 接收数据缓冲区指针
 * @param Size 接收数据大小
 * @retval HAL status
 */
HAL_StatusTypeDef UART2_ReceiveData_DMA(uint8_t *pData, uint16_t Size) {
  /* USER CODE BEGIN UART2_ReceiveData_DMA */
  if (pData == NULL || Size == 0 || Size > sizeof(rx_data)) {
    return HAL_ERROR;
  }

  // 重置接收完成标志
  is_rx_complete = false;
  rx_size = 0;

  // 使用DMA接收数据
  return HAL_UART_Receive_DMA(&huart2, rx_data, Size);
  /* USER CODE END UART2_ReceiveData_DMA */
}

/**
 * @brief UART2接收完成回调函数
 * @param size 接收到的数据大小
 */
void UART2_RxCpltCallback(uint16_t size) {
  /* USER CODE BEGIN UART2_RxCpltCallback */
  rx_size = size;
  is_rx_complete = true;

  // 在这里可以添加对接收到数据的处理逻辑
  // 例如：解析命令、存储数据等

  // 示例：回显接收到的数据
  if (rx_size > 0) {
    HAL_UART_Transmit_DMA(&huart2, rx_data, rx_size);
  }
  /* USER CODE END UART2_RxCpltCallback */
}

/**
 * @brief 获取接收完成状态
 * @retval true:接收完成 false:接收未完成
 */
bool UART2_IsRxComplete(void) {
  /* USER CODE BEGIN UART2_IsRxComplete */
  return is_rx_complete;
  /* USER CODE END UART2_IsRxComplete */
}

/**
 * @brief 获取接收到的数据大小
 * @retval 接收到的数据大小
 */
uint16_t UART2_GetRxSize(void) {
  /* USER CODE BEGIN UART2_GetRxSize */
  return rx_size;
  /* USER CODE END UART2_GetRxSize */
}

/**
 * @brief 获取接收缓冲区指针
 * @retval 接收缓冲区指针
 */
uint8_t *UART2_GetRxBuffer(void) {
  /* USER CODE BEGIN UART2_GetRxBuffer */
  return rx_data;
  /* USER CODE END UART2_GetRxBuffer */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */