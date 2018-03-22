#include "stm32f10x.h"
#include "usart/bsp_usart.h"
#include "fatfs/ff.h"
#include "fatfs_test_func.h"

FATFS fsObject;
FIL fp;

#define TEST_FILENAME "FatFs�ļ�ϵͳ��д�����ļ�.txt"
#define BUFF_SIZE 1024
const char testText[] = "  This is a test text file.\
  \r\n  ����һ��FatFs�ļ�ϵͳ��ֲ�����ļ���\r\n";
char buff[BUFF_SIZE] = "";
//BYTE work[FF_MAX_SS];

UINT bw;  /* Number of bytes written */
UINT br;  /* Number of bytes read */

int main(void)
{
  FRESULT res;
  DWORD fre_sect, tot_sect;
  USARTx_Init();
  printf("--- Flash FatFs �ļ�ϵͳ���� ---\n");
  /* �����ļ�ϵͳ */
  res = f_mount(&fsObject, "1:", 1);
  printf(" f_mount res = %d\n", res);
  if(res == FR_NO_FILESYSTEM)	{   /* û���ļ�ϵͳ�ͽ��и�ʽ�� */
//		res = f_mkfs("1:", FM_FAT, 0, work, FF_MAX_SS);
    res = f_mkfs("1:", FM_FAT, 0, NULL, 0);
		printf(" f_mkfs res =%d\n", res);
		// ��ʽ����Ҫȡ�����������¹����ļ�ϵͳ
		res = f_mount(NULL, "1:", 1);
		res = f_mount(&fsObject, "1:", 1);
		printf(" second f_mount res =%d\n", res);
	}
  if (res == FR_OK) {
    printf("\n---�豸��Ϣ��ȡ---\n");
    res = get_space_info("1:", &fre_sect, &tot_sect);
  }
  /* ��һ�δ� д */
  if (res == FR_OK) {
    printf("\n---�ļ�д�����---\n");
    res = f_open(&fp, "1:"TEST_FILENAME, FA_OPEN_ALWAYS | FA_READ | FA_WRITE);
    printf(" f_open res = %d\n", res);
  }
  if (res == FR_OK) {
    res = f_lseek(&fp, f_size(&fp));  /* �ļ�ָ�붨λ��ĩβ */
    printf(" f_lseek res = %d\n", res);
  }
  if (res == FR_OK) {
//    res = f_write(&fp, testText, sizeof(testText), &bw);  /* д���ļ� */
//    printf(" f_write res = %d, bw = %d\n", res, bw);
    f_printf(&fp, "%s%6lu KiB total drive space.%6lu KiB available.\n",
           testText, tot_sect / 2 * 8, fre_sect / 2 * 8);
    printf("Write data: %s%6lu KiB total drive space.%6lu KiB available.\n",
           testText, tot_sect / 2 * 8, fre_sect / 2 * 8);
  }
  if (res == FR_OK) {
    res = f_close(&fp);   /* �ر��ļ� */
    printf(" f_close res = %d\n", res);
  }
  /* �ڶ��δ� �� */
  if (res == FR_OK) {
    printf("\n---�ļ���ȡ����---\n");
    res = f_open(&fp, "1:"TEST_FILENAME, FA_READ);  /* ���ļ� */
    printf(" second f_open res = %d\n", res);
  }
  if (res == FR_OK) {
    while(f_eof(&fp) == 0) {
      res = f_read(&fp, buff, BUFF_SIZE, &br);   /* ��ȡ�ļ� */
      printf("\n>>> f_read res = %d, br = %d\n", res, br);
      for (uint32_t i = 0; i < br; i++) {
        putchar(buff[i]);
      }
    }
  }
  if (res == FR_OK) {
    res = f_close(&fp);     /* �ر��ļ� */
    printf(" second f_close res = %d\n", res);
  }
  
  printf("\n---�ļ���Ϣ��ȡ����---\n");
  file_check("1:"TEST_FILENAME);
  
  if (res == FR_OK) {
    strcpy(buff, "1:");
    printf("\n---�ļ�ɨ�����---\n");
    res = scan_files(buff);
  }
  
//  if (res == FR_OK) {
//    printf("\n---ɾ���ļ�---\n");
//    res = f_unlink("1:"TEST_FILENAME);
//  }
  
  f_mount(NULL, "1:", 1);   /* ȡ�������ļ�ϵͳ */
  
  printf("FatFs Test: %s\n", (res == FR_OK) ? "Success" : "Error");
  
  while(1) {
  }
}


