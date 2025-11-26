// #include "ttp229.h"

// #include "main.h"
// #include <stdio.h>
// #include <string.h>

// #include "gpio.h"
// #include "tim.h"

// extern UART_HandleTypeDef huart1;

// #define DLY_TIM_Handle (&htim3)
// void delay_us(uint16_t nus)
// {
// 	__HAL_TIM_SET_COUNTER(DLY_TIM_Handle, 0);
// 	__HAL_TIM_ENABLE(DLY_TIM_Handle);
// 	while (__HAL_TIM_GET_COUNTER(DLY_TIM_Handle) < nus)
// 	{
// 	}
// 	__HAL_TIM_DISABLE(DLY_TIM_Handle);
// }

// uint8_t _key8, _keys8, _key16;
// uint16_t _keys16;

// void SetSCL(GPIO_PinState v)
// {
// 		if(v == GPIO_PIN_SET)
// 			HAL_GPIO_WritePin(SCL_GPIO_Port, SCL_Pin, GPIO_PIN_SET);
// 		else
// 			HAL_GPIO_WritePin(SCL_GPIO_Port, SCL_Pin,
// GPIO_PIN_RESET);
// }

// GPIO_PinState GetSDO(void)
// {
// 	return HAL_GPIO_ReadPin(SDO_GPIO_Port, SDO_Pin);
// }

// uint8_t ReadKey8(void)
// {
// 	WaitForTouch();
// 	Key8();
// 	return _key8;
// }
// uint8_t GetKey8(void)
// {
// 	if (IsTouch()) Key8();
// 	return _key8;
// }
// uint8_t ReadKeys8(void)
// {
// 	WaitForTouch();
// 	Keys8();
// 	return _keys8;
// }
// uint8_t GetKeys8(void)
// {
// 	if (IsTouch()) Keys8();
// 	return _keys8;
// }
// uint8_t ReadKey16(void)
// {
// 	WaitForTouch();
// 	Key16();
// 	return _key16;
// }
// uint8_t GetKey16(void)
// {
// 	if (IsTouch()) Key16();
// 	return _key16;
// }
// uint16_t ReadKeys16(void)
// {
// 	WaitForTouch();
// 	Keys16();
// 	return _keys16;
// }
// uint16_t GetKeys16(void)
// {
// 	if (IsTouch()) Keys16();
// 	return _keys16;
// }

// void Key8(void)
// {
// 	_key8 = 0;
// 	for (uint8_t i = 0; i < 8; i++)
// 		if (GetBit()) _key8 = i + 1;
// 	HAL_Delay(2); // Tout
// }

// void Keys8(void)
// {
// 	_keys8 = 0;
// 	for (uint8_t i = 0; i < 8; i++)
// 		if (GetBit()) _keys8 |= 1 << i;
// 	HAL_Delay(2); // Tout
// }

// void Key16(void)
// {
// 	_key16 = 0;
// 	for (uint8_t i = 0; i < 16; i++)
// 		if (GetBit()) _key16 = i + 1;
// 	HAL_Delay(2); // Tout
// }

// void Keys16(void)
// {
// 	_keys16 = 0;
// 	for (uint8_t i = 0; i < 16; i++)
// 		if (GetBit()) _keys16 |= 1 << i;
// 	HAL_Delay(2); // Tout
// }

// bool GetBit(void)
// {
// 	SetSCL(GPIO_PIN_RESET);
// 	delay_us(2); // 500KHz
// 	bool retVal = !GetSDO();  //
// 读取SDO并取反，因为TTP229按键按下时SDO输出低电平 	SetSCL(GPIO_PIN_SET);
// 	delay_us(2); // 500KHz
// 	return retVal;
// }

// bool IsTouch(void)
// {
// 	uint16_t timeout = 5000; // 50ms timeout
// 	while (GetSDO()) // DV LOW
// 	{
// 		if (--timeout == 0) return false;
// 		delay_us(10);
// 	}
// 	while (!GetSDO()) // DV HIGH
// 	{
// 		if (--timeout == 0) return false;
// 		delay_us(10);
// 	}
// 	delay_us(10); // Tw
// 	return true;
// }

// void WaitForTouch(void)
// {
// 	while (GetSDO()) // DV LOW
// 	while (!GetSDO()) // DV HIGH
// 	delay_us(10); // Tw
// }

// /**
//  * @brief  从TTP229读取16个按键状态（稳定版）
//  * @retval 16位按键状态值，每一位代表一个按键的状态
//  */
// uint16_t TTP229_ReadKeysStable(void)
// {
//     uint16_t keys1, keys2, keys3;

//     // 连续读取3次，确保读取的稳定性
//     keys1 = ReadKeys16();
//     keys2 = ReadKeys16();
//     keys3 = ReadKeys16();

//     // 只有当至少两次读取结果一致时才认为是有效结果
//     if (keys1 == keys2) {
//         return keys1;
//     } else if (keys2 == keys3) {
//         return keys2;
//     } else if (keys1 == keys3) {
//         return keys1;
//     } else {
//         // 如果三次读取都不同，返回0表示无有效按键
//         return 0;
//     }
// }

// /**
//  * @brief  从TTP229读取16个按键状态
//  * @retval 16位按键状态值，每一位代表一个按键的状态
//  */
// uint16_t TTP229_ReadKeys(void)
// {
//     uint16_t keys = ReadKeys16();

//     char buffer[100];
//     int len = 0;

//     len += sprintf(buffer + len, "TTP229 Keys: 0x%04X (%d)", keys, keys);

//     // 检查是否有按键按下
//     if (keys != 0) {
//         len += sprintf(buffer + len, " Pressed:");
//         for (int i = 0; i < 16; i++) {
//             if (keys & (1 << i)) {
//                 len += sprintf(buffer + len, " K%d", i+1);
//             }
//         }
//     } else {
//         len += sprintf(buffer + len, " No key pressed");
//     }

//     len += sprintf(buffer + len, "\r\n");
//     HAL_UART_Transmit(&huart1, (uint8_t*)buffer, len, HAL_MAX_DELAY);

//     return keys;
// }
