#include "stm32f10x.h"
#include "led/bsp_led.h"
#include "usart/bsp_usart.h"
#include "dma/bsp_dma.h"

void Test1(void);
void Test2(void);
void Delay(uint32_t t) { while(t--); }

int main(void)
{
  USARTx_Init();
  printf("\n---DMA ����---\n");
  
  printf("\n---Test1 M2M---\n"
         "�ɹ����̵ƣ����������\n");
  Delay(0x5fffff);
  Test1();
  
  printf("\n---Test2 M2P---\n"
         "����ͨ��USARTѭ�������ַ���ͬʱLED��˸\n");
  Delay(0x1ffffff);
  Test2();
  
  /* LED ����ͬʱ��˸ */
  while(1) {
    LED_B_TOGGLE;
    Delay(0x1fffff);
  }
}

void Test1(void)
{
  LED_GPIO_Init();
  LED_YELLOW;
  Delay(0xffffff);
  DMA_M2M_Config();
  
  while(DMA_GetFlagStatus(DMA_M2M_FLAG_TC) != SET);
  
  if (BufferCmp(aSRC_Const_Buffer, aDST_Buffer, BUFFER_SIZE) == SUCCESS) {
    LED_GREEN;
  } else {
    LED_RED;
  }
  Delay(0xffffff);
  LED_RGBOFF;
}

void Test2(void)
{
  DMA_M2P_USARTx_Config();
  
  /* ��� A~Z 26����ĸ */
  for (uint32_t i = 0; i < SEND_BUFF_SIZE; i++) {
    if (i % 27 == 0) {
      if (i % (2 * 27) == 0) {
        SendBuffer[i] = '\n';
      } else {
        SendBuffer[i] = ' ';
      }
      continue;
    }
    SendBuffer[i] = 'A' + i % 27 - 1;
  }
  
  USART_DMACmd(DMA_M2P_USARTx, USART_DMAReq_Tx, ENABLE);
}
