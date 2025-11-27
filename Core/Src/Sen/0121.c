#include "0121.h"
#include "adc.h"

uint16_t Read_SEN_ADC_Channel(uint32_t channel) {
  ADC_ChannelConfTypeDef sConfig = {0};

  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
    /* 错误处理 */
    Error_Handler();
  }

  /* 启动ADC转换 */
  HAL_ADC_Start(&hadc1);

  /* 等待转换完成 */
  if (HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY) != HAL_OK) {
    /* 错误处理 */
    Error_Handler();
  }

  /* 获取转换结果 */
  uint16_t adc_value = HAL_ADC_GetValue(&hadc1);

  /* 停止ADC转换 */
  HAL_ADC_Stop(&hadc1);

  return adc_value;
}