#include "lcd/bsp_ili9341_lcd.h"

static uint16_t LCD_FG_COLOR = BLACK;   // 前景色
static uint16_t LCD_BG_COLOR = WHITE;   // 背景色

static void LCD_GPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
  RCC_APB2PeriphClockCmd(LCD_RST_GPIO_CLK | LCD_BL_GPIO_CLK | LCD_CS_GPIO_CLK |
      LCD_RD_GPIO_CLK | LCD_WE_GPIO_CLK | LCD_RS_GPIO_CLK | LCD_D0_GPIO_CLK |
      LCD_D1_GPIO_CLK | LCD_D2_GPIO_CLK | LCD_D3_GPIO_CLK | LCD_D4_GPIO_CLK |
      LCD_D5_GPIO_CLK | LCD_D6_GPIO_CLK | LCD_D7_GPIO_CLK | LCD_D8_GPIO_CLK |
      LCD_D9_GPIO_CLK | LCD_D10_GPIO_CLK | LCD_D11_GPIO_CLK | LCD_D12_GPIO_CLK |
      LCD_D13_GPIO_CLK | LCD_D14_GPIO_CLK | LCD_D15_GPIO_CLK, ENABLE);
  
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
  /* 复位引脚 */
  GPIO_InitStruct.GPIO_Pin = LCD_RST_GPIO_PIN;
  GPIO_Init(LCD_RST_GPIO_PORT, &GPIO_InitStruct);
  /* 背光引脚 */
  GPIO_InitStruct.GPIO_Pin = LCD_BL_GPIO_PIN;
  GPIO_Init(LCD_BL_GPIO_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
  /* 片选 */
  GPIO_InitStruct.GPIO_Pin = LCD_CS_GPIO_PIN;
  GPIO_Init(LCD_CS_GPIO_PORT, &GPIO_InitStruct);
  /* 读 */
  GPIO_InitStruct.GPIO_Pin = LCD_RD_GPIO_PIN;
  GPIO_Init(LCD_RD_GPIO_PORT, &GPIO_InitStruct);
  /* 写 */
  GPIO_InitStruct.GPIO_Pin = LCD_WE_GPIO_PIN;
  GPIO_Init(LCD_WE_GPIO_PORT, &GPIO_InitStruct);
  /* 地址信号线 */
  GPIO_InitStruct.GPIO_Pin = LCD_RS_GPIO_PIN;
  GPIO_Init(LCD_RS_GPIO_PORT, &GPIO_InitStruct);
  /* FSMC_D0~15 */
  GPIO_InitStruct.GPIO_Pin = LCD_D0_GPIO_PIN;
  GPIO_Init(LCD_D0_GPIO_PORT, &GPIO_InitStruct);
  GPIO_InitStruct.GPIO_Pin = LCD_D1_GPIO_PIN;
  GPIO_Init(LCD_D1_GPIO_PORT, &GPIO_InitStruct);
  GPIO_InitStruct.GPIO_Pin = LCD_D2_GPIO_PIN;
  GPIO_Init(LCD_D2_GPIO_PORT, &GPIO_InitStruct);
  GPIO_InitStruct.GPIO_Pin = LCD_D3_GPIO_PIN;
  GPIO_Init(LCD_D3_GPIO_PORT, &GPIO_InitStruct);
  GPIO_InitStruct.GPIO_Pin = LCD_D4_GPIO_PIN;
  GPIO_Init(LCD_D4_GPIO_PORT, &GPIO_InitStruct);
  GPIO_InitStruct.GPIO_Pin = LCD_D5_GPIO_PIN;
  GPIO_Init(LCD_D5_GPIO_PORT, &GPIO_InitStruct);
  GPIO_InitStruct.GPIO_Pin = LCD_D6_GPIO_PIN;
  GPIO_Init(LCD_D6_GPIO_PORT, &GPIO_InitStruct);
  GPIO_InitStruct.GPIO_Pin = LCD_D7_GPIO_PIN;
  GPIO_Init(LCD_D7_GPIO_PORT, &GPIO_InitStruct);
  GPIO_InitStruct.GPIO_Pin = LCD_D8_GPIO_PIN;
  GPIO_Init(LCD_D8_GPIO_PORT, &GPIO_InitStruct);
  GPIO_InitStruct.GPIO_Pin = LCD_D9_GPIO_PIN;
  GPIO_Init(LCD_D9_GPIO_PORT, &GPIO_InitStruct);
  GPIO_InitStruct.GPIO_Pin = LCD_D10_GPIO_PIN;
  GPIO_Init(LCD_D10_GPIO_PORT, &GPIO_InitStruct);
  GPIO_InitStruct.GPIO_Pin = LCD_D11_GPIO_PIN;
  GPIO_Init(LCD_D11_GPIO_PORT, &GPIO_InitStruct);
  GPIO_InitStruct.GPIO_Pin = LCD_D12_GPIO_PIN;
  GPIO_Init(LCD_D12_GPIO_PORT, &GPIO_InitStruct);
  GPIO_InitStruct.GPIO_Pin = LCD_D13_GPIO_PIN;
  GPIO_Init(LCD_D13_GPIO_PORT, &GPIO_InitStruct);
  GPIO_InitStruct.GPIO_Pin = LCD_D14_GPIO_PIN;
  GPIO_Init(LCD_D14_GPIO_PORT, &GPIO_InitStruct);
  GPIO_InitStruct.GPIO_Pin = LCD_D15_GPIO_PIN;
  GPIO_Init(LCD_D15_GPIO_PORT, &GPIO_InitStruct);
}

static void LCD_FSMC_Config(void)
{
  FSMC_NORSRAMTimingInitTypeDef Timing_InitStruct;
  FSMC_NORSRAMInitTypeDef NOR_InitStruct;
  
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
  
  Timing_InitStruct.FSMC_AccessMode = FSMC_AccessMode_B;
  Timing_InitStruct.FSMC_AddressSetupTime = 1;
  Timing_InitStruct.FSMC_DataSetupTime = 4;
  /* unused */
  Timing_InitStruct.FSMC_AddressHoldTime = 0;
  Timing_InitStruct.FSMC_BusTurnAroundDuration = 0;
  Timing_InitStruct.FSMC_CLKDivision = 0;
  Timing_InitStruct.FSMC_DataLatency = 0;
  
  NOR_InitStruct.FSMC_Bank = LCD_BANK_NORSRAMx;
  NOR_InitStruct.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
  NOR_InitStruct.FSMC_MemoryType = FSMC_MemoryType_NOR;
  NOR_InitStruct.FSMC_ReadWriteTimingStruct = &Timing_InitStruct;
  NOR_InitStruct.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
  /* unused */
  NOR_InitStruct.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
  NOR_InitStruct.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
  NOR_InitStruct.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
  NOR_InitStruct.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
  NOR_InitStruct.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
  NOR_InitStruct.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
  NOR_InitStruct.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
  NOR_InitStruct.FSMC_WrapMode = FSMC_WrapMode_Disable;
  NOR_InitStruct.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
  NOR_InitStruct.FSMC_WriteTimingStruct = &Timing_InitStruct;
  
  FSMC_NORSRAMInit(&NOR_InitStruct);
  FSMC_NORSRAMCmd(LCD_BANK_NORSRAMx, ENABLE);
}

static __inline void LCD_Write_Cmd(uint16_t cmd)
{
  *(__IO uint16_t *)LCD_CMD_ADDR = cmd;
}

static __inline void LCD_Write_Data(uint16_t data)
{
  *(__IO uint16_t *)LCD_DATA_ADDR = data;
}

static __inline uint16_t LCD_Read_Data(void)
{
  return *(__IO uint16_t *)LCD_DATA_ADDR;
}

void LCD_BackLed_Cmd(FunctionalState state)
{
  if (state == ENABLE) {
    GPIO_ResetBits(LCD_BL_GPIO_PORT, LCD_BL_GPIO_PIN);
  } else {
    GPIO_SetBits(LCD_BL_GPIO_PORT, LCD_BL_GPIO_PIN);
  }
}

static void LCD_Delay(__IO uint32_t count)
{
  for (; count != 0; --count);
}

void LCD_Reset(void)
{
  GPIO_ResetBits(LCD_RST_GPIO_PORT, LCD_RST_GPIO_PIN);  //低电平复位
  LCD_Delay(0xAFF);
  GPIO_SetBits(LCD_RST_GPIO_PORT, LCD_RST_GPIO_PIN);
  LCD_Delay(0xAFF);
}

static void LCD_REG_Config(void)
{
  /*  Power control B (CFh)  */
  DEBUG_DELAY();
  LCD_Write_Cmd(0xCF);
  LCD_Write_Data(0x00);
  LCD_Write_Data(0x81);
  LCD_Write_Data(0x30);
  
  /*  Power on sequence control (EDh) */
  DEBUG_DELAY();
  LCD_Write_Cmd(0xED);
  LCD_Write_Data(0x64);
  LCD_Write_Data(0x03);
  LCD_Write_Data(0x12);
  LCD_Write_Data(0x81);
  
  /*  Driver timing control A (E8h) */
  DEBUG_DELAY();
  LCD_Write_Cmd(0xE8);
  LCD_Write_Data(0x85);
  LCD_Write_Data(0x10);
  LCD_Write_Data(0x78);
  
  /*  Power control A (CBh) */
  DEBUG_DELAY();
  LCD_Write_Cmd(0xCB);
  LCD_Write_Data(0x39);
  LCD_Write_Data(0x2C);
  LCD_Write_Data(0x00);
  LCD_Write_Data(0x34);
  LCD_Write_Data(0x02);
  
  /* Pump ratio control (F7h) */
  DEBUG_DELAY();
  LCD_Write_Cmd(0xF7);
  LCD_Write_Data(0x20);
  
  /* Driver timing control B */
  DEBUG_DELAY();
  LCD_Write_Cmd(0xEA);
  LCD_Write_Data(0x00);
  LCD_Write_Data(0x00);
  
  /* Frame Rate Control (In Normal Mode/Full Colors) (B1h) */
  DEBUG_DELAY();
  LCD_Write_Cmd(0xB1);
  LCD_Write_Data(0x00);
  LCD_Write_Data(0x1B);
  
  /*  Display Function Control (B6h) */
  DEBUG_DELAY();
  LCD_Write_Cmd(0xB6);
  LCD_Write_Data(0x0A);
  LCD_Write_Data(0xA2);
  
  /* Power Control 1 (C0h) */
  DEBUG_DELAY();
  LCD_Write_Cmd(0xC0);
  LCD_Write_Data(0x35);
  
  /* Power Control 2 (C1h) */
  DEBUG_DELAY();
  LCD_Write_Cmd(0xC1);
  LCD_Write_Data(0x11);
  
  /* VCOM Control 1 (C5h) */
  LCD_Write_Cmd(0xC5);
  LCD_Write_Data(0x45);
  LCD_Write_Data(0x45);
  
  /*  VCOM Control 2 (C7h)  */
  LCD_Write_Cmd(0xC7);
  LCD_Write_Data(0xA2);
  
  /* Enable 3G (F2h) */
  LCD_Write_Cmd(0xF2);
  LCD_Write_Data(0x00);
  
  /* Gamma Set (26h) */
  LCD_Write_Cmd(0x26);
  LCD_Write_Data(0x01);
  DEBUG_DELAY();
  
  /* Positive Gamma Correction */
  LCD_Write_Cmd(0xE0); //Set Gamma
  LCD_Write_Data(0x0F);
  LCD_Write_Data(0x26);
  LCD_Write_Data(0x24);
  LCD_Write_Data(0x0B);
  LCD_Write_Data(0x0E);
  LCD_Write_Data(0x09);
  LCD_Write_Data(0x54);
  LCD_Write_Data(0xA8);
  LCD_Write_Data(0x46);
  LCD_Write_Data(0x0C);
  LCD_Write_Data(0x17);
  LCD_Write_Data(0x09);
  LCD_Write_Data(0x0F);
  LCD_Write_Data(0x07);
  LCD_Write_Data(0x00);
  
  /* Negative Gamma Correction (E1h) */
  LCD_Write_Cmd(0xE1); //Set Gamma
  LCD_Write_Data(0x00);
  LCD_Write_Data(0x19);
  LCD_Write_Data(0x1B);
  LCD_Write_Data(0x04);
  LCD_Write_Data(0x10);
  LCD_Write_Data(0x07);
  LCD_Write_Data(0x2A);
  LCD_Write_Data(0x47);
  LCD_Write_Data(0x39);
  LCD_Write_Data(0x03);
  LCD_Write_Data(0x06);
  LCD_Write_Data(0x06);
  LCD_Write_Data(0x30);
  LCD_Write_Data(0x38);
  LCD_Write_Data(0x0F);
  
  /* memory access control set */
  DEBUG_DELAY();
  LCD_Write_Cmd(0x36);
  LCD_Write_Data(0xC8);    /* 竖屏 左上角(起点)到右下角(终点)扫描方式*/
  DEBUG_DELAY();
  
  /* column address control set */
  LCD_Write_Cmd(CMD_SetCoordinateX);
  LCD_Write_Data(0x00);
  LCD_Write_Data(0x00);
  LCD_Write_Data(0x00);
  LCD_Write_Data(0xEF);
  
  /* page address control set */
  DEBUG_DELAY();
  LCD_Write_Cmd(CMD_SetCoordinateY);
  LCD_Write_Data(0x00);
  LCD_Write_Data(0x00);
  LCD_Write_Data(0x01);
  LCD_Write_Data(0x3F);
  
  /*  Pixel Format Set (3Ah)  */
  DEBUG_DELAY();
  LCD_Write_Cmd(0x3a); 
  LCD_Write_Data(0x55);
  
  /* Sleep Out (11h)  */
  LCD_Write_Cmd(0x11);
  LCD_Delay(0xAFFf<<2);
  DEBUG_DELAY();
  
  /* Display ON (29h) */
  LCD_Write_Cmd(0x29);
}

static void LCD_OpenWindow(uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
  LCD_Write_Cmd(CMD_SetCoordinateX);  // 设置X坐标
  LCD_Write_Data((x >> 8) & 0xff);    // X 起始坐标高8位
  LCD_Write_Data(x & 0xff);           // X 起始坐标低8位
  LCD_Write_Data(((x + width - 1) >> 8) & 0xff);  // X 结束坐标高8位
  LCD_Write_Data((x + width - 1) & 0xff);         // X 结束坐标低8位
  
  LCD_Write_Cmd(CMD_SetCoordinateY);  // 设置Y坐标
  LCD_Write_Data((y >> 8) & 0xff);    // Y 起始坐标高8位
  LCD_Write_Data(y & 0xff);           // Y 起始坐标低8位
  LCD_Write_Data(((y + height - 1) >> 8) & 0xff);   // Y 结束坐标高8位
  LCD_Write_Data((y + height - 1) & 0xff);          // Y 结束坐标低8位
}

void LCD_GramScan(uint8_t option)
{
  uint16_t LCD_X_LENGTH, LCD_Y_LENGTH;
  //参数检查，只可输入0-7
  if (option > 7) return;
  
  //根据模式更新LCD_SCAN_MODE的值，主要用于触摸屏选择计算参数
  //LCD_SCAN_MODE = option;
  
  //根据模式更新XY方向的像素宽度
  if (option%2 == 0) {
    //0 2 4 6模式下X方向像素宽度为240，Y方向为320
    LCD_X_LENGTH = 240;
    LCD_Y_LENGTH = 320;
  } else {
    //1 3 5 7模式下X方向像素宽度为320，Y方向为240
    LCD_X_LENGTH = 320;
    LCD_Y_LENGTH = 240; 
  }

  //0x36命令参数的高3位可用于设置GRAM扫描方向
  LCD_Write_Cmd(0x36);
  LCD_Write_Data(0x08 | (option<<5));   //根据option的值设置LCD参数，共0-7种模式
  LCD_OpenWindow(0, 0, LCD_X_LENGTH, LCD_Y_LENGTH);

  /* write gram start */
  LCD_Write_Cmd(CMD_SetPixel);
}

static __inline void LCD_SetCursor(uint16_t x, uint16_t y)
{
  LCD_OpenWindow(x, y, 1, 1);
}

static __inline void LCD_FillColor(uint32_t count, uint16_t color)
{
  uint32_t i;
  LCD_Write_Cmd(CMD_SetPixel);
  for (i = 0; i < count; ++i) {
    LCD_Write_Data(color);
  }
}

static __inline void LCD_SetPointPixel(uint16_t x, uint16_t y)
{
  LCD_SetCursor(x, y);
  LCD_FillColor(1, LCD_FG_COLOR);
}

/* 矩形左上角坐标(x, y), 宽width, 高height, 是否填充fill */
void LCD_DrawRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t fill)
{
  if (fill) {
    LCD_OpenWindow(x, y, width, height);
    LCD_Write_Cmd(CMD_SetPixel);
    for (int i = 0; i < height; i++) {
      for (int j = 0; j < width; j++) {
          LCD_Write_Data(LCD_FG_COLOR);
      }
    }
  } else {
    LCD_DrawLine(x, y, x + width - 1, y);
    LCD_DrawLine(x, y, x, y + height - 1);
    LCD_DrawLine(x + width - 1, y, x + width - 1, y + height - 1);
    LCD_DrawLine(x, y + height - 1, x + width - 1, y + height - 1);
  }
}

/* 圆心坐标(x, y), 圆的八分对称性 */
static __inline void _draw_circle_8(uint16_t x, uint16_t y, uint16_t curX, uint16_t curY)
{
  LCD_SetPointPixel(x + curX, y + curY);
  LCD_SetPointPixel(x - curX, y + curY);
  LCD_SetPointPixel(x - curY, y + curX);
  LCD_SetPointPixel(x - curY, y - curX);
  LCD_SetPointPixel(x - curX, y - curY);
  LCD_SetPointPixel(x + curX, y - curY);
  LCD_SetPointPixel(x + curY, y - curX);
  LCD_SetPointPixel(x + curY, y + curX);
}

/* 圆心(x, y), 半径r, 是否填充fill */
void LCD_DrawCircle(uint16_t x, uint16_t y, uint16_t r, uint8_t fill)
{
#if 0
  /* 改进的中点画圆法－Bresenham算法 */
  int16_t curX, curY, d;
  curX = 0, curY = r;
  d = 3 - r * 2;
  if (fill) {   /* 实心圆 */
    for (uint8_t i = curX; i <= curY; ++i)
      _draw_circle_8(x, y, curX, i);
    while (curX <= curY) {
      if (d < 0) {
        d += curX * 4 + 6;
      } else {
        d += (curX - curY) * 4 + 10;
        curY--;
      }
      curX++;
      for (uint8_t i = curX; i <= curY; ++i)
        _draw_circle_8(x, y, curX, i);
    }
  } else {    /* 空心圆 */
    _draw_circle_8(x, y, curX, curY);
    while (curX <= curY) {
      if (d < 0) {
        d += curX * 4 + 6;
      } else {
        d += (curX - curY) * 4 + 10;
        curY--;
      }
      curX++;
      _draw_circle_8(x, y, curX, curY);
    }
  }
#elif 0
  /* 中点画圆法 */
  int16_t curX, curY;
  float d;
  curX = 0, curY = r;
  d = 1.25 - r;
  if (fill) {   /* 实心圆 */
    for (uint8_t i = curX; i <= curY; ++i)
      _draw_circle_8(x, y, curX, i);
    while (curX <= curY) {
      if (d < 0) {
        d += 2 * curX + 3;
      } else {
        d += 2 * (curX - curY) + 5;
        curY--;
      }
      curX++;
      for (uint8_t i = curX; i <= curY; ++i)
        _draw_circle_8(x, y, curX, i);
    }
  } else {    /* 空心圆 */
    _draw_circle_8(x, y, curX, curY);
    while (curX <= curY) {
      if (d < 0) {
        d += 2 * curX + 3;
      } else {
        d += 2 * (curX - curY) + 5;
        curY--;
      }
      curX++;
      _draw_circle_8(x, y, curX, curY);
    }
  }

#else
  /* 快速画圆法 */
  int16_t curX, curY, d;
  curX = 0;
  curY = r;
  d = -r / 2;
  if (fill) {   /* 实心圆 */
    for (uint8_t i = curX; i <= curY; ++i)
      _draw_circle_8(x, y, curX, i);
    if (r % 2 == 0) {
      while (curX < curY) {
        curX++;
        if (d < 0) d += curX;
        else {
          curY--;
          d += curX - curY;
        }
        for (uint8_t i = curX; i <= curY; ++i)
          _draw_circle_8(x, y, curX, i);
      }
    } else {
      while (curX < curY) {
        curX++;
        if (d < 0) d += curX + 1;
        else {
          curY--;
          d += curX - curY + 1;
        }
        for (uint8_t i = curX; i <= curY; ++i)
          _draw_circle_8(x, y, curX, i);
      }
    }
  } else {    /* 空心圆 */
    _draw_circle_8(x, y, curX, curY);
    if (r % 2 == 0) {
      while (curX < curY) {
        curX++;
        if (d < 0) d += curX;
        else {
          curY--;
          d += curX - curY;
        }
        _draw_circle_8(x, y, curX, curY);
      }
    } else {
      while (curX < curY) {
        curX++;
        if (d < 0) d += curX + 1;
        else {
          curY--;
          d += curX - curY + 1;
        }
        _draw_circle_8(x, y, curX, curY);
      }
    }
  }
#endif
}

void LCD_Init()
{
  LCD_GPIO_Config();
  LCD_FSMC_Config();
  
  LCD_BackLed_Cmd(ENABLE);
  LCD_Reset();
  LCD_REG_Config();
  
  //设置默认扫描方向，其中 6 模式为大部分液晶例程的默认显示方向  
  LCD_GramScan(6);
}

uint16_t LCD_ReadPixelFormat(void)
{
  LCD_Write_Cmd(0x0C);
  LCD_Read_Data();
  return LCD_Read_Data();
}

void LCD_Clear(uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
  LCD_OpenWindow(x, y, width, height);
  LCD_Write_Cmd(CMD_SetPixel);
  while (width--) {
    while (height--) {
      LCD_Write_Data(LCD_BG_COLOR);
    }
  }
}

static uint16_t LCD_ReadPixelData(void)
{
  uint16_t R, G, B;
  LCD_Write_Cmd(0x2E);
  LCD_Read_Data();  // Dummy Read
  R = LCD_Read_Data();
  G = LCD_Read_Data();
  B = LCD_Read_Data();
  return (R | G >> 5 | B >> 11);
}

uint16_t LCD_GetPointPixel(uint16_t x, uint16_t y)
{
  LCD_SetCursor(x, y);
  return LCD_ReadPixelData();
}

void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
  int8_t incX = 0, incY = 0;
  int16_t dx, dy, ex = 0, ey = 0, d, i;
  dx = x2 - x1;
  dy = y2 - y1;
  incX = dx > 0;
  if (dx < 0) {
    incX = -1;
    dx = -dx;
  }
  incY = dy > 0;
  if (dy < 0) {
    incY = -1;
    dy = -dy;
  }
  d = dx > dy ? dx : dy;
  for (i = 0; i <= d; ++i) {
    LCD_SetPointPixel(x1, y1);
    ex += dx;
    ey += dy;
    if (ex > d) {
      ex -= d;
      x1 += incX;
    }
    if (ey > d) {
      ey -= d;
      y1 += incY;
    }
  }
}

void LCD_SetBgColor(uint16_t bgColor)
{
  LCD_BG_COLOR = bgColor;
}

void LCD_SetFgColor(uint16_t fgColor)
{
  LCD_FG_COLOR = fgColor;
}

void LCD_SetColors(uint16_t fgColor, uint16_t bgColor)
{
  LCD_FG_COLOR = fgColor;
  LCD_BG_COLOR = bgColor;
}

void LCD_GetColors(uint16_t *fgColor, uint16_t *bgColor)
{
  *fgColor = LCD_FG_COLOR;
  *bgColor = LCD_BG_COLOR;
}
