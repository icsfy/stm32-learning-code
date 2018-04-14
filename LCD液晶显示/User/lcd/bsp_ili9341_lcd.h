#ifndef __BSP_ILI9341_LCD_H
#define __BSP_ILI9341_LCD_H

#include "stm32f10x.h"
#include <stdio.h>

// ¸´Î»Òý½Å
#define LCD_RST_GPIO_CLK          RCC_APB2Periph_GPIOE
#define LCD_RST_GPIO_PORT         GPIOE
#define LCD_RST_GPIO_PIN          GPIO_Pin_1

// ±³¹âÒý½Å
#define LCD_BL_GPIO_CLK           RCC_APB2Periph_GPIOD
#define LCD_BL_GPIO_PORT          GPIOD
#define LCD_BL_GPIO_PIN           GPIO_Pin_12

// Æ¬Ñ¡
#define LCD_CS_GPIO_CLK           RCC_APB2Periph_GPIOD
#define LCD_CS_GPIO_PORT          GPIOD
#define LCD_CS_GPIO_PIN           GPIO_Pin_7

// ¶ÁÊ¹ÄÜ
#define LCD_RD_GPIO_CLK           RCC_APB2Periph_GPIOD
#define LCD_RD_GPIO_PORT          GPIOD
#define LCD_RD_GPIO_PIN           GPIO_Pin_4

// Ð´Ê¹ÄÜ
#define LCD_WE_GPIO_CLK           RCC_APB2Periph_GPIOD
#define LCD_WE_GPIO_PORT          GPIOD
#define LCD_WE_GPIO_PIN           GPIO_Pin_5

// µØÖ·ÐÅºÅ¿ØÖÆ
#define LCD_RS_GPIO_CLK           RCC_APB2Periph_GPIOD
#define LCD_RS_GPIO_PORT          GPIOD
#define LCD_RS_GPIO_PIN           GPIO_Pin_11

/* FSMC_D0~15 */
#define LCD_D0_GPIO_CLK           RCC_APB2Periph_GPIOD
#define LCD_D0_GPIO_PORT          GPIOD
#define LCD_D0_GPIO_PIN           GPIO_Pin_14

#define LCD_D1_GPIO_CLK           RCC_APB2Periph_GPIOD
#define LCD_D1_GPIO_PORT          GPIOD
#define LCD_D1_GPIO_PIN           GPIO_Pin_15

#define LCD_D2_GPIO_CLK           RCC_APB2Periph_GPIOD
#define LCD_D2_GPIO_PORT          GPIOD
#define LCD_D2_GPIO_PIN           GPIO_Pin_0

#define LCD_D3_GPIO_CLK           RCC_APB2Periph_GPIOD
#define LCD_D3_GPIO_PORT          GPIOD
#define LCD_D3_GPIO_PIN           GPIO_Pin_1

#define LCD_D4_GPIO_CLK           RCC_APB2Periph_GPIOE
#define LCD_D4_GPIO_PORT          GPIOE
#define LCD_D4_GPIO_PIN           GPIO_Pin_7

#define LCD_D5_GPIO_CLK           RCC_APB2Periph_GPIOE
#define LCD_D5_GPIO_PORT          GPIOE
#define LCD_D5_GPIO_PIN           GPIO_Pin_8

#define LCD_D6_GPIO_CLK           RCC_APB2Periph_GPIOE
#define LCD_D6_GPIO_PORT          GPIOE
#define LCD_D6_GPIO_PIN           GPIO_Pin_9

#define LCD_D7_GPIO_CLK           RCC_APB2Periph_GPIOE
#define LCD_D7_GPIO_PORT          GPIOE
#define LCD_D7_GPIO_PIN           GPIO_Pin_10

#define LCD_D8_GPIO_CLK           RCC_APB2Periph_GPIOE
#define LCD_D8_GPIO_PORT          GPIOE
#define LCD_D8_GPIO_PIN           GPIO_Pin_11

#define LCD_D9_GPIO_CLK           RCC_APB2Periph_GPIOE
#define LCD_D9_GPIO_PORT          GPIOE
#define LCD_D9_GPIO_PIN           GPIO_Pin_12

#define LCD_D10_GPIO_CLK          RCC_APB2Periph_GPIOE
#define LCD_D10_GPIO_PORT         GPIOE
#define LCD_D10_GPIO_PIN          GPIO_Pin_13

#define LCD_D11_GPIO_CLK          RCC_APB2Periph_GPIOE
#define LCD_D11_GPIO_PORT         GPIOE
#define LCD_D11_GPIO_PIN          GPIO_Pin_14

#define LCD_D12_GPIO_CLK          RCC_APB2Periph_GPIOE
#define LCD_D12_GPIO_PORT         GPIOE
#define LCD_D12_GPIO_PIN          GPIO_Pin_15

#define LCD_D13_GPIO_CLK          RCC_APB2Periph_GPIOD
#define LCD_D13_GPIO_PORT         GPIOD
#define LCD_D13_GPIO_PIN          GPIO_Pin_8

#define LCD_D14_GPIO_CLK          RCC_APB2Periph_GPIOD
#define LCD_D14_GPIO_PORT         GPIOD
#define LCD_D14_GPIO_PIN          GPIO_Pin_9

#define LCD_D15_GPIO_CLK          RCC_APB2Periph_GPIOD
#define LCD_D15_GPIO_PORT         GPIOD
#define LCD_D15_GPIO_PIN          GPIO_Pin_10

#define LCD_BANK_NORSRAMx         FSMC_Bank1_NORSRAM1

#define LCD_CMD_ADDR              0x60000000
#define LCD_DATA_ADDR             0x60020000

#define DEBUG_DELAY()

#define CMD_SetCoordinateX        0x2A       //ÉèÖÃX×ø±ê
#define CMD_SetCoordinateY        0x2B       //ÉèÖÃY×ø±ê
#define CMD_SetPixel              0x2C       //Ìî³äÏñËØ

#define RGB888_2_RGB565(R,G,B)    (uint16_t)(((R & 0xF8) << 8) | \
                                             ((G & 0xFC) << 3) | \
                                             (B >> 3))
enum LCD_COLOR {
  BLACK   = 0x0000,   // ºÚ
  WHITE   = 0xFFFF,   // °×
  RED     = 0xF800,   // ºì
  GREEN   = 0x07E0,   // ÂÌ
  BLUE    = 0x001F,   // À¶
  MAGENTA = 0xF81F,   // Ñóºì£¬×Ïºì
  CYAN    = 0x07FF,   // Çà
  YELLOW  = 0xFFE0,   // »Æ
  PURPLE  = 0x8010,   // ×Ï
  ORANGE  = 0xFD20,   // ³È
  GRAY    = 0x8410,   // »Ò
};

void LCD_Init(void);
void LCD_Clear(uint16_t x, uint16_t y, uint16_t width, uint16_t height);
uint16_t LCD_ReadPixelFormat(void);
uint16_t LCD_GetPointPixel(uint16_t x, uint16_t y);
void LCD_SetBgColor(uint16_t bgColor);
void LCD_SetFgColor(uint16_t fgColor);
void LCD_SetColors(uint16_t fgColor, uint16_t bgColor);
void LCD_GetColors(uint16_t *fgColor, uint16_t *bgColor);
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void LCD_DrawRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t fill);
void LCD_DrawCircle(uint16_t x, uint16_t y, uint16_t r, uint8_t fill);

#endif /* __BSP_ILI9341_LCD_H */
