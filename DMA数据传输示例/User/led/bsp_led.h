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

#define LED_G_TOGGLE  LED_G_GPIO_PORT->ODR ^= LED_G_GPIO_PIN
#define LED_B_TOGGLE  LED_B_GPIO_PORT->ODR ^= LED_B_GPIO_PIN
#define LED_R_TOGGLE  LED_R_GPIO_PORT->ODR ^= LED_R_GPIO_PIN

//ºì
#define LED_RED     LED_R(ON) \
                    LED_G(OFF)  \
                    LED_B(OFF)

//ÂÌ
#define LED_GREEN   LED_R(OFF)  \
                    LED_G(ON) \
                    LED_B(OFF)

//À¶
#define LED_BLUE    LED_R(OFF)  \
                    LED_G(OFF)  \
                    LED_B(ON)

//»Æ(ºì+ÂÌ)          
#define LED_YELLOW  LED_R(ON) \
                    LED_G(ON) \
                    LED_B(OFF)

//×Ï(ºì+À¶)
#define LED_PURPLE  LED_R(ON) \
                    LED_G(OFF)  \
                    LED_B(ON)

//Çà(ÂÌ+À¶)
#define LED_CYAN    LED_R(OFF)  \
                    LED_G(ON) \
                    LED_B(ON)

//°×(ºì+ÂÌ+À¶)
#define LED_WHITE   LED_R(ON) \
                    LED_G(ON) \
                    LED_B(ON)

//ºÚ(È«²¿¹Ø±Õ)
#define LED_RGBOFF  LED_R(OFF)  \
                    LED_G(OFF)  \
                    LED_B(OFF)

void LED_GPIO_Init(void);

#endif /* __BSP_LED_H */
