#include "stm32f10x.h"
#include "usart/bsp_usart.h"
#include "i2c/bsp_i2c_eeprom.h"

#define TEST_ADDR        0x05
//#define TEST_DATA        'a'

#define TEST_SIZE        26

uint8_t tx_buff[TEST_SIZE];
uint8_t rx_buff[TEST_SIZE];

int main(void)
{
  uint8_t i;
  USARTx_Init();
  EEPROM_Init();
  
  /* 填充数据 */
  for (uint8_t i = 0; i < TEST_SIZE; i++) {
    tx_buff[i] = i + 'A';
  }
  
  printf("\n I2C EEPROM 测试 \n");
  EEPROM_DEBUG("AT24C02");
  
  /* 写数据 */
  if (EEPROM_Check() != 0) {
    EEPROM_ERROR("EEPROM Check Error");
  } else {
    EEPROM_INFO("Data write to EEPROM:");
    for (i = 0; i < TEST_SIZE; i++) {
      printf("%c ", tx_buff[i]);
    }
    putchar('\n');
    //EEPROM_ByteWrite(TEST_ADDR, TEST_DATA);
    EEPROM_Write(TEST_ADDR, tx_buff, TEST_SIZE);
  }
  
  /* 读数据 */
  if (EEPROM_Check() != 0) {
    EEPROM_ERROR("EEPROM Check Error");
  } else {
    EEPROM_INFO("Data read from EEPROM:");
    EEPROM_Read(TEST_ADDR, rx_buff, TEST_SIZE);
    for (i = 0; i < TEST_SIZE; i++) {
      printf("%c ", rx_buff[i]);
    }
    putchar('\n');
  }
  
  /* 比较读写数据是否相同 */
  for (i = 0; i < TEST_SIZE; i++) {
    if (tx_buff[i] != rx_buff[i]) break;
  }
  if (i == TEST_SIZE) {
    EEPROM_INFO("Wow! It's same.");
  } else {
    EEPROM_INFO("Oops! Something wrong.");
  }
  
  /* 浮点数测试 */
  double pi = 3.1415927;
  double pi_tmp;
  if (EEPROM_Check() != 0) {
    EEPROM_ERROR("EEPROM Check Error");
  } else {
    EEPROM_INFO("Write data: %f", pi);
    EEPROM_Write(TEST_ADDR, (uint8_t *)&pi, sizeof(double));
  }
  
  if (EEPROM_Check() != 0) {
    EEPROM_ERROR("EEPROM Check Error");
  } else {
    EEPROM_Read(TEST_ADDR, (uint8_t *)&pi_tmp, sizeof(double));
    EEPROM_INFO("Read data: %f", pi_tmp);
  }
  
  if (pi == pi_tmp) {
    EEPROM_INFO("Success");
  } else {
    EEPROM_INFO("Error");
  }
  
  while(1) {
  }
}
