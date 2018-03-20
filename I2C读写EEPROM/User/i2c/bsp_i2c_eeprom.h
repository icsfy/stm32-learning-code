#ifndef __BSP_I2C_EEPROM_H
#define __BSP_I2C_EEPROM_H

#include "stm32f10x.h"
#include "stdio.h"

#define EEPROM_DEBUG_ON             1

#define I2C_x                       I2C1
#define I2Cx_CLK_SPEED              400000
#define I2Cx_OWNADDR                0x0A

#define EEPROM_ADDRESS              0xA0
#define EEPROM_PAGESIZE             8
#define EEPROM_TIMEOUT              0x1000

#define RCC_I2Cx_CLK_CMD            RCC_APB1PeriphClockCmd
#define RCC_I2Cx_PERIPH             RCC_APB1Periph_I2C1

/* I2C_SCL */
#define RCC_I2Cx_SCL_CLK_CMD        RCC_APB2PeriphClockCmd
#define RCC_I2Cx_SCL_PERIPH         RCC_APB2Periph_GPIOB
#define I2Cx_SCL_GPIO_PORT          GPIOB
#define I2Cx_SCL_GPIO_PIN           GPIO_Pin_6

/* I2C_SDA */
#define RCC_I2Cx_SDA_CLK_CMD        RCC_APB2PeriphClockCmd
#define RCC_I2Cx_SDA_PERIPH         RCC_APB2Periph_GPIOB
#define I2Cx_SDA_GPIO_PORT          GPIOB
#define I2Cx_SDA_GPIO_PIN           GPIO_Pin_7

#define EEPROM_INFO(fmt,arg...)     printf("<<-EEPROM-INFO->> "fmt"\n",##arg)
#define EEPROM_ERROR(fmt,arg...)    printf("<<-EEPROM-ERROR->> "fmt"\n",##arg)
#define EEPROM_DEBUG(fmt,arg...)    do{ \
                                      if(EEPROM_DEBUG_ON) \
                                        printf("<<-EEPROM-DEBUG->> [%d]"fmt"\n",__LINE__,##arg); \
                                    }while(0)

void EEPROM_Init(void);
uint8_t EEPROM_Check(void);
uint8_t EEPROM_ByteWrite(uint8_t addr, uint8_t data);
uint8_t EEPROM_Write(uint8_t addr, uint8_t *buff, uint8_t size);
uint8_t EEPROM_Read(uint8_t addr, uint8_t *buff, uint8_t size);

#endif /* __BSP_I2C_EEPROM_H */
