#include <stm32f10x.h>
#include "led/bsp_led.h"
#include "exti/bsp_exti.h"

int main(void)
{
  LED_GPIO_Init();
  KEY_EXTI_Init();
  LED_G(OFF);
  LED_B(OFF);
  LED_R(OFF);
  
  while(1) {
  }
}
