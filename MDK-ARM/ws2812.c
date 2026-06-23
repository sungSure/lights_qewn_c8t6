#include "ws2812.h"
#include "tim.h" // 包含定时器头文件，用于访问 htim2

// DMA缓冲区，存放所有LED的PWM占空比数据
uint16_t ws2812_dma_buf[DMA_BUF_SIZE];

/**
  * @brief  设置单个LED的颜色
  * @param  led_index: LED的索引 (0 到 LED_COUNT-1)
  * @param  color: RGB颜色结构体
  */
void WS2812_SetPixelColor(uint16_t led_index, RGB_Color_TypeDef color) {
    if (led_index >= LED_COUNT) return; // 防止数组越界

    // WS2812的数据顺序是 G, R, B
    uint32_t grb_data = ((uint32_t)color.G << 16) | ((uint32_t)color.R << 8) | (color.B);

    // 将24位GRB数据转换为PWM占空比数组
    for (int i = 0; i < 24; i++) {
        // 从最高位(MSB)开始处理
        if (grb_data & (1 << (23 - i))) {
            ws2812_dma_buf[led_index * 24 + i] = WS2812_HIGH; // 位为1
        } else {
            ws2812_dma_buf[led_index * 24 + i] = WS2812_LOW;  // 位为0
        }
    }
}

/**
  * @brief  通过DMA发送数据，更新所有LED显示
  */
void WS2812_Show(void) {
    // 添加复位信号（一段低电平）
    for (int i = 0; i < RESET_BITS; i++) {
        ws2812_dma_buf[(LED_COUNT * 24) + i] = 0;
    }

    // 启动DMA传输
    // 注意：这里使用HAL_TIM_PWM_Start_DMA，它会自动处理DMA的启动
    HAL_TIM_PWM_Start_DMA(&htim2, TIM_CHANNEL_1, (uint32_t*)ws2812_dma_buf, DMA_BUF_SIZE);
}

/**
  * @brief  清空所有LED (设置为黑色)
  */
void WS2812_Clear(void) {
    RGB_Color_TypeDef black = {0, 0, 0};
    for (int i = 0; i < LED_COUNT; i++) {
        WS2812_SetPixelColor(i, black);
    }
    WS2812_Show();
}