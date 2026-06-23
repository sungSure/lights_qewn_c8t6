#ifndef __WS2812_H
#define __WS2812_H

#include "main.h"
#include <stdint.h>
#include <string.h>

// 定义LED数量
#define LED_COUNT 60

// 定义PWM周期的1/3和2/3，用于产生0和1
// ARR = 90, 1/3 * 90 = 30, 2/3 * 90 = 60
#define WS2812_LOW  30  // 对应逻辑 '0' 的占空比
#define WS2812_HIGH 60  // 对应逻辑 '1' 的占空比

// 每个LED需要24位数据 (G, R, B 各8位)
#define BITS_PER_LED 24
// 额外增加一些位用于复位（低电平），保证时序稳定
#define RESET_BITS 50
// DMA缓冲区总大小
#define DMA_BUF_SIZE (LED_COUNT * BITS_PER_LED + RESET_BITS)

// RGB颜色结构体
typedef struct {
    uint8_t R;
    uint8_t G;
    uint8_t B;
} RGB_Color_TypeDef;

// 函数声明
void WS2812_SetPixelColor(uint16_t led_index, RGB_Color_TypeDef color);
void WS2812_Show(void);
void WS2812_Clear(void);

#endif /* __WS2812_H */