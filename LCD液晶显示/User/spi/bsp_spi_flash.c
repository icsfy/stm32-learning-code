#include "spi/bsp_spi_flash.h"

static uint8_t FLASH_UserCallback(uint8_t errorcode)
{
  FLASH_ERROR("FLASH timeout! Errorcode = %d", errorcode);
  return errorcode;
}

static void SPI_GPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
  RCC_APB2PeriphClockCmd(SPI_FLASH_NSS_RCC_CLK |
                         SPI_FLASH_SCK_RCC_CLK |
                         SPI_FLASH_MOSI_RCC_CLK |
                         SPI_FLASH_MISO_RCC_CLK, ENABLE);
  
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStruct.GPIO_Pin = SPI_FLASH_NSS_GPIO_PIN;
  GPIO_Init(SPI_FLASH_NSS_GPIO_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStruct.GPIO_Pin = SPI_FLASH_SCK_GPIO_PIN;
  GPIO_Init(SPI_FLASH_SCK_GPIO_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStruct.GPIO_Pin = SPI_FLASH_MOSI_GPIO_PIN;
  GPIO_Init(SPI_FLASH_MOSI_GPIO_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStruct.GPIO_Pin = SPI_FLASH_MISO_GPIO_PIN;
  GPIO_Init(SPI_FLASH_MISO_GPIO_PORT, &GPIO_InitStruct);
  
  SPI_FLASH_NSS_HIGH;
}

static void SPI_Flash_Config(void)
{
  SPI_InitTypeDef SPI_InitStruct;
  
  RCC_SPIx_CLK_CMD(RCC_SPIx_CLK, ENABLE);
  
  SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
  SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;
  SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
  SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStruct.SPI_CRCPolynomial = 0;
  
  SPI_Init(FLASH_SPIx, &SPI_InitStruct);
  SPI_Cmd(FLASH_SPIx, ENABLE);
}

void SPI_Flash_Init(void)
{
  SPI_GPIO_Config();
  SPI_Flash_Config();
}

static uint8_t SPI_Flash_SendByte(uint8_t data)
{
  uint32_t timeout = FLASH_TIMEOUT;
  while(SPI_I2S_GetFlagStatus(FLASH_SPIx, SPI_I2S_FLAG_TXE) != SET){
    if (--timeout == 0) return FLASH_UserCallback(11);
  }
  SPI_I2S_SendData(FLASH_SPIx, data);
  timeout = FLASH_TIMEOUT;
  while(SPI_I2S_GetFlagStatus(FLASH_SPIx, SPI_I2S_FLAG_RXNE) != SET){
    if (--timeout == 0) return FLASH_UserCallback(12);
  }
  return SPI_I2S_ReceiveData(FLASH_SPIx);
}

uint32_t Flash_JEDEC_ID(void)
{
  uint32_t id = 0;
  SPI_FLASH_NSS_LOW;
  
  SPI_Flash_SendByte(W25X_JEDEC_ID);
  
  id = SPI_Flash_SendByte(DUMMY_CODE);
  id <<= 8;
  id |= SPI_Flash_SendByte(DUMMY_CODE);
  id <<= 8;
  id |= SPI_Flash_SendByte(DUMMY_CODE);
  
  SPI_FLASH_NSS_HIGH;
  return id;
}

static uint8_t Flash_WaitForStandBy(void)
{
  uint32_t timeout = FLASH_LONG_TIMEOUT;
  SPI_FLASH_NSS_LOW;
  SPI_Flash_SendByte(W25X_READ_SR1);
  while(SPI_Flash_SendByte(DUMMY_CODE) & FLASH_FLAG_BUSY){
    if (--timeout == 0) return FLASH_UserCallback(127);
  }
  SPI_FLASH_NSS_HIGH;
  return 0;
}

static void Flash_Write_Enable(void)
{
  SPI_FLASH_NSS_LOW;
  SPI_Flash_SendByte(W25X_WRITE_ENABLE);
  SPI_FLASH_NSS_HIGH;
}

uint8_t Flash_Sector_Erase(uint32_t addr)
{
  /* ÉÈÇø²Á³ýÒª 4K ¶ÔÆë */
  if ((addr % 0x1000) != 0) {
    return FLASH_UserCallback(20);
  }
  Flash_WaitForStandBy();
  Flash_Write_Enable();
  SPI_FLASH_NSS_LOW;
  SPI_Flash_SendByte(W25X_SECTOR_ERASE);
  SPI_Flash_SendByte((addr >> 16) & 0xff);
  SPI_Flash_SendByte((addr >> 8) & 0xff);
  SPI_Flash_SendByte(addr & 0xff);
  SPI_FLASH_NSS_HIGH;
  return 0;
}

void Flash_Read_Data(uint8_t *buff, uint32_t addr, uint32_t size)
{
  Flash_WaitForStandBy();
  SPI_FLASH_NSS_LOW;
  SPI_Flash_SendByte(W25X_READ_Data);
  SPI_Flash_SendByte((addr >> 16) & 0xff);
  SPI_Flash_SendByte((addr >> 8) & 0xff);
  SPI_Flash_SendByte(addr & 0xff);
  while (size--) {
    *buff = SPI_Flash_SendByte(DUMMY_CODE);
    buff++;
  }
  SPI_FLASH_NSS_HIGH;
}

static uint8_t Flash_Page_Program(uint8_t *buff, uint32_t addr, uint32_t size)
{
  if (size < 1 || size > FLASH_PAGE_SIZE) {
    return FLASH_UserCallback(30);
  }
  Flash_WaitForStandBy();
  Flash_Write_Enable();
  SPI_FLASH_NSS_LOW;
  SPI_Flash_SendByte(W25X_PAGE_PROGRAM);
  SPI_Flash_SendByte((addr >> 16) & 0xff);
  SPI_Flash_SendByte((addr >> 8) & 0xff);
  SPI_Flash_SendByte(addr & 0xff);
  while (size--) {
    SPI_Flash_SendByte(*buff);
    buff++;
  }
  SPI_FLASH_NSS_HIGH;
  return 0;
}

void Flash_Write_Data(uint8_t *buff, uint32_t addr, uint32_t size)
{
  while (size > FLASH_PAGE_SIZE) {
    Flash_Page_Program(buff, addr, FLASH_PAGE_SIZE);
    buff += FLASH_PAGE_SIZE;
    addr += FLASH_PAGE_SIZE;
    size -= FLASH_PAGE_SIZE;
  }
  if (size) {
    Flash_Page_Program(buff, addr, size);
  }
}
