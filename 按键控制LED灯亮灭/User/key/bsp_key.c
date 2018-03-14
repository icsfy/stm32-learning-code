#include "bsp_key.h"

void KEY_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
  RCC_APB2PeriphClockCmd(KEY1_GPIO_CLK | KEY2_GPIO_CLK, ENABLE);
  
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  
  GPIO_InitStruct.GPIO_Pin = KEY1_GPIO_PIN;
  GPIO_Init(KEY1_GPIO_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.GPIO_Pin = KEY2_GPIO_PIN;
  GPIO_Init(KEY2_GPIO_PORT, &GPIO_InitStruct);
}

uint8_t Key_Scan(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
  if (GPIO_ReadInputDataBit(GPIOx, GPIO_Pin) == KEY_ON) {
    while(GPIO_ReadInputDataBit(GPIOx, GPIO_Pin) != KEY_OFF);
    return KEY_ON;
  } else {
    return KEY_OFF;
  }
}
