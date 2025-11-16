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
#include "math.h"
#include <stdio.h>
#include <string.h>

#include "ttp229.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define SERIES_RESISTOR 10000.0    // NTC串联电阻值，10kΩ
#define NOMINAL_RESISTANCE 10000.0 // NTC标称电阻值（25°C时），10kΩ
#define NOMINAL_TEMP 25.0          // 标称温度（摄氏度）
#define B_COEFFICIENT 3950.0       // NTC的B值
#define ADC_MAX_VALUE 4095.0       // 12位ADC最大值
#define VREF 3.3                   // 参考电压3.3V
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint16_t adc_values[5]; // 存储ADC转换结果的数组
uint32_t adc_timestamp = 0; // 记录ADC转换时间戳
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
float ADC2Resistance(uint32_t adc_value);
float resistance2Temperature(float R1);
void float_to_string(float value, char* str, int decimals);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/**
  * @brief  通过ADC值计算NTC电阻值
  * @param  adc_value: ADC读取的数值，范围 0 - 4095
  * @retval NTC电阻值，浮点数类型，单位Ω
  */
float ADC2Resistance(uint32_t adc_value)
{
    // 防止除零错误
    if (adc_value >= ADC_MAX_VALUE)
    {
        adc_value = ADC_MAX_VALUE - 1;
    }
    if (adc_value == 0)
    {
        adc_value = 1;
    }
    return (adc_value / (4095.0f - adc_value)) * 10000.0f;
}

/**
  * @brief  通过NTC阻值计算温度
  * @param  R1: NTC电阻值
  * @retval 温度值，float类型，单位摄氏度
  */
float resistance2Temperature(float R1)
{
    float B = 3950.0f;
    float R2 = 10000.0f;
    float T2 = 25.0f;
    return (1.0f / ((1.0f / B) * log(R1 / R2) + (1.0f / (T2 + 273.15f))) - 273.15f);
}

/**
  * @brief  将浮点数转换为字符串
  * @param  value: 要转换的浮点数
  * @param  str: 存储结果的字符串缓冲区
  * @param  decimals: 小数位数
  * @retval None
  */
void float_to_string(float value, char* str, int decimals)
{
    // 处理负数
    int pos = 0;
    if (value < 0)
    {
        str[pos++] = '-';
        value = -value;
    }

    // 提取整数部分
    int integer_part = (int)value;

    // 提取小数部分
    float decimal_part = value - (float)integer_part;
    for (int i = 0; i < decimals; i++)
    {
        decimal_part *= 10;
    }
    int decimal_part_int = (int)(decimal_part + 0.5f); // 四舍五入

    // 转换整数部分为字符串
    char int_str[20];
    int int_pos = 0;

    if (integer_part == 0)
    {
        int_str[int_pos++] = '0';
    }
    else
    {
        int temp = integer_part;
        int_pos = 0;
        char temp_str[20];
        while (temp > 0)
        {
            temp_str[int_pos++] = '0' + (temp % 10);
            temp /= 10;
        }
        // 反转字符串
        for (int i = 0; i < int_pos; i++)
        {
            int_str[i] = temp_str[int_pos - 1 - i];
        }
    }
    int_str[int_pos] = '\0';

    // 复制整数部分到结果字符串
    int i = 0;
    while (int_str[i] != '\0')
    {
        str[pos++] = int_str[i++];
    }

    // 添加小数点
    if (decimals > 0)
    {
        str[pos++] = '.';

        // 处理小数部分前导零
        int decimal_divider = 1;
        for (int i = 1; i < decimals; i++)
        {
            decimal_divider *= 10;
        }

        while (decimal_part_int < decimal_divider && decimal_divider > 1)
        {
            str[pos++] = '0';
            decimal_divider /= 10;
        }

        // 转换小数部分为字符串
        if (decimal_part_int > 0)
        {
            char dec_str[20];
            int dec_pos = 0;

            int temp = decimal_part_int;
            if (temp == 0)
            {
                dec_str[dec_pos++] = '0';
            }
            else
            {
                char temp_str[20];
                int temp_pos = 0;
                while (temp > 0)
                {
                    temp_str[temp_pos++] = '0' + (temp % 10);
                    temp /= 10;
                }
                // 反转字符串
                for (int i = 0; i < temp_pos; i++)
                {
                    dec_str[i] = temp_str[temp_pos - 1 - i];
                }
                dec_pos = temp_pos;
            }
            dec_str[dec_pos] = '\0';

            // 复制小数部分到结果字符串
            i = 0;
            while (dec_str[i] != '\0')
            {
                str[pos++] = dec_str[i++];
            }
        }
        else if (decimal_part_int == 0)
        {
            str[pos++] = '0';
        }
    }

    str[pos] = '\0';
}
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
  MX_ADC1_Init();
  MX_USART1_UART_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
    // 启动ADC校准
    HAL_ADCEx_Calibration_Start(&hadc1);
    
    // 启动ADC DMA转换
    HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc_values, 5);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    while (1)
    {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
        // 获取IN4 (PA4) ADC值
        uint32_t in4_adc = adc_values[0];

        // 获取IN5 (PA5) ADC值
        uint32_t in5_adc = adc_values[1];

        // 获取内部温度传感器ADC值
        uint32_t temp_adc = adc_values[2];

        // 获取内部参考电压ADC值
        uint32_t vref_adc = adc_values[3];
        
        // 获取IN6 (PA6) 光照强度ADC值
        uint32_t light_adc = adc_values[4];

        // 将IN4 ADC值转换为NTC电阻值
        float in4_resistance = ADC2Resistance(in4_adc);

        // 将IN4电阻值转换为温度
        float in4_temperature = resistance2Temperature(in4_resistance);

        // 计算实际参考电压 (VREFINT标称值为1.2V)
        float vref_actual = 1.2f * 4095.0f / vref_adc;

        // 计算实际温度 (使用STM32内部温度传感器)
        // 公式: Temperature = {(VSENSE - V25) / Avg_Slope} + 25
        // V25 = 1.43V, Avg_Slope = 4.3mV/°C
        float temp_voltage = (temp_adc * vref_actual) / 4095.0f;
        float temperature = ((temp_voltage - 1.43f) / 0.0043f) + 25.0f;

        // 将值转换为字符串并输出
        char in4_res_str[20];
        char in4_temp_str[20];
        char in5_str[20];
        char temp_str[20];
        char vref_str[20];
        char light_str[20];

        float_to_string(in4_resistance, in4_res_str, 0);
        float_to_string(in4_temperature, in4_temp_str, 2);
        float_to_string((in5_adc * vref_actual) / 4095.0f, in5_str, 3);
        float_to_string(temperature, temp_str, 2);
        float_to_string(vref_actual, vref_str, 3);
        float_to_string((light_adc * vref_actual) / 4095.0f, light_str, 3);

        // 通过USART输出所有值
        char buffer[250];
        sprintf(buffer, "IN4_ADC: %lu, IN4_R: %s Ohm, IN4_T: %s C, IN5: %s V, LIGHT: %s V, VREF: %s V, TEMP: %s C\r\n",
                in4_adc, in4_res_str, in4_temp_str, in5_str, light_str, vref_str, temp_str);
        HAL_UART_Transmit(&huart1, (uint8_t*)buffer, strlen(buffer), HAL_MAX_DELAY);

        TTP229_ReadKeys();

        // 主循环中不需要做任何事情，所有处理都在中断回调函数中完成
        HAL_Delay(1000);
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
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
