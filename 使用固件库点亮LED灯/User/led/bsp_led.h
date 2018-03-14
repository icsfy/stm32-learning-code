#ifndef __BSP_LED_H
#define __BSP_LED_H

#include <stm32f10x.h>

#define LED_G_GPIO_PORT       GPIOB
#define LED_G_GPIO_PIN        GPIO_Pin_0
#define LED_G_GPIO_CLK        RCC_APB2Periph_GPIOB

#define LED_B_GPIO_PORT       GPIOB
#define LED_B_GPIO_PIN        GPIO_Pin_1
#define LED_B_GPIO_CLK        RCC_APB2Periph_GPIOB

#define LED_R_GPIO_PORT       GPIOB
#define LED_R_GPIO_PIN        GPIO_Pin_5
#define LED_R_GPIO_CLK        RCC_APB2Periph_GPIOB

#define ON 1
#define OFF 0

#define LED_G(s)  if(s) \
                    GPIO_ResetBits(LED_G_GPIO_PORT, LED_G_GPIO_PIN); \
                  else \
                    GPIO_SetBits(LED_G_GPIO_PORT, LED_G_GPIO_PIN);

#define LED_B(s)  if(s) \
                    GPIO_ResetBits(LED_B_GPIO_PORT, LED_B_GPIO_PIN); \
                  else \
                    GPIO_SetBits(LED_B_GPIO_PORT, LED_B_GPIO_PIN);

#define LED_R(s)  if(s) \
                    GPIO_ResetBits(LED_R_GPIO_PORT, LED_R_GPIO_PIN); \
                  else \
                    GPIO_SetBits(LED_R_GPIO_PORT, LED_R_GPIO_PIN);

void LED_GPIO_Init(void);

#endif /* __BSP_LED_H */
