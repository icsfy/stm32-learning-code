#include "stm32f10x.h"
#include "usart/bsp_usart.h"
#include "spi/bsp_spi_flash.h"

#define BUFFER_SIZE       421
#define WRITE_ADDR        0x2000
#define READ_ADDR         0x2000

uint8_t tx_buff[BUFFER_SIZE];
uint8_t rx_buff[BUFFER_SIZE];

int main(void)
{
  uint32_t i, id;
  USARTx_Init();
  SPI_Flash_Init();
  
  printf("\n---SPI ��д FLASH ����---\n");
  
  /* ��ȡ Flash оƬ ID */
  id = Flash_JEDEC_ID();
  if (id == sFLASH_ID) {
    FLASH_INFO("FOUND W25Q64!");
  } else {
    FLASH_ERROR("ID UNKNOWN: 0x%X", id);
  }
  
  /* д��ǰ���Ȳ������� */
  Flash_Sector_Erase(WRITE_ADDR);
  
  FLASH_INFO("Write %d bytes, start address: 0x%x", BUFFER_SIZE, WRITE_ADDR);
  /* ������� */
  for (i = 0; i < BUFFER_SIZE; i++) {
    tx_buff[i] = i % 0xff;
    printf("0x%02x ", tx_buff[i]);
  }
  putchar('\n');
  
  /* д������ */
  Flash_Write_Data(tx_buff, WRITE_ADDR, BUFFER_SIZE);
  
  /* ��ȡ���� */
  Flash_Read_Data(rx_buff, READ_ADDR, BUFFER_SIZE);
  
  FLASH_INFO("Read %d bytes, start address: 0x%x", BUFFER_SIZE, READ_ADDR);
  for (i = 0; i < BUFFER_SIZE; i++) {
    printf("0x%02x ", rx_buff[i]);
  }
  putchar('\n');
  
  /* �Ƚ����� */
  for (i = 0; i < BUFFER_SIZE; i++) {
    if (rx_buff[i] != tx_buff[i]) break;
  }
  if (i == BUFFER_SIZE) {
    FLASH_INFO("Good! It's same.");
  } else {
    FLASH_ERROR("It's different! Something wrong!");
  }
  
  /* ��������ȡ���� */
  double pi = 3.1415927;
  double pi_tmp;
  Flash_Write_Data((uint8_t*)&pi, WRITE_ADDR+BUFFER_SIZE, sizeof(double));
  Flash_Read_Data((uint8_t*)&pi_tmp, WRITE_ADDR+BUFFER_SIZE, sizeof(double));
  printf("\nWrite data: %f, Read data: %f\n%s\n", pi, pi_tmp,
          (pi == pi_tmp) ? "Success" : "Error");
  
  while(1) {
  }
}
