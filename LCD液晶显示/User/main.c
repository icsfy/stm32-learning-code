#include "stm32f10x.h"
#include "usart/bsp_usart.h"
#include "lcd/bsp_ili9341_lcd.h"

void LCD_Test1()
{
  printf("\nDraw Rectangular\n");
  LCD_DrawRect(120, 20, 120, 40, 0);
  LCD_SetFgColor(RED);
  LCD_DrawRect(140, 30, 80, 20, 1);
  printf("\nDraw Circle\n");
  LCD_SetFgColor(GREEN);
  for (int i = 1; i < 100; i += 5)
    LCD_DrawCircle(120, 160, i, i < 80);
  printf("\nDraw Line\n");
  LCD_SetFgColor(BLUE);
  LCD_DrawLine(1, 300, 239, 300);
  LCD_SetFgColor(MAGENTA);
  LCD_DrawLine(200, 310, 20, 310);
  LCD_SetFgColor(CYAN);
  LCD_DrawLine(10, 10, 10, 310);
  LCD_SetFgColor(YELLOW);
  LCD_DrawLine(200, 300, 20, 30);
  LCD_SetFgColor(PURPLE);
  LCD_DrawLine(20, 300, 230, 260);
  LCD_SetFgColor(ORANGE);
  LCD_DrawLine(100, 30, 30, 240);
  LCD_SetBgColor(GRAY);
  LCD_Clear(80, 120, 80, 80);
  LCD_SetBgColor(RGB888_2_RGB565(219,112,147));
  LCD_Clear(20, 20, 40, 40);
}

int main(void)
{
  USARTx_Init();
  LCD_Init();
  printf("\n---LCD TEST---\n");
  printf("\nRead Display Pixel Format: 0x%x\n", LCD_ReadPixelFormat());
  LCD_Test1();
  while(1) {
  }
}
