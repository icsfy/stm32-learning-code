#include "exti/bsp_exti.h"

static void KEY_NVIC_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStruct;
  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  
  NVIC_InitStruct.NVIC_IRQChannel = KEY1_NVIC_IRQn;
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStruct);
  
  NVIC_InitStruct.NVIC_IRQChannel = KEY2_NVIC_IRQn;
  NVIC_Init(&NVIC_InitStruct);
}

static void KEY_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
  RCC_APB2PeriphClockCmd(KEY1_GPIO_CLK | KEY2_GPIO_CLK, ENABLE);
  
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  
  GPIO_InitStruct.GPIO_Pin = KEY1_GPIO_PIN;
  GPIO_Init(KEY1_GPIO_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.GPIO_Pin = KEY2_GPIO_PIN;
  GPIO_Init(KEY2_GPIO_PORT, &GPIO_InitStruct);
}

void KEY_EXTI_Init(void)
{
  EXTI_InitTypeDef EXTI_InitStruct;
  KEY_NVIC_Config();
  KEY_GPIO_Init();
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  
  GPIO_EXTILineConfig(KEY1_EXTI_PORT, KEY1_EXTI_PIN);
  EXTI_InitStruct.EXTI_Line = KEY1_EXTI_LINE;
  EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStruct.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStruct);
  
  GPIO_EXTILineConfig(KEY2_EXTI_PORT, KEY2_EXTI_PIN);
  EXTI_InitStruct.EXTI_Line = KEY2_EXTI_LINE;
  EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_Init(&EXTI_InitStruct);
}
