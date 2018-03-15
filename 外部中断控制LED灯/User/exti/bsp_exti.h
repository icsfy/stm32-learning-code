#ifndef __BSP_EXTI_H
#define __BSP_EXTI_H

#include <stm32f10x.h>

#define KEY1_GPIO_CLK         RCC_APB2Periph_GPIOA
#define KEY1_GPIO_PORT        GPIOA
#define KEY1_GPIO_PIN         GPIO_Pin_0

#define KEY1_EXTI_PORT        GPIO_PortSourceGPIOA
#define KEY1_EXTI_PIN         GPIO_PinSource0
#define KEY1_EXTI_LINE        EXTI_Line0
#define KEY1_NVIC_IRQn        EXTI0_IRQn
#define KEY1_IRQHandler       EXTI0_IRQHandler

#define KEY2_GPIO_CLK         RCC_APB2Periph_GPIOC
#define KEY2_GPIO_PORT        GPIOC
#define KEY2_GPIO_PIN         GPIO_Pin_13

#define KEY2_EXTI_PORT        GPIO_PortSourceGPIOC
#define KEY2_EXTI_PIN         GPIO_PinSource13
#define KEY2_EXTI_LINE        EXTI_Line13
#define KEY2_NVIC_IRQn        EXTI15_10_IRQn
#define KEY2_IRQHandler       EXTI15_10_IRQHandler

void KEY_EXTI_Init(void);

#endif /* __BSP_EXTI_H */
