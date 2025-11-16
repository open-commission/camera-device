#ifndef __TTP229_H__
#define __TTP229_H__
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

/* USER CODE BEGIN Prototypes */
void TTP229_Init(void);
uint16_t TTP229_ReadKeys(void);
uint16_t TTP229_ReadKeysStable(void);  // 添加稳定读取函数
void TTP229_SendKeysViaUART(uint16_t keys);

// TTP229 驱动函数声明
void SetSCL(GPIO_PinState v);
GPIO_PinState GetSDO(void);
uint8_t ReadKey8(void);
uint8_t GetKey8(void);
uint8_t ReadKeys8(void);
uint8_t GetKeys8(void);
uint8_t ReadKey16(void);
uint8_t GetKey16(void);
uint16_t ReadKeys16(void);
uint16_t GetKeys16(void);
void Key8(void);
void Keys8(void);
void Key16(void);
void Keys16(void);
bool GetBit(void);
bool IsTouch(void);
void WaitForTouch(void);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __TTP229_H__ */