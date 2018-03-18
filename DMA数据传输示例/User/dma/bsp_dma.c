#include "dma/bsp_dma.h"

/* 定义aSRC_Const_Buffer数组作为DMA传输数据源
 * const关键字将aSRC_Const_Buffer数组变量定义为常量类型
 * 表示数据存储在内部的FLASH中
 */
const uint32_t aSRC_Const_Buffer[BUFFER_SIZE]= {
  0x01020304,0x05060708,0x090A0B0C,0x0D0E0F10,
  0x11121314,0x15161718,0x191A1B1C,0x1D1E1F20,
  0x21222324,0x25262728,0x292A2B2C,0x2D2E2F30,
  0x31323334,0x35363738,0x393A3B3C,0x3D3E3F40,
  0x41424344,0x45464748,0x494A4B4C,0x4D4E4F50,
  0x51525354,0x55565758,0x595A5B5C,0x5D5E5F60,
  0x61626364,0x65666768,0x696A6B6C,0x6D6E6F70,
  0x71727374,0x75767778,0x797A7B7C,0x7D7E7F80};

/* 定义DMA传输目标存储器
 * 存储在内部的SRAM中																		
 */
uint32_t aDST_Buffer[BUFFER_SIZE];

uint8_t SendBuffer[SEND_BUFF_SIZE];

void DMA_M2M_Config(void)
{
  DMA_InitTypeDef DMA_InitStruct;
  
  RCC_AHBPeriphClockCmd(DMA_M2M_CLK, ENABLE);
  
  DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)aSRC_Const_Buffer;
  DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)aDST_Buffer;
  DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStruct.DMA_BufferSize = BUFFER_SIZE;
  DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
  DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
  DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
  DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStruct.DMA_Priority = DMA_Priority_High;
  DMA_InitStruct.DMA_M2M = DMA_M2M_Enable;
  
  DMA_Init(DMA_M2M_CHANNEL, &DMA_InitStruct);
  DMA_ClearFlag(DMA_M2M_FLAG_TC);
  DMA_Cmd(DMA_M2M_CHANNEL, ENABLE);
}

void DMA_M2P_USARTx_Config(void)
{
  DMA_InitTypeDef DMA_InitStruct;
  
  RCC_AHBPeriphClockCmd(DMA_M2P_CLK, ENABLE);
  
  DMA_InitStruct.DMA_PeripheralBaseAddr = DMA_M2P_PADDR;
  DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)SendBuffer;
  DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralDST;
  DMA_InitStruct.DMA_BufferSize = SEND_BUFF_SIZE;
  DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStruct.DMA_Priority = DMA_Priority_High;
  DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
  
  DMA_Init(DMA_M2P_CHANNEL, &DMA_InitStruct);
  DMA_ClearFlag(DMA_M2P_FLAG_TC);
  DMA_Cmd(DMA_M2P_CHANNEL, ENABLE);
}

ErrorStatus BufferCmp(const uint32_t *pb1, const uint32_t *pb2, uint16_t size)
{
  for (--size; size > 0; --size) {
    if (pb1[size] != pb2[size]) {
      return ERROR;
    }
  }
  return SUCCESS;
}
