#ifndef __BSP_USART_H
#define __BSP_USART_H

#include "stm32f10x.h"
#include "stdio.h"      // 重定向c库函数

#define DEBUG_USART1    1
#define DEBUG_USART2    0
#define DEBUG_USART3    0
#define DEBUG_USART4    0
#define DEBUG_USART5    0

#define DEBUG_USARTx_RX_IR_ENABLE 0   // 接收中断

#if DEBUG_USART1  /* USART1 */
  #define DEBUG_USARTx                      USART1
  #define DEBUG_USARTx_CLK                  RCC_APB2Periph_USART1
  #define DEBUG_USARTx_APBxClkCmd           RCC_APB2PeriphClockCmd
  #define DEBUG_USARTx_GPIO_CLK             RCC_APB2Periph_GPIOA
  #define DEBUG_USARTx_GPIO_APBxClkCmd      RCC_APB2PeriphClockCmd
  #define DEBUG_USARTx_TX_GPIO_PORT         GPIOA
  #define DEBUG_USARTx_TX_GPIO_PIN          GPIO_Pin_9
  #define DEBUG_USARTx_RX_GPIO_PORT         GPIOA
  #define DEBUG_USARTx_RX_GPIO_PIN          GPIO_Pin_10
  #define DEBUG_USARTx_BaudRate             115200
  #define DEBUG_USARTx_WordLength           USART_WordLength_8b
  #define DEBUG_USARTx_StopBits             USART_StopBits_1
  #define DEBUG_USARTx_Parity               USART_Parity_No
  #define DEBUG_USARTx_Mode                 (USART_Mode_Rx | USART_Mode_Tx)
  #define DEBUG_USARTx_HardwareFlowControl  USART_HardwareFlowControl_None
  #define DEBUG_USARTx_IRQn                 USART1_IRQn
  #define DEBUG_USARTx_IRQHandler           USART1_IRQHandler
#elif DEBUG_USART2  /* USART2 */
  #define DEBUG_USARTx                      USART2
  #define DEBUG_USARTx_CLK                  RCC_APB1Periph_USART2
  #define DEBUG_USARTx_APBxClkCmd           RCC_APB1PeriphClockCmd
  #define DEBUG_USARTx_GPIO_CLK             RCC_APB2Periph_GPIOA
  #define DEBUG_USARTx_GPIO_APBxClkCmd      RCC_APB2PeriphClockCmd
  #define DEBUG_USARTx_TX_GPIO_PORT         GPIOA
  #define DEBUG_USARTx_TX_GPIO_PIN          GPIO_Pin_2
  #define DEBUG_USARTx_RX_GPIO_PORT         GPIOA
  #define DEBUG_USARTx_RX_GPIO_PIN          GPIO_Pin_3
  #define DEBUG_USARTx_BaudRate             115200
  #define DEBUG_USARTx_WordLength           USART_WordLength_8b
  #define DEBUG_USARTx_StopBits             USART_StopBits_1
  #define DEBUG_USARTx_Parity               USART_Parity_No
  #define DEBUG_USARTx_Mode                 (USART_Mode_Rx | USART_Mode_Tx)
  #define DEBUG_USARTx_HardwareFlowControl  USART_HardwareFlowControl_None
  #define DEBUG_USARTx_IRQn                 USART2_IRQn
  #define DEBUG_USARTx_IRQHandler           USART2_IRQHandler
#elif DEBUG_USART3  /* USART3 */
  #define DEBUG_USARTx                      USART3
  #define DEBUG_USARTx_CLK                  RCC_APB1Periph_USART3
  #define DEBUG_USARTx_APBxClkCmd           RCC_APB1PeriphClockCmd
  #define DEBUG_USARTx_GPIO_CLK             RCC_APB2Periph_GPIOB
  #define DEBUG_USARTx_GPIO_APBxClkCmd      RCC_APB2PeriphClockCmd
  #define DEBUG_USARTx_TX_GPIO_PORT         GPIOB
  #define DEBUG_USARTx_TX_GPIO_PIN          GPIO_Pin_10
  #define DEBUG_USARTx_RX_GPIO_PORT         GPIOB
  #define DEBUG_USARTx_RX_GPIO_PIN          GPIO_Pin_11
  #define DEBUG_USARTx_BaudRate             115200
  #define DEBUG_USARTx_WordLength           USART_WordLength_8b
  #define DEBUG_USARTx_StopBits             USART_StopBits_1
  #define DEBUG_USARTx_Parity               USART_Parity_No
  #define DEBUG_USARTx_Mode                 (USART_Mode_Rx | USART_Mode_Tx)
  #define DEBUG_USARTx_HardwareFlowControl  USART_HardwareFlowControl_None
  #define DEBUG_USARTx_IRQn                 USART3_IRQn
  #define DEBUG_USARTx_IRQHandler           USART3_IRQHandler
#elif DEBUG_USART4  /* UART4 */
  #define DEBUG_USARTx                      UART4
  #define DEBUG_USARTx_CLK                  RCC_APB1Periph_UART4
  #define DEBUG_USARTx_APBxClkCmd           RCC_APB1PeriphClockCmd
  #define DEBUG_USARTx_GPIO_CLK             RCC_APB2Periph_GPIOC
  #define DEBUG_USARTx_GPIO_APBxClkCmd      RCC_APB2PeriphClockCmd
  #define DEBUG_USARTx_TX_GPIO_PORT         GPIOC
  #define DEBUG_USARTx_TX_GPIO_PIN          GPIO_Pin_10
  #define DEBUG_USARTx_RX_GPIO_PORT         GPIOC
  #define DEBUG_USARTx_RX_GPIO_PIN          GPIO_Pin_11
  #define DEBUG_USARTx_BaudRate             115200
  #define DEBUG_USARTx_WordLength           USART_WordLength_8b
  #define DEBUG_USARTx_StopBits             USART_StopBits_1
  #define DEBUG_USARTx_Parity               USART_Parity_No
  #define DEBUG_USARTx_Mode                 (USART_Mode_Rx | USART_Mode_Tx)
  #define DEBUG_USARTx_HardwareFlowControl  USART_HardwareFlowControl_None
  #define DEBUG_USARTx_IRQn                 UART4_IRQn
  #define DEBUG_USARTx_IRQHandler           UART4_IRQHandler
#elif DEBUG_USART5  /* UART5 */
  #define DEBUG_USARTx                      UART5
  #define DEBUG_USARTx_CLK                  RCC_APB1Periph_UART5
  #define DEBUG_USARTx_APBxClkCmd           RCC_APB1PeriphClockCmd
  #define DEBUG_USARTx_GPIO_CLK             (RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD)
  #define DEBUG_USARTx_GPIO_APBxClkCmd      RCC_APB2PeriphClockCmd
  #define DEBUG_USARTx_TX_GPIO_PORT         GPIOC
  #define DEBUG_USARTx_TX_GPIO_PIN          GPIO_Pin_12
  #define DEBUG_USARTx_RX_GPIO_PORT         GPIOD
  #define DEBUG_USARTx_RX_GPIO_PIN          GPIO_Pin_2
  #define DEBUG_USARTx_BaudRate             115200
  #define DEBUG_USARTx_WordLength           USART_WordLength_8b
  #define DEBUG_USARTx_StopBits             USART_StopBits_1
  #define DEBUG_USARTx_Parity               USART_Parity_No
  #define DEBUG_USARTx_Mode                 (USART_Mode_Rx | USART_Mode_Tx)
  #define DEBUG_USARTx_HardwareFlowControl  USART_HardwareFlowControl_None
  #define DEBUG_USARTx_IRQn                 UART5_IRQn
  #define DEBUG_USARTx_IRQHandler           UART5_IRQHandler
#endif

void USARTx_Init(void);
//void USART_SendByte(USART_TypeDef* USARTx, uint8_t Data);
void USART_SendByte(uint8_t Data);
//void USART_SendStr(USART_TypeDef* USARTx, uint8_t *str);
void USART_SendStr(char *str);

/* USART 接收中断处理函数 回显字符 */
__STATIC_INLINE void USARTx_RX_IRQHandler(void)
{
  uint8_t tmpreg;
  if (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_RXNE) == SET) {
    tmpreg = USART_ReceiveData(DEBUG_USARTx);
    USART_SendData(DEBUG_USARTx, tmpreg);
  }
}

#endif /* __BSP_USART_H */
