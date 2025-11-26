#ifndef __HX711_H
#define __HX711_H

#include "stdint.h"

#define HX711_SCK PBout(0) // PB0
#define HX711_DOUT PBin(1) // PB1

extern uint32_t HX711_Read(void);
extern void Get_Maopi(void);
extern void Get_Weight(void);

extern uint32_t HX711_Buffer;
extern uint32_t Weight_Maopi;
extern int32_t Weight_Shiwu;
extern uint8_t Flag_Error;

#endif