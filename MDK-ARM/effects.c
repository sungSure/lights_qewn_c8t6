#include "effects.h"

 void singleColor(RGB_Color_TypeDef color){
        WS2812_Clear(); // 先清�?
        for (int i = 0; i < LED_COUNT; i++) {
            WS2812_SetPixelColor(i, color);
        }
        WS2812_Show(); // 更新显示
       HAL_Delay(80); // 延时1�?
			}

void flow(RGB_Color_TypeDef color){        // 示例4：流水灯效果
        WS2812_Clear();
        for (int i = 0; i < LED_COUNT; i++) {
            WS2812_SetPixelColor(i, color);
            WS2812_Show();
            HAL_Delay(50); // 每个灯间�?50ms
            if(Key_GetPress()){
              interupt = 0;
              break;
            }
        }
        if(interupt) HAL_Delay(1000);
        else HAL_Delay(80);
        
			}
//---跑马灯---
void runningLight(RGB_Color_TypeDef color) {
    WS2812_Clear(); // 初始清屏

    for (int i = 0; i < LED_COUNT; i++) {
        // 1. 检测按键，如果有按下则退出当前特效
        if (Key_GetPress()) {
            interupt = 0;
            return; // 直接返回，让主循环进入下一个模式
        }

        // 2. 清屏，确保只有一个灯亮
        WS2812_Clear();

        // 3. 设置当前索引的灯珠颜色
        WS2812_SetPixelColor(i, color);

        // 4. 刷新显示
        WS2812_Show();

        // 5. 延时控制速度
        HAL_Delay(50);
    }
}
//---双向汇聚与发散---
void convergeAndDiverge(RGB_Color_TypeDef color) {
    WS2812_Clear();

    // 第一阶段：从两端向中间汇聚 (0 -> 30)
    for (int i = 0; i <= LED_COUNT / 2; i++) {
        if (Key_GetPress()) { interupt = 0; return; }

        WS2812_Clear();
        // 左侧灯珠
        WS2812_SetPixelColor(i, color);
        // 右侧灯珠 (总数 - 1 - 当前索引)
        WS2812_SetPixelColor(LED_COUNT - 1 - i, color);

        WS2812_Show();
        HAL_Delay(60);
    }

    // 第二阶段：从中间向两端发散 (30 -> 0)
    for (int i = LED_COUNT / 2; i >= 0; i--) {
        if (Key_GetPress()) { interupt = 0; return; }

        WS2812_Clear();
        WS2812_SetPixelColor(i, color);
        WS2812_SetPixelColor(LED_COUNT - 1 - i, color);

        WS2812_Show();
        HAL_Delay(60);
    }
}
//---渐进式填充---
void fillProgressive(RGB_Color_TypeDef color) {
    // 外层循环控制填充的过程
    for (int j = 0; j < LED_COUNT; j++) {
        if (Key_GetPress()) {interupt = 0; return; }

        // 设置第 j 个灯珠
        WS2812_SetPixelColor(j, color);

        // 刷新显示（不需要每次清屏，所以灯会积累）
        WS2812_Show();
        HAL_Delay(40);
    }

    // 填满后的暂停
    HAL_Delay(500);

    // 快速闪烁一下或者渐灭（这里采用直接熄灭）
    if (Key_GetPress()) {interupt = 0; return; }
    WS2812_Clear();
    WS2812_Show();
    HAL_Delay(200);
}
//---流星拖尾---
void meteorTrail(RGB_Color_TypeDef color) {
    // 定义尾巴长度
    int trailLen = 5;

    for (int i = 0; i < LED_COUNT; i++) {
        if (Key_GetPress()) { interupt = 0; return; }

        WS2812_Clear();

        // 绘制流星头
        WS2812_SetPixelColor(i, color);

        // 绘制尾巴 (亮度递减)
        for (int j = 1; j <= trailLen; j++) {
            if (i - j >= 0) {
                // 这里简单模拟变暗，实际颜色结构体可能需要支持亮度调整
                // 如果结构体不支持，可以全部设为同一个颜色，或者简单的变暗逻辑
                WS2812_SetPixelColor(i - j, color);
            }
        }

        WS2812_Show();
        HAL_Delay(80);
    }
}
void color_preview(RGB_Color_TypeDef color){ 
  WS2812_Clear();
  WS2812_SetPixelColor(2, color);
  WS2812_Show();
}