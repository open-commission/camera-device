/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "DebugUart/uart2.h"
#include "Delay/delay.h"
#include "Dht11/11.h"
#include "HX711/hx711.h"
#include "Sen/0121.h"
#include "math.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "90.h"
#include "start.h"
#include "uart1.h"
#include "uart3.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_ADC1_Init();
  MX_USART3_UART_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */

    // 发送提示信息
    UART2_TransmitString_DMA("MCU Started...\r\n");

    init_mcu();

    extern uint8_t Data[5];

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    while (1)
    {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
        Get_Weight();

        char weight_str[32];
        sprintf(weight_str, "%d\r\n", Weight_Shiwu);
        UART2_TransmitString_DMA(weight_str);

        uint16_t senValue = Read_SEN_ADC_Channel(ADC_CHANNEL_4);
        char sen_str[32];
        sprintf(sen_str, "%d\r\n", senValue);
        UART2_TransmitString_DMA(sen_str);

        if (DHT_Read())
        {
            uint8_t* DHTValue1 = (uint8_t*)Data + 0;
            uint8_t* DHTValue2 = (uint8_t*)Data + 2;
            char dht_str[32];
            sprintf(dht_str, "%d\r\n%d\r\n\r\n", *DHTValue1, *DHTValue2);
            UART2_TransmitString_DMA(dht_str);
        }

        delay_ms(1000);

        HAL_GPIO_WritePin(HX_CLK_GPIO_Port, HX_CLK_Pin, GPIO_PIN_SET);
        delay_us(20);
        HAL_GPIO_WritePin(HX_CLK_GPIO_Port, HX_CLK_Pin, GPIO_PIN_RESET);
        delay_us(20);


      DuoJi_Angle_TIM2(0 , 1000);
      DuoJi_Angle_TIM2(45 , 1000);
      DuoJi_Angle_TIM2(90 , 1000);
      DuoJi_Angle_TIM2(135 , 1000);
      DuoJi_Angle_TIM2(180, 1000);
    }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1)
    {
    }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line
       number, ex: printf("Wrong parameters value: file %s on line %d\r\n", file,
       line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */



//测试专用


#include "main.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

/* --- 硬件句柄 (由CubeMX生成) --- */
extern UART_HandleTypeDef huart1;

/* --- printf 重定向 --- */
int __io_putchar(int ch) {
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);
    return ch;
}

/* --- 数据结构 --- */
typedef struct {
    char name[10];
    char trigger_key;
    float current_val;
    float base_val;
    float target_val;
    float step;
    bool is_active;
} sensor_attr_t;

typedef struct {
    char name[10];
    char trigger_key;
    uint16_t gpio_pin;
    GPIO_TypeDef* gpio_port;
    bool state;
} status_attr_t;

/* --- 可配置配置区 --- */
sensor_attr_t sensors[] = {
    {"Temp", '1', 25.0f, 25.0f, 40.0f, 0.5f, false},
    {"Humi", '2', 50.0f, 50.0f, 85.0f, 1.5f, false},
    {"Lux",  '3', 100.0f, 100.0f, 900.0f, 20.0f, false}
};

status_attr_t status_devs[] = {
    {"Relay", '5', GPIO_PIN_13, GPIOC, false} // 假设继电器在PC13
};

#define SENSOR_COUNT (sizeof(sensors)/sizeof(sensor_attr_t))
#define STATUS_COUNT (sizeof(status_devs)/sizeof(status_attr_t))

/* --- 全局变量 --- */
uint8_t rx_byte; // 接收缓冲区
uint32_t last_print_time = 0;

/* --- 中断回调函数 --- */
// 只要串口接收到一个字符，硬件就会自动调用这个函数
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART1) {
        // 1. 检查传感器触发
        for (int i = 0; i < SENSOR_COUNT; i++) {
            if (rx_byte == sensors[i].trigger_key) {
                sensors[i].is_active = !sensors[i].is_active;
            }
        }
        // 2. 检查状态切换
        for (int i = 0; i < STATUS_COUNT; i++) {
            if (rx_byte == status_devs[i].trigger_key) {
                status_devs[i].state = !status_devs[i].state;
                HAL_GPIO_WritePin(status_devs[i].gpio_port, status_devs[i].gpio_pin,
                                 status_devs[i].state ? GPIO_PIN_SET : GPIO_PIN_RESET);
            }
        }
        // 3. 重新开启中断接收（HAL库特性，每次接收完需要手动重新开启）
        HAL_UART_Receive_IT(&huart1, &rx_byte, 1);
    }
}

int main(void) {
    // 基础硬件初始化 (由CubeMX自动生成)
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_USART1_UART_Init();

    // 首次开启串口中断接收
    HAL_UART_Receive_IT(&huart1, &rx_byte, 1);

    while (1) {
        uint32_t current_time = HAL_GetTick();

        // --- 逻辑处理与定时输出 (每1000ms) ---
        if (current_time - last_print_time >= 1000) {
            last_print_time = current_time;

            // 1. 更新传感器模拟数值（实现缓慢变化）
            for (int i = 0; i < SENSOR_COUNT; i++) {
                float goal = sensors[i].is_active ? sensors[i].target_val : sensors[i].base_val;
                if (sensors[i].current_val < goal) {
                    sensors[i].current_val += sensors[i].step;
                    if (sensors[i].current_val > goal) sensors[i].current_val = goal;
                } else if (sensors[i].current_val > goal) {
                    sensors[i].current_val -= sensors[i].step;
                    if (sensors[i].current_val < goal) sensors[i].current_val = goal;
                }
            }

            // 2. 静默打印输出
            printf(">> ");
            for (int i = 0; i < SENSOR_COUNT; i++) {
                printf("%s: %.1f | ", sensors[i].name, sensors[i].current_val);
            }
            for (int i = 0; i < STATUS_COUNT; i++) {
                printf("%s: %s ", status_devs[i].name, status_devs[i].state ? "ON " : "OFF");
            }
            printf("\r\n");
        }

        // 这里可以处理其他不干扰定时的业务逻辑
    }
}