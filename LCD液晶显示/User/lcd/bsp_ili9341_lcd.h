#ifndef __BSP_ILI9341_LCD_H
#define __BSP_ILI9341_LCD_H

#include "stm32f10x.h"
#include "fonts/fonts.h"

// 复位引脚
#define LCD_RST_GPIO_CLK          RCC_APB2Periph_GPIOE
#define LCD_RST_GPIO_PORT         GPIOE
#define LCD_RST_GPIO_PIN          GPIO_Pin_1

// 背光引脚
#define LCD_BL_GPIO_CLK           RCC_APB2Periph_GPIOD
#define LCD_BL_GPIO_PORT          GPIOD
#define LCD_BL_GPIO_PIN           GPIO_Pin_12

// 片选
#define LCD_CS_GPIO_CLK           RCC_APB2Periph_GPIOD
#define LCD_CS_GPIO_PORT          GPIOD
#define LCD_CS_GPIO_PIN           GPIO_Pin_7

// 读使能
#define LCD_RD_GPIO_CLK           RCC_APB2Periph_GPIOD
#define LCD_RD_GPIO_PORT          GPIOD
#define LCD_RD_GPIO_PIN           GPIO_Pin_4

// 写使能
#define LCD_WE_GPIO_CLK           RCC_APB2Periph_GPIOD
#define LCD_WE_GPIO_PORT          GPIOD
#define LCD_WE_GPIO_PIN           GPIO_Pin_5

// 地址信号控制
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

#define LCD_CMD_ADDR              0x60000000    // 命令操作地址
#define LCD_DATA_ADDR             0x60020000    // 数据操作地址

#define DEBUG_DELAY()

#define CMD_SetCoordinateX        0x2A       //设置X坐标
#define CMD_SetCoordinateY        0x2B       //设置Y坐标
#define CMD_SetPixel              0x2C       //填充像素

#define RGB888_2_RGB565(R,G,B)    (uint16_t)((((R) & 0xF8) << 8) | \
                                             (((G) & 0xFC) << 3) | \
                                             ((B) >> 3))
enum LCD_COLOR {
  BLACK   = 0x0000,   // 黑
  WHITE   = 0xFFFF,   // 白
  RED     = 0xF800,   // 红
  GREEN   = 0x07E0,   // 绿
  BLUE    = 0x001F,   // 蓝
  MAGENTA = 0xF81F,   // 洋红，紫红
  CYAN    = 0x07FF,   // 青
  YELLOW  = 0xFFE0,   // 黄
  PURPLE  = 0x8010,   // 紫
  ORANGE  = 0xFD20,   // 橙
  GRAY    = 0x8410,   // 灰
};

extern uint8_t LCD_SCAN_MODE;
extern uint16_t LCD_X_LENGTH, LCD_Y_LENGTH;

void LCD_Init(void);
void LCD_GramScan(uint8_t option);
void LCD_Clear(uint16_t x, uint16_t y, uint16_t width, uint16_t height);
uint16_t LCD_ReadPixelFormat(void);
uint16_t LCD_GetPointPixel(uint16_t x, uint16_t y);
void LCD_SetBgColor(uint16_t bgColor);
void LCD_SetFgColor(uint16_t fgColor);
void LCD_SetFont(sFONT *fonts);
sFONT *LCD_GetFont(void);
void LCD_SetColors(uint16_t fgColor, uint16_t bgColor);
void LCD_GetColors(uint16_t *fgColor, uint16_t *bgColor);
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void LCD_DrawRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t fill);
void LCD_DrawCircle(uint16_t x, uint16_t y, uint16_t r, uint8_t fill);
void LCD_DisplayChar_EN(uint16_t x, uint16_t y, const char c);
void LCD_DisplayString_EN(uint16_t x, uint16_t y, const char *pStr);
void LCD_DisplayStringLine_EN(uint16_t line, const char *pStr);
void LCD_DisplayChar_CH(uint16_t x, uint16_t y, uint16_t ch);
void LCD_DisplayString_CH(uint8_t x, uint8_t y, const char *str);
void LCD_DisplayString(uint16_t x, uint16_t y, const char *str);

#endif /* __BSP_ILI9341_LCD_H */
