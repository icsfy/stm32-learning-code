#include "stm32f10x.h"
#include "led/bsp_led.h"
#include "systick/bsp_systick.h"

int main(void)
{
  LED_GPIO_Init();
  SysTick_Init();
  
  LED_G(OFF);
  LED_B(OFF);
  LED_R(OFF);
  
  while(1) {
    LED_G_TOGGLE;
    SysTick_Delay_ms(500);
    LED_B_TOGGLE;
    SysTick_Delay_ms(500);
    LED_R_TOGGLE;
    SysTick_Delay_ms(500);
  }
}
