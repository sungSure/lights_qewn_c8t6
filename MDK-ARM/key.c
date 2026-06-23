#include "keys.h"

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