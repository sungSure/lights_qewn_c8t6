#ifndef __EFFECTS_H__          // 防止重复包含（必须）
#define __EFFECTS_H__

#include "stm32f1xx.h"     // 包含必要的库头文件
#include "ws2812.h"

extern uint8_t interupt ;
extern uint8_t color_switch[3];//通过按键切换颜色的数组

// 函数声明（告诉编译器这些函数存在）
void singleColor(RGB_Color_TypeDef color);
void flow(RGB_Color_TypeDef color);
void runningLight(RGB_Color_TypeDef color);
void convergeAndDiverge(RGB_Color_TypeDef color);
void fillProgressive(RGB_Color_TypeDef color);
void meteorTrail(RGB_Color_TypeDef color);
void color_preview(RGB_Color_TypeDef color);

// 宏定义（可选）
#define LED_PIN  GPIO_PIN_13
#define LED_PORT GPIOC

#endif