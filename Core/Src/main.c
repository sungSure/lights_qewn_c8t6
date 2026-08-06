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
#include "i2c.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ws2812.h" 
#include "effects.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define SWITCH_GPIO_PORT GPIOA
#define BRI_ADD_PORT GPIOA
#define BRI_MIN_PORT GPIOB
#define MODE_SWITCH_KEY GPIO_PIN_8
#define BRI_ADD_KEY GPIO_PIN_10
#define BRI_MIN_KEY GPIO_PIN_15
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

RGB_Color_TypeDef color = {255, 180, 100};//默认暖白�??
uint8_t color_switch_index = 0;
uint32_t last_click_time = 0;
uint8_t click_flag = 0;
uint8_t mode = 0;
uint8_t is_color_switching = 0;
uint32_t timeout = 0;
uint8_t interupt = 0;
uint8_t frame = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
uint8_t irq_key_check();
void effect_player(uint8_t,RGB_Color_TypeDef);
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
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    if(is_color_switching == 0){
      if(irq_key_check()){
        mode += 1;
        if(mode>5)mode = 0;
        interupt = 1;
      }
    }
    if(is_color_switching){
      while(HAL_GetTick() - timeout<3000){
        if(irq_key_check()){
          color_switch_index += 1;
          if(color_switch_index>2)color_switch_index=0;
          timeout = HAL_GetTick();
        }
        uint8_t operation = 0;
        if(HAL_GPIO_ReadPin(BRI_ADD_PORT,BRI_ADD_KEY)==RESET){
          HAL_Delay(20);
          if(HAL_GPIO_ReadPin(BRI_ADD_PORT,BRI_ADD_KEY)==RESET){
            operation = 1;
          }
        }else if(HAL_GPIO_ReadPin(BRI_MIN_PORT,BRI_MIN_KEY)==RESET){
          HAL_Delay(20);
          if(HAL_GPIO_ReadPin(BRI_MIN_PORT,BRI_MIN_KEY)==RESET){
            operation = -1;
          }
        }
        if(operation){
          color_switch[color_switch_index]+= operation;
          if(color_switch[color_switch_index]>255)color_switch[color_switch_index]=0;
          timeout = HAL_GetTick();
          color.R = color_switch[0];
          color.G = color_switch[1];
          color.B = color_switch[2];
          color_preview(color);
        }
      }
      is_color_switching = 0;
    }else{
      effect_player(mode,color);
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

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
  if(GPIO_Pin == GPIO_PIN_8){
    last_click_time = HAL_GetTick();
    click_flag = 1;
    interupt = 1;
  }
}

uint8_t irq_key_check(){
  if(click_flag == 1){
    if(HAL_GetTick() - last_click_time >= 20){
      if(HAL_GPIO_ReadPin(SWITCH_GPIO_PORT,MODE_SWITCH_KEY)==GPIO_PIN_RESET){
        click_flag = 2;
        return 1;
      }else{
        click_flag = 0;
        return 0;
      }
    }else{
      return 0;
    }
  }else if(click_flag == 2){
    if(HAL_GPIO_ReadPin(SWITCH_GPIO_PORT,MODE_SWITCH_KEY)==GPIO_PIN_RESET){
      if(HAL_GetTick() - last_click_time >= 3000){
        click_flag = 0;
        is_color_switching = 1;
        timeout = HAL_GetTick();
        return 0;
      }
    }else{
      click_flag = 0;
      return 0;
    }
  }
  return 0;
}

void effect_player(uint8_t index,RGB_Color_TypeDef color){
  if(interupt){
    interupt = 0;
    frame = 0;
    return;
  }else{
    effects(index,color);
    // frame ++;
  }
}
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
