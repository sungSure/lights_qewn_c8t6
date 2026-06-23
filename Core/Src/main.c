/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ws2812.h" 
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
RGB_Color_TypeDef red   = {255, 0, 0};
RGB_Color_TypeDef green = {0, 255, 0};
RGB_Color_TypeDef blue  = {0, 0, 255};
RGB_Color_TypeDef white = {180,220,230};
RGB_Color_TypeDef color = {255, 180, 100};//默认暖白光
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define SWITCH_GPIO_PORT GPIOA
#define MODE_SWITCH_KEY GPIO_PIN_8
#define COLOR_SWITCH_KEY GPIO_PIN_9
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t mode = 0;
uint8_t interupt = 1;
uint8_t is_color_switching = 0;
uint8_t color_switch[3]= {255, 180, 100};
uint8_t color_switch_index = 0;
uint32_t tick=0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
//非阻塞按键检测与效果切换
uint8_t mode_switch_Key_GetPress(void){
  if(HAL_GPIO_ReadPin(SWITCH_GPIO_PORT,MODE_SWITCH_KEY)==RESET){
    HAL_Delay(20);
    if(HAL_GPIO_ReadPin(SWITCH_GPIO_PORT,MODE_SWITCH_KEY)==RESET){
      while(HAL_GPIO_ReadPin(SWITCH_GPIO_PORT,MODE_SWITCH_KEY)==RESET);
      mode++;
      return 1;
    }
  }
  return 0;
}
uint8_t color_switch_key_GetPress(){
  if(HAL_GPIO_ReadPin(SWITCH_GPIO_PORT,COLOR_SWITCH_KEY)==RESET){
    HAL_Delay(20);
    if(HAL_GPIO_ReadPin(SWITCH_GPIO_PORT,COLOR_SWITCH_KEY)==RESET){
      while(HAL_GPIO_ReadPin(SWITCH_GPIO_PORT,COLOR_SWITCH_KEY)==RESET);
      is_color_switching = 1;
      tick = HAL_GetTick();
      return 1;
    }
  }
  return 0;
}
uint8_t Key_GetPress(void){
  if(mode_switch_Key_GetPress() ||
  color_switch_key_GetPress()){
    return 1;
  }else return 0;
}
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
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
   
    if(mode_switch_Key_GetPress()){
      mode += 1;
    }
    if(is_color_switching){
      while(HAL_GetTick()-tick<3000){
        if(HAL_GPIO_ReadPin(SWITCH_GPIO_PORT,MODE_SWITCH_KEY)==RESET){
          HAL_Delay(20);
          if(HAL_GPIO_ReadPin(SWITCH_GPIO_PORT,MODE_SWITCH_KEY)==RESET){
            color_switch_index += 1;
            if(color_switch_index>2)color_switch_index=0;
            tick = HAL_GetTick();
          }
        }

        if(HAL_GPIO_ReadPin(SWITCH_GPIO_PORT,COLOR_SWITCH_KEY)==RESET){
          HAL_Delay(20);
          if(HAL_GPIO_ReadPin(SWITCH_GPIO_PORT,COLOR_SWITCH_KEY)==RESET){
            color_switch[color_switch_index]+= 1;
            if(color_switch[color_switch_index]>255)color_switch[color_switch_index]=0;
            tick = HAL_GetTick();
          }
          color.R = color_switch[0];
          color.G = color_switch[1];
          color.B = color_switch[2];
          color_preview(color);
        }
      }
      is_color_switching = 0;
    }else{
      switch (mode)
      {
      case 0:singleColor(color);break;
      case 1:flow(color);break;
      case 2:runningLight(color);break;
      case 3:convergeAndDiverge(color);break;
      case 4:fillProgressive(color);break;
      case 5:meteorTrail(color);break;
      // case 6:flow(color);break;
      // case 7:flow(color);break;
      default:mode = 0;break;
      }
    }
     /* USER CODE END WHILE */
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
