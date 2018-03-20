#include "usart/bsp_usart.h"

#if DEBUG_USARTx_RX_IR_ENABLE
static void USARTx_NVIC_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStruct;
  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  
  NVIC_InitStruct.NVIC_IRQChannel = DEBUG_USARTx_IRQn;
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
  
  NVIC_Init(&NVIC_InitStruct);
}
#endif

static void USARTx_GPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  DEBUG_USARTx_GPIO_APBxClkCmd(DEBUG_USARTx_GPIO_CLK, ENABLE);
  // 将 USART TX 的 GPIO 配置为推挽复用模式
  GPIO_InitStruct.GPIO_Pin = DEBUG_USARTx_TX_GPIO_PIN;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(DEBUG_USARTx_TX_GPIO_PORT, &GPIO_InitStruct);
  // 将 USART RX 的 GPIO 配置为浮空输入模式
  GPIO_InitStruct.GPIO_Pin = DEBUG_USARTx_RX_GPIO_PIN;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(DEBUG_USARTx_RX_GPIO_PORT, &GPIO_InitStruct);
}

static void USARTx_Config(void)
{
  USART_InitTypeDef USARTx_InitStruct;
  DEBUG_USARTx_APBxClkCmd(DEBUG_USARTx_CLK, ENABLE);
  // 配置串口的工作参数
  USARTx_InitStruct.USART_BaudRate = DEBUG_USARTx_BaudRate;
  USARTx_InitStruct.USART_WordLength = DEBUG_USARTx_WordLength;
  USARTx_InitStruct.USART_StopBits = DEBUG_USARTx_StopBits;
  USARTx_InitStruct.USART_Parity = DEBUG_USARTx_Parity;
  USARTx_InitStruct.USART_Mode = DEBUG_USARTx_Mode;
  USARTx_InitStruct.USART_HardwareFlowControl = DEBUG_USARTx_HardwareFlowControl;
  // 完成串口的初始化配置
  USART_Init(DEBUG_USARTx, &USARTx_InitStruct);
  // 使能串口
  USART_Cmd(DEBUG_USARTx, ENABLE);
#if DEBUG_USARTx_RX_IR_ENABLE
  USARTx_NVIC_Config();
  // 使能接收中断
  USART_ITConfig(DEBUG_USARTx, USART_IT_RXNE, ENABLE);
#endif
}

void USARTx_Init(void)
{
  USARTx_GPIO_Config();
  USARTx_Config();
}

/* 发送一个字节 */
//void USART_SendByte(USART_TypeDef* USARTx, uint8_t Data)
//{
//  USART_SendData(USARTx, Data);
//  while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
//}
void USART_SendByte(uint8_t Data)
{
  USART_SendData(DEBUG_USARTx, Data);
  while(USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_TXE) == RESET);
}

/* 发送字符串 */
//void USART_SendStr(USART_TypeDef* USARTx, uint8_t *str)
//{
//  while (*str != '\0') {
//    USART_SendByte(USARTx, *str++);
//  }
//  while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
//}
void USART_SendStr(char *str)
{
  while (*str != '\0') {
    USART_SendByte(*str++);
  }
  while(USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_TC) == RESET);
}

/* 重定向c库函数printf到串口，重定向后可使用printf函数 */
int fputc(int ch, FILE *f)
{
  USART_SendData(DEBUG_USARTx, (uint8_t)ch);
  while(USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_TXE) == RESET);
  return ch;
}

int fgetc(FILE *f)
{
  while(USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_RXNE) == RESET);
  return (int)USART_ReceiveData(DEBUG_USARTx);
}
