#ifndef __KEYS_H__          // 防止重复包含（必须）
#define __KEYS_H__

#include "stm32f1xx.h"     // 包含必要的库头文件

extern uint8_t is_color_switching;
extern uint8_t mode;
extern uint32_t tick;

uint8_t mode_switch_Key_GetPress(void);
uint8_t color_switch_key_GetPress(void);
uint8_t Key_GetPress(void);

#define SWITCH_GPIO_PORT GPIOA
#define MODE_SWITCH_KEY GPIO_PIN_8
#define COLOR_SWITCH_KEY GPIO_PIN_9

#endif