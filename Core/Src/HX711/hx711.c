/************************************************************************************

*************************************************************************************/
#include "hx711.h"

u32 HX711_Buffer;
u32 Weight_Maopi;
s32 Weight_Shiwu;
u8 Flag_Error = 0;

float P = 1;
float P_; // 对应公式中的p'
float X = 0;
float X_; // X'
float K = 0;
float Q = 0.01; // 噪声
// float R=0.2;  //R如果很大，更相信预测值，那么传感器反应就会迟钝，反之相反
float R = 0.5;
float distance = 0;
float distance1 = 0;
float KLM(float Z) {
  X_ = X + 0;
  P_ = P + Q;
  K = P_ / (P_ + R);
  X = X_ + K * (Z - X_);
  P = P_ - K * P_;
  return X;
}

// 校准参数
// 因为不同的传感器特性曲线不是很一致，因此，每一个传感器需要矫正这里这个参数才能使测量值很准确。
// 当发现测试出来的重量偏大时，增加该数值。
// 如果测试出来的重量偏小时，减小改数值。
// 该值可以为小数
#define GapValue 106.5

//****************************************************
// 读取HX711
//****************************************************
u32 HX711_Read(void) // 增益128
{
  unsigned long count;
  unsigned char i;
  HX711_DOUT = 1;
  delay_us(1);
  HX711_SCK = 0;
  count = 0;
  while (HX711_DOUT)
    ;
  for (i = 0; i < 24; i++) {
    HX711_SCK = 1;
    count = count << 1;
    delay_us(1);
    HX711_SCK = 0;
    if (HX711_DOUT)
      count++;
    delay_us(1);
  }
  HX711_SCK = 1;
  count = count ^ 0x800000; // 第25个脉冲下降沿来时，转换数据
  delay_us(1);
  HX711_SCK = 0;
  return (count);
}

//****************************************************
// 获取毛皮重量
//****************************************************
void Get_Maopi(void) { Weight_Maopi = HX711_Read(); }

//****************************************************
// 称重
//****************************************************
void Get_Weight(void) {
  HX711_Buffer = HX711_Read();
  if (HX711_Buffer > Weight_Maopi) {
    Weight_Shiwu = HX711_Buffer;
    Weight_Shiwu = Weight_Shiwu - Weight_Maopi; // 获取实物的AD采样数值。

    Weight_Shiwu =
        (s32)((float)Weight_Shiwu / GapValue) -
        478; // 计算实物的实际重量
             // 因为不同的传感器特性曲线不一样，因此，每一个传感器需要矫正这里的GapValue这个除数。
             // 当发现测试出来的重量偏大时，增加该数值。
             // 如果测试出来的重量偏小时，减小改数值。
    Weight_Shiwu = KLM(Weight_Shiwu);
  }
}