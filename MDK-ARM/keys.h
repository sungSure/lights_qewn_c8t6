#ifndef __KEYS_H__          // 防止重复包含（必须）
#define __KEYS_H__

#include "stm32f1xx.h"     // 包含必要的库头文件

uint8_t is_color_switching;
uint8_t color_switch[3]={255, 180, 100};//通过按键切换颜色的数组，预设米色
uint8_t mode = 0;
uint32_t tick=0;

uint8_t mode_switch_Key_GetPress(void);
uint8_t color_switch_key_GetPress(void);
uint8_t Key_GetPress(void);

#define SWITCH_GPIO_PORT GPIOA
#define MODE_SWITCH_KEY GPIO_PIN_8
#define COLOR_SWITCH_KEY GPIO_PIN_9

#endif