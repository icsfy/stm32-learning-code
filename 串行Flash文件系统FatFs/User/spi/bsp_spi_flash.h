#ifndef __BSP_SPI_FLASH_H
#define __BSP_SPI_FLASH_H

#include "stm32f10x.h"
#include "stdio.h"

#define FLASH_DEBUG_ON                1

#define FLASH_SPIx                    SPI1
#define RCC_SPIx_CLK_CMD              RCC_APB2PeriphClockCmd
#define RCC_SPIx_CLK                  RCC_APB2Periph_SPI1

#define SPI_FLASH_NSS_RCC_CLK         RCC_APB2Periph_GPIOC
#define SPI_FLASH_NSS_GPIO_PORT       GPIOC
#define SPI_FLASH_NSS_GPIO_PIN        GPIO_Pin_0

#define SPI_FLASH_SCK_RCC_CLK         RCC_APB2Periph_GPIOA
#define SPI_FLASH_SCK_GPIO_PORT       GPIOA
#define SPI_FLASH_SCK_GPIO_PIN        GPIO_Pin_5

#define SPI_FLASH_MOSI_RCC_CLK        RCC_APB2Periph_GPIOA
#define SPI_FLASH_MOSI_GPIO_PORT      GPIOA
#define SPI_FLASH_MOSI_GPIO_PIN       GPIO_Pin_7

#define SPI_FLASH_MISO_RCC_CLK        RCC_APB2Periph_GPIOA
#define SPI_FLASH_MISO_GPIO_PORT      GPIOA
#define SPI_FLASH_MISO_GPIO_PIN       GPIO_Pin_6

#define SPI_FLASH_NSS_HIGH            GPIO_SetBits(SPI_FLASH_NSS_GPIO_PORT, SPI_FLASH_NSS_GPIO_PIN)
#define SPI_FLASH_NSS_LOW             GPIO_ResetBits(SPI_FLASH_NSS_GPIO_PORT, SPI_FLASH_NSS_GPIO_PIN)

#define FLASH_TIMEOUT             0x1000
#define FLASH_LONG_TIMEOUT        (10 * FLASH_TIMEOUT)

#define FLASH_INFO(fmt,arg...)     printf("<<-FLASH-INFO->> "fmt"\n",##arg)
#define FLASH_ERROR(fmt,arg...)    printf("<<-FLASH-ERROR->> "fmt"\n",##arg)
#define FLASH_DEBUG(fmt,arg...)    do{ \
                                      if(FLASH_DEBUG_ON) \
                                        printf("<<-FLASH-DEBUG->> [%d]"fmt"\n",__LINE__,##arg); \
                                    }while(0)

#define W25X_WRITE_ENABLE       0x06
#define W25X_WRITE_DISABLE      0x04
#define W25X_READ_SR1           0x05
#define W25X_READ_SR2           0x35
#define W25X_WRITE_SR           0x01
#define W25X_PAGE_PROGRAM       0x02
#define W25X_READ_Data          0x03
#define W25X_SECTOR_ERASE       0x20
#define W25X_POWER_DOWN         0xB9
#define W25X_JEDEC_ID           0x9F

#define DUMMY_CODE              0x00
#define FLASH_FLAG_BUSY         0x01
#define FLASH_PAGE_SIZE         256

#define sFLASH_ID               0XEF4017    //W25Q64

void SPI_Flash_Init(void);
uint32_t Flash_JEDEC_ID(void);
uint8_t Flash_Sector_Erase(uint32_t addr);
void Flash_Read_Data(uint8_t *buff, uint32_t addr, uint32_t size);
//uint8_t Flash_Page_Program(uint8_t *buff, uint32_t addr, uint32_t size);
void Flash_Write_Data(uint8_t *buff, uint32_t addr, uint32_t size);

#endif /* __BSP_SPI_FLASH_H */
