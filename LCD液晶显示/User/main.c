#include "stm32f10x.h"
#include "usart/bsp_usart.h"
#include "lcd/bsp_ili9341_lcd.h"
#include "lcd/bsp_xpt2046_lcd.h"
#include <string.h>

void softDelay(int count)
{
  int n;
  for (int i = 0; i < count; ++i) n += i;
}

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

void LCD_Test2()
{
  LCD_SetColors(WHITE, BLACK);
  LCD_DisplayChar_EN(120, 160, 'a');
  LCD_DisplayString_EN(100, 300, "hello, world! This is a long string Test! This is a long string Test!");
  LCD_DisplayStringLine_EN(LINE(3), "This should be the third line.");
  LCD_DisplayChar_CH(30, 64, *(uint16_t *)"啊");
  LCD_DisplayString_CH(200, 64, "这是中文测试！");
  LCD_SetFgColor(RGB888_2_RGB565(147, 63, 13));
  LCD_DisplayString(100, 96, "这li是yi中文ce测试(This is English）");
}

void LCD_Test3()
{
  softDelay(0xffffff);
  while (XPT2046_Touch_Calibrate(LCD_SCAN_MODE) == ERROR) {
    XPT2046_ERROR("Calibration Error!");
  } {
    XPT2046_INFO("Calibration Success!");
  }
  LCD_Clear(0, 0, LCD_X_LENGTH, LCD_Y_LENGTH);
  char *str = "Touch to continue";
  LCD_DisplayString_EN((LCD_X_LENGTH - strlen(str) * LCD_GetFont()->Width) / 2, LCD_Y_LENGTH / 2, str);
  while (1) {
    XPT2046_TouchEvenHandler();
  }
}

void Touch_Trace(XPT2046_Coordinate *touch, uint8_t sta)
{
  char buff[32];
  static uint8_t cleared = 0;
  if (sta == 1) {
    if (cleared == 0) {
      LCD_Clear(0, 0, LCD_X_LENGTH, LCD_Y_LENGTH);
      cleared = 1;
    }
    sprintf(buff, "Touch Down: x=%d, y=%d", touch->x, touch->y);
    LCD_DisplayStringLine_EN(LINE(0), buff);
    LCD_Clear(touch->pre_x, 0, 1, LCD_Y_LENGTH);
    LCD_Clear(0, touch->pre_y, LCD_X_LENGTH, 1);
    LCD_DisplayStringLine_EN(LINE(0), buff);
    LCD_DrawLine(touch->x, 0, touch->x, LCD_Y_LENGTH);
    LCD_DrawLine(0, touch->y, LCD_X_LENGTH, touch->y);
  } else {
    sprintf(buff, "Touch Up  : x=%d, y=%d", touch->x, touch->y);
    LCD_Clear(0, 0, LCD_X_LENGTH, LCD_Y_LENGTH);
    LCD_DisplayStringLine_EN(LINE(0), buff);
    LCD_DrawLine(touch->x, 0, touch->x, LCD_Y_LENGTH);
    LCD_DrawLine(0, touch->y, LCD_X_LENGTH, touch->y);
    cleared = 0;
  }
}

int main(void)
{
  USARTx_Init();
  LCD_Init();
  XPT2046_Init();
  printf("\n---LCD TEST---\n");
  printf("\nRead Display Pixel Format: 0x%x\n", LCD_ReadPixelFormat());
  LCD_Test1();
  LCD_Test2();
  LCD_Test3();
  while(1) {
  }
}
