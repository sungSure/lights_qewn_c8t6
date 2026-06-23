#ifndef __EFFECTS_H__          // 防止重复包含（必须）
#define __EFFECTS_H__

#include "stm32f1xx.h"     // 包含必要的库头文件
#include "ws2812.h"

uint8_t interupt = 1;

RGB_Color_TypeDef red   = {255, 0, 0};
RGB_Color_TypeDef green = {0, 255, 0};
RGB_Color_TypeDef blue  = {0, 0, 255};
RGB_Color_TypeDef white = {180,220,230};
RGB_Color_TypeDef color = {255, 180, 100};//默认暖白光

// 函数声明（告诉编译器这些函数存在）
void LED_Init(void);       // 初始化函数
void LED_On(void);         // 开灯
void LED_Off(void);        // 关灯
void LED_Toggle(void);     // 翻转状态

// 宏定义（可选）
#define LED_PIN  GPIO_PIN_13
#define LED_PORT GPIOC

#endif