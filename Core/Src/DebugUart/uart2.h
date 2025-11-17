/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    uart2.h
  * @brief   UART2 工具函数头文件
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __UART2_H__
#define __UART2_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

/**
  * @brief 通过DMA发送字符串
  * @param str 要发送的字符串
  * @retval HAL status
  */
HAL_StatusTypeDef UART2_TransmitString_DMA(const char* str);

/**
  * @brief 通过DMA接收数据
  * @param pData 接收数据缓冲区指针
  * @param Size 接收数据大小
  * @retval HAL status
  */
HAL_StatusTypeDef UART2_ReceiveData_DMA(uint8_t* pData, uint16_t Size);

/**
  * @brief UART2接收完成回调函数
  * @param size 接收到的数据大小
  */
void UART2_RxCpltCallback(uint16_t size);

/**
  * @brief 获取接收完成状态
  * @retval true:接收完成 false:接收未完成
  */
bool UART2_IsRxComplete(void);

/**
  * @brief 获取接收到的数据大小
  * @retval 接收到的数据大小
  */
uint16_t UART2_GetRxSize(void);

/**
  * @brief 获取接收缓冲区指针
  * @retval 接收缓冲区指针
  */
uint8_t* UART2_GetRxBuffer(void);
/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __UART2_H__ */