#include "stm32f10x.h"
#include "usart/bsp_usart.h"
#include "fatfs/ff.h"
#include "fatfs_test_func.h"

FATFS fsObject;
FIL fp;

#define TEST_FILENAME "FatFs文件系统读写测试文件.txt"
#define BUFF_SIZE 1024
const char testText[] = "  This is a test text file.\
  \r\n  这是一个FatFs文件系统移植测试文件。\r\n";
char buff[BUFF_SIZE] = "";
//BYTE work[FF_MAX_SS];

UINT bw;  /* Number of bytes written */
UINT br;  /* Number of bytes read */

int main(void)
{
  FRESULT res;
  DWORD fre_sect, tot_sect;
  USARTx_Init();
  printf("--- Flash FatFs 文件系统测试 ---\n");
  /* 挂载文件系统 */
  res = f_mount(&fsObject, "1:", 1);
  printf(" f_mount res = %d\n", res);
  if(res == FR_NO_FILESYSTEM)	{   /* 没有文件系统就进行格式化 */
//		res = f_mkfs("1:", FM_FAT, 0, work, FF_MAX_SS);
    res = f_mkfs("1:", FM_FAT, 0, NULL, 0);
		printf(" f_mkfs res =%d\n", res);
		// 格式化后要取消挂载再重新挂载文件系统
		res = f_mount(NULL, "1:", 1);
		res = f_mount(&fsObject, "1:", 1);
		printf(" second f_mount res =%d\n", res);
	}
  if (res == FR_OK) {
    printf("\n---设备信息获取---\n");
    res = get_space_info("1:", &fre_sect, &tot_sect);
  }
  /* 第一次打开 写 */
  if (res == FR_OK) {
    printf("\n---文件写入测试---\n");
    res = f_open(&fp, "1:"TEST_FILENAME, FA_OPEN_ALWAYS | FA_READ | FA_WRITE);
    printf(" f_open res = %d\n", res);
  }
  if (res == FR_OK) {
    res = f_lseek(&fp, f_size(&fp));  /* 文件指针定位到末尾 */
    printf(" f_lseek res = %d\n", res);
  }
  if (res == FR_OK) {
//    res = f_write(&fp, testText, sizeof(testText), &bw);  /* 写入文件 */
//    printf(" f_write res = %d, bw = %d\n", res, bw);
    f_printf(&fp, "%s%6lu KiB total drive space.%6lu KiB available.\n",
           testText, tot_sect / 2 * 8, fre_sect / 2 * 8);
    printf("Write data: %s%6lu KiB total drive space.%6lu KiB available.\n",
           testText, tot_sect / 2 * 8, fre_sect / 2 * 8);
  }
  if (res == FR_OK) {
    res = f_close(&fp);   /* 关闭文件 */
    printf(" f_close res = %d\n", res);
  }
  /* 第二次打开 读 */
  if (res == FR_OK) {
    printf("\n---文件读取测试---\n");
    res = f_open(&fp, "1:"TEST_FILENAME, FA_READ);  /* 打开文件 */
    printf(" second f_open res = %d\n", res);
  }
  if (res == FR_OK) {
    while(f_eof(&fp) == 0) {
      res = f_read(&fp, buff, BUFF_SIZE, &br);   /* 读取文件 */
      printf("\n>>> f_read res = %d, br = %d\n", res, br);
      for (uint32_t i = 0; i < br; i++) {
        putchar(buff[i]);
      }
    }
  }
  if (res == FR_OK) {
    res = f_close(&fp);     /* 关闭文件 */
    printf(" second f_close res = %d\n", res);
  }
  
  printf("\n---文件信息获取测试---\n");
  file_check("1:"TEST_FILENAME);
  
  if (res == FR_OK) {
    strcpy(buff, "1:");
    printf("\n---文件扫描测试---\n");
    res = scan_files(buff);
  }
  
//  if (res == FR_OK) {
//    printf("\n---删除文件---\n");
//    res = f_unlink("1:"TEST_FILENAME);
//  }
  
  f_mount(NULL, "1:", 1);   /* 取消挂载文件系统 */
  
  printf("FatFs Test: %s\n", (res == FR_OK) ? "Success" : "Error");
  
  while(1) {
  }
}


