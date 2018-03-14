#include <stm32f10x.h>
#include "led/bsp_led.h"

void Delay()
{
  for(int i = 0x2fffff; i; i--);
}

int main(void)
{
  LED_GPIO_Init();
  
  while(1) {
    LED_G(ON);
    Delay();
    LED_G(OFF);
    Delay();
    LED_B(ON);
    Delay();
    LED_B(OFF);
    Delay();
    LED_R(ON);
    Delay();
    LED_R(OFF);
    Delay();
  }
}
