#ifndef __FONT_H
#define __FONT_H

#include "stm32f10x.h"

typedef struct _tFont
{
  const uint8_t *table;
  uint16_t Width;
  uint16_t Height;
} sFONT;

extern sFONT Font24x32;
extern sFONT Font16x24;
extern sFONT Font8x16;

#define CH_CHAR_WIDTH   16  // 中文字符宽度
#define CH_CHAR_HEIGHT  16  // 中文字符高度

#define LINE(x) ((x) * (((sFONT *)LCD_GetFont())->Height))

#define GBKCODE_FLASH

#ifdef GBKCODE_FLASH
#define GBKCODE_START_ADDR  387*4096
#define GetGBKCode(buff, ch)  GetGBKCodeFromEXFlash(buff, ch)
void GetGBKCodeFromEXFlash(uint8_t *buff, uint16_t ch);
#else
//#define GBKCODE_FILE_NAME   "0:/Font/GB2312_H1616.FON"
//#define GetGBKCode(buff, ch)  GetGBKCodeFromSD(buff, ch)
//void GetGBKCodeFromSD(uint8_t *buff, uint16_t ch);
#endif

#endif /* __FONT_H */
