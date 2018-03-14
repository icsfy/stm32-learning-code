#include <stm32f10x.h>
#include "led/bsp_led.h"
#include "key/bsp_key.h"

int main(void)
{
  LED_GPIO_Init();
  KEY_GPIO_Init();
  
  LED_G(OFF);
  LED_B(OFF);
  LED_R(OFF);
  
  while(1) {
    if (Key_Scan(KEY1_GPIO_PORT, KEY1_GPIO_PIN) == KEY_ON) {
      LED_G_TOGGLE;
    }
    if (Key_Scan(KEY2_GPIO_PORT, KEY2_GPIO_PIN) == KEY_ON) {
      LED_R_TOGGLE;
    }
  }
}
