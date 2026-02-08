/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define RING_Pin GPIO_PIN_13
#define RING_GPIO_Port GPIOC
#define HOT_Pin GPIO_PIN_14
#define HOT_GPIO_Port GPIOC
#define DEBUG_TX_Pin GPIO_PIN_2
#define DEBUG_TX_GPIO_Port GPIOA
#define DEBUG_RX_Pin GPIO_PIN_3
#define DEBUG_RX_GPIO_Port GPIOA
#define SEN_Pin GPIO_PIN_4
#define SEN_GPIO_Port GPIOA
#define HX_DO_Pin GPIO_PIN_14
#define HX_DO_GPIO_Port GPIOB
#define HX_CLK_Pin GPIO_PIN_15
#define HX_CLK_GPIO_Port GPIOB
#define DHT_Pin GPIO_PIN_10
#define DHT_GPIO_Port GPIOA
#define NET_TX_Pin GPIO_PIN_6
#define NET_TX_GPIO_Port GPIOB
#define NET_RX_Pin GPIO_PIN_7
#define NET_RX_GPIO_Port GPIOB
#define SG_Pin GPIO_PIN_8
#define SG_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
