#include "i2c/bsp_i2c_eeprom.h"

static uint8_t I2C_TIMEOUT_UserCallback(uint8_t errorcode)
{
  EEPROM_ERROR("I2C wait timout! Errorcode = %d", errorcode);
  return errorcode;
}

static void I2C_GPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
  RCC_I2Cx_SCL_CLK_CMD(RCC_I2Cx_SCL_PERIPH, ENABLE);
  RCC_I2Cx_SDA_CLK_CMD(RCC_I2Cx_SDA_PERIPH, ENABLE);
  
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  
  GPIO_InitStruct.GPIO_Pin = I2Cx_SCL_GPIO_PIN;
  GPIO_Init(I2Cx_SCL_GPIO_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.GPIO_Pin = I2Cx_SDA_GPIO_PIN;
  GPIO_Init(I2Cx_SDA_GPIO_PORT, &GPIO_InitStruct);
}

static void I2C_Mode_Config(void)
{
  I2C_InitTypeDef I2C_InitStruct;
  
  RCC_I2Cx_CLK_CMD(RCC_I2Cx_PERIPH, ENABLE);
  
  I2C_InitStruct.I2C_ClockSpeed = I2Cx_CLK_SPEED;
  I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
  I2C_InitStruct.I2C_OwnAddress1 = I2Cx_OWNADDR;
  I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  
  I2C_Init(I2C_x, &I2C_InitStruct);
  I2C_Cmd(I2C_x, ENABLE);
}

void EEPROM_Init(void)
{
  I2C_GPIO_Config();
  I2C_Mode_Config();
}

/* 等待 EEPROM 内部时序完成 */
uint8_t EEPROM_Check(void)
{
  uint32_t timeout = EEPROM_TIMEOUT;
  do {
    I2C_GenerateSTART(I2C_x, ENABLE);
    while(I2C_GetFlagStatus(I2C_x, I2C_FLAG_SB) != SET){
      if (--timeout == 0) return I2C_TIMEOUT_UserCallback(1);
    }
    I2C_Send7bitAddress(I2C_x, EEPROM_ADDRESS, I2C_Direction_Transmitter);
  } while (I2C_GetFlagStatus(I2C_x, I2C_FLAG_ADDR) != SET);
  I2C_GenerateSTOP(I2C_x, ENABLE);
  return 0;
}

/* 向 EEPROM 写入一个字节 */
uint8_t EEPROM_ByteWrite(uint8_t addr, uint8_t data)
{
  uint32_t timeout = EEPROM_TIMEOUT;
  I2C_GenerateSTART(I2C_x, ENABLE);
  while(I2C_CheckEvent(I2C_x, I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS){
    if (--timeout == 0) return I2C_TIMEOUT_UserCallback(11);
  }
  timeout = EEPROM_TIMEOUT;
  I2C_Send7bitAddress(I2C_x, EEPROM_ADDRESS, I2C_Direction_Transmitter);
  while(I2C_CheckEvent(I2C_x, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) != SUCCESS){
    if (--timeout == 0) return I2C_TIMEOUT_UserCallback(12);
  }
  timeout = EEPROM_TIMEOUT;
  I2C_SendData(I2C_x, addr);
  while(I2C_CheckEvent(I2C_x, I2C_EVENT_MASTER_BYTE_TRANSMITTING) != SUCCESS){
    if (--timeout == 0) return I2C_TIMEOUT_UserCallback(13);
  }
  timeout = EEPROM_TIMEOUT;
  I2C_SendData(I2C_x, data);
  while(I2C_CheckEvent(I2C_x, I2C_EVENT_MASTER_BYTE_TRANSMITTED) != SUCCESS){
    if (--timeout == 0) return I2C_TIMEOUT_UserCallback(14);
  }
  I2C_GenerateSTOP(I2C_x, ENABLE);
  return 0;
}

/* 向 EEPROM 写入 size 字节数据 */
uint8_t EEPROM_Write(uint8_t addr, uint8_t *buff, uint8_t size)
{
  uint32_t timeout;
  while (size) {
    if (EEPROM_Check() != 0) return I2C_TIMEOUT_UserCallback(41);
    I2C_GenerateSTART(I2C_x, ENABLE);
    timeout = EEPROM_TIMEOUT;
    while(I2C_CheckEvent(I2C_x, I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS){
      if (--timeout == 0) return I2C_TIMEOUT_UserCallback(42);
    }
    I2C_Send7bitAddress(I2C_x, EEPROM_ADDRESS, I2C_Direction_Transmitter);
    timeout = EEPROM_TIMEOUT;
    while(I2C_CheckEvent(I2C_x, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) != SUCCESS){
      if (--timeout == 0) return I2C_TIMEOUT_UserCallback(43);
    }
    I2C_SendData(I2C_x, addr);
    timeout = EEPROM_TIMEOUT;
    while(I2C_CheckEvent(I2C_x, I2C_EVENT_MASTER_BYTE_TRANSMITTING) != SUCCESS){
      if (--timeout == 0) return I2C_TIMEOUT_UserCallback(44);
    }
    if ((addr % EEPROM_PAGESIZE) == 0) {
      for (uint8_t i = 0; i < EEPROM_PAGESIZE && size; i++, size--) {
        I2C_SendData(I2C_x, *buff++);
        timeout = EEPROM_TIMEOUT;
        while(I2C_CheckEvent(I2C_x, I2C_EVENT_MASTER_BYTE_TRANSMITTED) != SUCCESS){
          if (--timeout == 0) return I2C_TIMEOUT_UserCallback(45);
        }
      }
      addr += EEPROM_PAGESIZE;
    } else {
      I2C_SendData(I2C_x, *buff++);
      timeout = EEPROM_TIMEOUT;
      while(I2C_CheckEvent(I2C_x, I2C_EVENT_MASTER_BYTE_TRANSMITTED) != SUCCESS){
        if (--timeout == 0) return I2C_TIMEOUT_UserCallback(46);
      }
      size--;
      addr++;
    }
    I2C_GenerateSTOP(I2C_x, ENABLE);
  }
  return 0;
}

/* 从 EEPROM 读取 size 字节数据存入 buff */
uint8_t EEPROM_Read(uint8_t addr, uint8_t *buff, uint8_t size)
{
  uint32_t timeout = EEPROM_TIMEOUT;
  I2C_GenerateSTART(I2C_x, ENABLE);
  while(I2C_CheckEvent(I2C_x, I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS){
    if (--timeout == 0) return I2C_TIMEOUT_UserCallback(21);
  }
  timeout = EEPROM_TIMEOUT;
  I2C_Send7bitAddress(I2C_x, EEPROM_ADDRESS, I2C_Direction_Transmitter);
  while(I2C_CheckEvent(I2C_x, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) != SUCCESS){
    if (--timeout == 0) return I2C_TIMEOUT_UserCallback(22);
  }
  timeout = EEPROM_TIMEOUT;
  I2C_SendData(I2C_x, addr);
  while(I2C_CheckEvent(I2C_x, I2C_EVENT_MASTER_BYTE_TRANSMITTING) != SUCCESS){
    if (--timeout == 0) return I2C_TIMEOUT_UserCallback(23);
  }
  timeout = EEPROM_TIMEOUT;
  I2C_GenerateSTART(I2C_x, ENABLE);
  while(I2C_CheckEvent(I2C_x, I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS){
    if (--timeout == 0) return I2C_TIMEOUT_UserCallback(24);
  }
  timeout = EEPROM_TIMEOUT;
  I2C_Send7bitAddress(I2C_x, EEPROM_ADDRESS, I2C_Direction_Receiver);
  while(I2C_CheckEvent(I2C_x, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED) != SUCCESS){
    if (--timeout == 0) return I2C_TIMEOUT_UserCallback(25);
  }
  timeout = EEPROM_TIMEOUT;
  while (size) {
    if (size == 1) {
      I2C_AcknowledgeConfig(I2C_x, DISABLE);
    }
    while(I2C_CheckEvent(I2C_x, I2C_EVENT_MASTER_BYTE_RECEIVED) != SUCCESS){
      if (--timeout == 0) return I2C_TIMEOUT_UserCallback(26);
    }
    *buff = I2C_ReceiveData(I2C_x);
    buff++;
    size--;
  }
  I2C_GenerateSTOP(I2C_x, ENABLE);
  I2C_AcknowledgeConfig(I2C_x, ENABLE);
  return 0;
}
