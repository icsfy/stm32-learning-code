#ifndef __BSP_DMA_H
#define __BSP_DMA_H

#include "stm32f10x.h"

#define DMA_M2M_CLK           RCC_AHBPeriph_DMA1
#define DMA_M2M_CHANNEL       DMA1_Channel6
#define DMA_M2M_FLAG_TC       DMA1_FLAG_TC6

#define DMA_M2P_USARTx        USART1
#define DMA_M2P_PADDR         (USART1_BASE + 0x04)
#define DMA_M2P_CLK           RCC_AHBPeriph_DMA1
#define DMA_M2P_CHANNEL       DMA1_Channel4
#define DMA_M2P_FLAG_TC       DMA1_FLAG_TC4

#define BUFFER_SIZE           32
#define SEND_BUFF_SIZE        50000

extern const uint32_t aSRC_Const_Buffer[BUFFER_SIZE];
extern uint32_t aDST_Buffer[BUFFER_SIZE];
extern uint8_t SendBuffer[SEND_BUFF_SIZE];

void DMA_M2M_Config(void);
void DMA_M2P_USARTx_Config(void);
ErrorStatus BufferCmp(const uint32_t *pb1, const uint32_t *pb2, uint16_t size);

#endif /* __BSP_DMA_H */
