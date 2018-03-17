#include "stm32f10x.h"
#include "led/bsp_led.h"
#include "usart/bsp_usart.h"

#define RX_BUFF_SIZE  64

char rx_buff[RX_BUFF_SIZE];

void led_parser(char *param);
void led_switch(uint8_t led_flag, uint8_t stat);

int main(void)
{
  LED_GPIO_Init();
  USARTx_Init();
  
  LED_G(OFF);
  LED_B(OFF);
  LED_R(OFF);
  
  printf("\n---USART 测试---\n");
  printf("---LED控制：\n"
         "---\t\"RGB\"三种颜色---\n"
         "---\t\"ON\",\"OFF\"开关---\n"
         "示例1：\"R ON G OFF B ON\"\n"
         "示例2：\"RGB OFF\"\n");
  while(1) {
    scanf("%[^\n]", rx_buff);
    printf("\nReceived: %s\n", rx_buff);
    led_parser(rx_buff);
  }
}

/* 分析接收 buff 参数
 * 先分析颜色，再分析状态
 * 一次只操作一种颜色的灯
 * 有三种颜色：'RGB', 3bit
 * 有开关状态："ON","OFF"
 */
void led_parser(char *param)
{
  uint8_t led_flag = 0;
  while (*param != '\0' && *(param+1) != '\0') {
    switch (*param++) {
      case 'R':
        led_flag |= 0x04;
        break;
      case 'G':
        led_flag |= 0x02;
        break;
      case 'B':
        led_flag |= 0x01;
        break;
      case 'O':
        if (*param == 'N') {          // ON
          led_switch(led_flag, ON);
          led_flag = 0;
          param++;
        } else if (*param == 'F') {   // OFF
          led_switch(led_flag, OFF);
          led_flag = 0;
          param++;
        }
        break;
      default:
        break;
    }
  }
}

void led_switch(uint8_t led_flag, uint8_t stat)
{
  //printf(" flag: %d ", led_flag);
  if ((led_flag & 0x04) == 0x04) {
    LED_R(stat);
    printf(" LED_R: %s ", stat == ON ? "ON" : "OFF");
  }
  if ((led_flag & 0x02) == 0x02) {
    LED_G(stat);
    printf(" LED_G: %s ", stat == ON ? "ON" : "OFF");
  }
  if ((led_flag & 0x01) == 0x01) {
    LED_B(stat);
    printf(" LED_B: %s ", stat == ON ? "ON" : "OFF");
  }
}
