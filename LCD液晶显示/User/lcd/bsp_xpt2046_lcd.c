#include "lcd/bsp_xpt2046_lcd.h"
#include "lcd/bsp_ili9341_lcd.h"
#include <string.h>

//默认触摸参数，不同的屏幕稍有差异，可重新调用触摸校准函数获取
XPT2046_TouchPara strXPT2046_TouchPara[] = {
 -0.006464,   -0.073259,  280.358032,    0.074878,    0.002052,   -6.545977,  //扫描方式0
  0.086314,    0.001891,  -12.836658,   -0.003722,   -0.065799,  254.715714,  //扫描方式1
  0.002782,    0.061522,  -11.595689,    0.083393,    0.005159,  -15.650089,  //扫描方式2
  0.089743,   -0.000289,  -20.612209,   -0.001374,    0.064451,  -16.054003,  //扫描方式3
  0.000767,   -0.068258,  250.891769,   -0.085559,   -0.000195,  334.747650,  //扫描方式4
 -0.084744,    0.000047,  323.163147,   -0.002109,   -0.066371,  260.985809,  //扫描方式5
 -0.001848,    0.066984,  -12.807136,   -0.084858,   -0.000805,  333.395386,  //扫描方式6
 -0.085470,   -0.000876,  334.023163,   -0.003390,    0.064725,   -6.211169,  //扫描方式7
};

__IO uint8_t XPT2046_TouchFlag = 0;

void XPT2046_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  /* 开启GPIO时钟 */
  RCC_APB2PeriphClockCmd(XPT2046_SPI_GPIO_CLK | XPT2046_PENIRQ_GPIO_CLK, ENABLE);
  /* 模拟SPI GPIO初始化 */
  GPIO_InitStructure.GPIO_Pin = XPT2046_SPI_CLK_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(XPT2046_SPI_CLK_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = XPT2046_SPI_CS_PIN;
  GPIO_Init(XPT2046_SPI_CS_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = XPT2046_SPI_MOSI_PIN;
  GPIO_Init(XPT2046_SPI_MOSI_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   // 上拉输入
  GPIO_InitStructure.GPIO_Pin = XPT2046_SPI_MISO_PIN;
  GPIO_Init(XPT2046_SPI_MISO_PORT, &GPIO_InitStructure);

  //触摸屏触摸信号指示引脚，不使用中断
  GPIO_InitStructure.GPIO_Pin = XPT2046_PENIRQ_GPIO_PIN;
  GPIO_Init(XPT2046_PENIRQ_GPIO_PORT, &GPIO_InitStructure);

  /* 拉低片选，选择XPT2046 */
  XPT2046_CS_LOW();
}

/* 用于 XPT2046 的简单微秒级延时函数 */
static void XPT2046_DelayUS(__IO uint32_t count)
{
  uint32_t i;
  for (i = 0; i < count; ++i) {
    uint8_t uc = 12;     // 设置值为12，大约延1微秒
    while ( uc -- );     // 延1微秒
  }
}

/* XPT2046 的写入命令 */
static void XPT2046_WriteCMD(uint8_t cmd)
{
  uint8_t i;
  XPT2046_MOSI_0();
  XPT2046_CLK_LOW();
  for ( i = 0; i < 8; i ++ ) {
    ((cmd >> (7 - i)) & 0x01) ? XPT2046_MOSI_1() : XPT2046_MOSI_0();
    XPT2046_DelayUS(5);
    XPT2046_CLK_HIGH();
    XPT2046_DelayUS(5);
    XPT2046_CLK_LOW();
  }
}

/* XPT2046 的读取命令 */
static uint16_t XPT2046_ReadCMD(void)
{
  uint8_t i;
  uint16_t data = 0;
  XPT2046_MOSI_0();
  XPT2046_CLK_HIGH();
  for (i = 0; i < 12; ++i) {
    XPT2046_CLK_LOW();
    data <<= 1;
    data |= XPT2046_MISO();
    XPT2046_CLK_HIGH();
  }
  return data;
}

/* 对 XPT2046 选择一个模拟通道后，启动ADC并返回ADC采样结果 */
static uint16_t XPT2046_ReadAdc(uint8_t channel)
{
  XPT2046_WriteCMD(channel);
  return XPT2046_ReadCMD();
}

/* 读取 XPT2046 的X通道和Y通道的AD值(12 bit，最大值4096) */
static void XPT2046_ReadAdc_XY(int16_t *X, int16_t *Y)
{
  *X = XPT2046_ReadAdc(XPT2046_CHANNEL_X);
  XPT2046_DelayUS(1);
  *Y = XPT2046_ReadAdc(XPT2046_CHANNEL_Y);
}

/* 在触摸 XPT2046 屏幕时获取一组坐标的AD值，并对该坐标进行滤波 */
static ErrorStatus XPT2046_ReadAdc_Smooth_XY(XPT2046_Coordinate *pScreenCoordinate)
{
  uint8_t i;
  int16_t buff[2][10];              //坐标X和Y进行多次采样
  uint8_t minX, minY, maxX, maxY;   //存储采样中的最小值、最大值
  /* 循环采样10次 */
  for (i = 0; i < 10; ++i) {
    //用户点击触摸屏时即TP_INT_IN信号为低
    if (XPT2046_PENIRQ_Read() != XPT2046_PENIRQ_ActiveLevel) break;
    XPT2046_ReadAdc_XY(&buff[0][i], &buff[1][i]);
  }
  /* 如果触笔弹起 */
  if (XPT2046_PENIRQ_Read() != XPT2046_PENIRQ_ActiveLevel)
    XPT2046_TouchFlag = 0;      //中断标志复位
  /* 如果成功采样10个样本 */
  if (i ==10) {
    maxX = minX = maxY = minY = 0;
    for (i = 1; i < 10; ++i) {
      if (buff[0][i] < buff[0][minX]) minX = i;
      else if (buff[0][i] > buff[0][maxX]) maxX = i;
      if (buff[1][i] < buff[1][minY]) minY = i;
      else if (buff[1][i] > buff[1][maxY]) maxY = i;
    }
    pScreenCoordinate->x = pScreenCoordinate->y = 0;
    /*去除最小值和最大值之后求平均值*/
    for (i = 0; i < 10; ++i) {
      if ((minX == i) || (maxX == i)) continue;
      pScreenCoordinate->x += buff[0][i];
    }
    pScreenCoordinate->x >>= 3;
    for (i = 0; i < 10; ++i) {
      if ((minY == i) || (maxY == i)) continue;
      pScreenCoordinate->y += buff[1][i];
    }
    pScreenCoordinate->y >>= 3;
    return SUCCESS;
  }
  return ERROR;      
}

/* 获取 XPT2046 触摸点（校准后）的坐标 */
ErrorStatus XPT2046_Get_TouchedPoint(XPT2046_Coordinate *pDisplayCoordinate, XPT2046_TouchPara *pTouchPara)
{
  XPT2046_Coordinate screenCoordinate;
  if (XPT2046_ReadAdc_Smooth_XY(&screenCoordinate) == SUCCESS) {
    pDisplayCoordinate->x = ((pTouchPara[LCD_SCAN_MODE].dX_X * screenCoordinate.x) + (pTouchPara[LCD_SCAN_MODE].dX_Y * screenCoordinate.y) + pTouchPara[LCD_SCAN_MODE].dX);        
    pDisplayCoordinate->y = ((pTouchPara[LCD_SCAN_MODE].dY_X * screenCoordinate.x) + (pTouchPara[LCD_SCAN_MODE].dY_Y * screenCoordinate.y) + pTouchPara[LCD_SCAN_MODE].dY);
  }
  else return ERROR;
  return SUCCESS;
}

/* 触摸屏检测状态机 */
static uint8_t XPT2046_TouchDetect(void)
{
  static TouchState touch_state = XPT2046_STATE_RELEASE;
  static uint32_t i;
  uint8_t res = TOUCH_NOT_PRESSED;
  switch (touch_state) {
    case XPT2046_STATE_RELEASE:
      if (XPT2046_PENIRQ_Read() == XPT2046_PENIRQ_ActiveLevel) {
        // 第一次出现触摸信号
        touch_state = XPT2046_STATE_WAITING;
        res =TOUCH_NOT_PRESSED;
      } else {
        // 无触摸
        touch_state = XPT2046_STATE_RELEASE;
        res =TOUCH_NOT_PRESSED;
      }
      break;
    case XPT2046_STATE_WAITING:
      if (XPT2046_PENIRQ_Read() == XPT2046_PENIRQ_ActiveLevel) {
        i++;
        //等待时间大于阈值则认为触摸被按下
        //消抖时间 = DURIATION_TIME * 本函数被调用的时间间隔
        //如在定时器中调用，每10ms调用一次，则消抖时间为：DURIATION_TIME*10ms
        if (i > DURIATION_TIME) {
          i = 0;
          touch_state = XPT2046_STATE_PRESSED;
          res = TOUCH_PRESSED;
        } else {
          //等待时间累加
          touch_state = XPT2046_STATE_WAITING;
          res = TOUCH_NOT_PRESSED;
        }
      } else {
        //等待时间值未达到阈值就为无效电平，当成抖动处理
        i = 0;
        touch_state = XPT2046_STATE_RELEASE;
        res = TOUCH_NOT_PRESSED;
      }
      break;
    case XPT2046_STATE_PRESSED:
      if (XPT2046_PENIRQ_Read() == XPT2046_PENIRQ_ActiveLevel) {
        //触摸持续按下
        touch_state = XPT2046_STATE_PRESSED;
        res = TOUCH_PRESSED;
      } else {
        //触摸释放
        touch_state = XPT2046_STATE_RELEASE;
        res = TOUCH_NOT_PRESSED;
      }
      break;
    default:
      touch_state = XPT2046_STATE_RELEASE;
      res = TOUCH_NOT_PRESSED;
      break;
  }
  return res;
}

/* 触摸屏被按下的时候会调用本函数 */
void XPT2046_TouchDown(XPT2046_Coordinate *touch)
{
  //若为负值表示之前已处理过
  if(touch->pre_x == -1 && touch->pre_x == -1)
    return;
  {
    extern void Touch_Trace(XPT2046_Coordinate *touch, uint8_t);
    Touch_Trace(touch, 1);
  }
}

/* 触摸屏释放的时候会调用本函数 */
void XPT2046_TouchUp(XPT2046_Coordinate *touch)
{
  //若为负值表示之前已处理过
  if(touch->pre_x == -1 && touch->pre_x == -1)
    return;
  {
    extern void Touch_Trace(XPT2046_Coordinate *touch, uint8_t);
    Touch_Trace(touch, 0);
  }
}

/* 检测到触摸中断时调用的处理函数,通过它调用tp_down 和tp_up汇报触摸点 */
void XPT2046_TouchEvenHandler(void)
{
  static XPT2046_Coordinate cinfo = {-1, -1, -1, -1};
  if (XPT2046_TouchDetect() == TOUCH_PRESSED) {
    // 获取触摸坐标
    XPT2046_Get_TouchedPoint(&cinfo, strXPT2046_TouchPara);
    // 输出调试信息到串口
    XPT2046_DEBUG("x=%d, y=%d", cinfo.x, cinfo.y);
    // 调用触摸被按下时的处理函数，可在该函数编写自己的触摸按下处理过程
    XPT2046_TouchDown(&cinfo);
    /* 更新触摸信息到 pre_x, pre_y */
    cinfo.pre_x = cinfo.x;
    cinfo.pre_y = cinfo.y;
  } else {
    // 调用触摸被释放时的处理函数，可在该函数编写自己的触摸释放处理过程
    XPT2046_TouchUp(&cinfo);
    /* 触笔释放，把 x, y 重置为负 */
    cinfo.x = -1;
    cinfo.y = -1; 
    cinfo.pre_x = -1;
    cinfo.pre_y = -1;
  }
}

/* 在 LCD 上显示校正触摸时需要的十字 */
static void LCD_DrawCross(uint16_t x, uint16_t y)
{
  LCD_DrawLine(x - 10, y, x + 10, y);
  LCD_DrawLine(x, y - 10, x, y + 10);
}

/* 计算 XPT2046 触摸坐标校正系数
 * （注意：只有在LCD和触摸屏间的误差角度非常小时,才能运用下面公式）
 */
static ErrorStatus XPT2046_Calculate_CalibrationFactor(XPT2046_Coordinate *pDisplayCoordinate,
  XPT2046_Coordinate * pScreenSample, XPT2046_Calibration * pCalibrationFactor)
{
  /* K = (X0 - X2)(Y1 - Y2) - (X1 - X2)(Y0 - Y2) */
  pCalibrationFactor->Divider = ((pScreenSample[0].x - pScreenSample[2].x) * (pScreenSample[1].y - pScreenSample[2].y)) -
      ((pScreenSample[1].x - pScreenSample[2].x) * (pScreenSample[0].y - pScreenSample[2].y));
  if (pCalibrationFactor->Divider == 0)
    return ERROR;
  else {
    /* A = ((XD0 - XD2)(Y1 - Y2) - (XD1 - XD2)(Y0 - Y2)) / K */
    pCalibrationFactor->An = ((pDisplayCoordinate[0].x - pDisplayCoordinate[2].x) * (pScreenSample[1].y - pScreenSample[2].y)) -
        ((pDisplayCoordinate[1].x - pDisplayCoordinate[2].x) * (pScreenSample[0].y - pScreenSample[2].y));
    /* B = ((X0 - X2)(XD1 - XD2) - (XD0 - XD2)(X1 - X2)) / K */
    pCalibrationFactor->Bn = ((pScreenSample[0].x - pScreenSample[2].x) * (pDisplayCoordinate[1].x - pDisplayCoordinate[2].x)) -
        ((pDisplayCoordinate[0].x - pDisplayCoordinate[2].x) * (pScreenSample[1].x - pScreenSample[2].x));
    /* C = (Y0(X2XD1 - X1XD2) + Y1(X0XD2 - X2XD0) + Y2(X1XD0 - X0XD1)) / K */
    pCalibrationFactor->Cn = (pScreenSample[2].x * pDisplayCoordinate[1].x - pScreenSample[1].x * pDisplayCoordinate[2].x) * pScreenSample[0].y +
        (pScreenSample[0].x * pDisplayCoordinate[2].x - pScreenSample[2].x * pDisplayCoordinate[0].x) * pScreenSample[1].y +
        (pScreenSample[1].x * pDisplayCoordinate[0].x - pScreenSample[0].x * pDisplayCoordinate[1].x) * pScreenSample[2].y;
    /* D = ((YD0 - YD2)(Y1 - Y2) - (YD1 - YD2)(Y0 - Y2)) / K */
    pCalibrationFactor->Dn = ((pDisplayCoordinate[0].y - pDisplayCoordinate[2].y) * (pScreenSample[1].y - pScreenSample[2].y)) -
        ((pDisplayCoordinate[1].y - pDisplayCoordinate[2].y) * (pScreenSample[0].y - pScreenSample[2].y));
    /* E = ((X0 - X2)(YD1 - YD2) - (YD0 - YD2)(X1 - X2)) / K */
    pCalibrationFactor->En = ((pScreenSample[0].x - pScreenSample[2].x) * (pDisplayCoordinate[1].y - pDisplayCoordinate[2].y)) -
        ((pDisplayCoordinate[0].y - pDisplayCoordinate[2].y) * (pScreenSample[1].x - pScreenSample[2].x));
    /* F = (Y0(X2YD1 - X1YD2) + Y1(X0YD2 - X2YD0) + Y2(X1YD0 - X0YD1)) / K */
    pCalibrationFactor->Fn = (pScreenSample[2].x * pDisplayCoordinate[1].y - pScreenSample[1].x * pDisplayCoordinate[2].y) * pScreenSample[0].y +
        (pScreenSample[0].x * pDisplayCoordinate[2].y - pScreenSample[2].x * pDisplayCoordinate[0].y) * pScreenSample[1].y +
        (pScreenSample[1].x * pDisplayCoordinate[0].y - pScreenSample[0].x * pDisplayCoordinate[1].y) * pScreenSample[2].y;
  }
  return SUCCESS;
}

/* XPT2046 触摸屏校准 */
ErrorStatus XPT2046_Touch_Calibrate(uint8_t LCD_Mode)
{
  uint8_t i;
  char cStr[100];
  uint16_t test_x = 0, test_y = 0, gap_x = 0, gap_y = 0;
  char *pStr = NULL;
  XPT2046_Coordinate crossCoordinate[4], screenSample[4];
  XPT2046_Calibration CalibrationFactor;
  LCD_SetFont(&Font8x16);
  LCD_SetColors(BLUE, BLACK);
  //设置扫描方向，横屏
  LCD_GramScan(LCD_Mode);
  /* 设定“十”字交叉点的坐标 */
  crossCoordinate[0].x = LCD_X_LENGTH >> 2;
  crossCoordinate[0].y = LCD_Y_LENGTH >> 2;
  crossCoordinate[1].x = crossCoordinate[0].x;
  crossCoordinate[1].y = crossCoordinate[0].y * 3;
  crossCoordinate[2].x = crossCoordinate[0].x * 3;
  crossCoordinate[2].y = crossCoordinate[1].y;
  crossCoordinate[3].x = crossCoordinate[2].x;
  crossCoordinate[3].y = crossCoordinate[0].y;
  for (i = 0; i < 4; i++) {
    LCD_Clear(0, 0, LCD_X_LENGTH, LCD_Y_LENGTH);
    pStr = "Touch Calibrate ......";
    //插入空格，居中显示
    sprintf(cStr, "%*c%s", ((LCD_X_LENGTH / (((sFONT *) LCD_GetFont())->Width)) - strlen(pStr)) / 2, ' ', pStr);
    LCD_DisplayStringLine_EN(LCD_Y_LENGTH >> 1, cStr);
    //插入空格，居中显示
    sprintf(cStr, "%*c%d", ((LCD_X_LENGTH / (((sFONT *) LCD_GetFont())->Width)) - 1) / 2, ' ', i + 1);
    LCD_DisplayStringLine_EN((LCD_Y_LENGTH >> 1) - (((sFONT *) LCD_GetFont())->Height), cStr);
    XPT2046_DelayUS(300000);    //适当的延时很有必要
    LCD_DrawCross(crossCoordinate[i].x, crossCoordinate[i].y);    //显示校正用的“十”字
    while (!XPT2046_ReadAdc_Smooth_XY(&screenSample[i]));    //读取XPT2046数据到变量pCoordinate，当ptr为空时表示没有触点被按下
  }
  XPT2046_Calculate_CalibrationFactor(crossCoordinate, screenSample, &CalibrationFactor); //用原始参数计算出 原始参数与坐标的转换系数
  if (CalibrationFactor.Divider == 0)
    goto Failure;
  test_x = ((CalibrationFactor.An * screenSample[3].x) + (CalibrationFactor.Bn * screenSample[3].y) + CalibrationFactor.Cn) / CalibrationFactor.Divider;  //取一个点计算X值
  test_y = ((CalibrationFactor.Dn * screenSample[3].x) + (CalibrationFactor.En * screenSample[3].y) + CalibrationFactor.Fn) / CalibrationFactor.Divider;  //取一个点计算Y值
  gap_x = (test_x > crossCoordinate[3].x) ? (test_x - crossCoordinate[3].x) : (crossCoordinate[3].x - test_x); //实际X坐标与计算坐标的绝对差
  gap_y = (test_y > crossCoordinate[3].y) ? (test_y - crossCoordinate[3].y) : (crossCoordinate[3].y - test_y); //实际Y坐标与计算坐标的绝对差
  if ((gap_x > 15) || (gap_y > 15))
    goto Failure;               //可以通过修改这两个值的大小来调整精度
  /* 校准系数为全局变量 */
  strXPT2046_TouchPara[LCD_Mode].dX_X = (CalibrationFactor.An * 1.0) / CalibrationFactor.Divider;
  strXPT2046_TouchPara[LCD_Mode].dX_Y = (CalibrationFactor.Bn * 1.0) / CalibrationFactor.Divider;
  strXPT2046_TouchPara[LCD_Mode].dX = (CalibrationFactor.Cn * 1.0) / CalibrationFactor.Divider;
  strXPT2046_TouchPara[LCD_Mode].dY_X = (CalibrationFactor.Dn * 1.0) / CalibrationFactor.Divider;
  strXPT2046_TouchPara[LCD_Mode].dY_Y = (CalibrationFactor.En * 1.0) / CalibrationFactor.Divider;
  strXPT2046_TouchPara[LCD_Mode].dY = (CalibrationFactor.Fn * 1.0) / CalibrationFactor.Divider;
#if 0                           //输出调试信息，注意要先初始化串口
  {
    float *ulHeadAddres;
    /* 打印校校准系数 */
    XPT2046_INFO("显示模式【%d】校准系数如下：", LCD_Mode);

    ulHeadAddres = (float *) (&strXPT2046_TouchPara[LCD_Mode]);

    for (i = 0; i < 6; i++) {
      printf("%12f,", *ulHeadAddres++);
    }
    printf("\r\n");
  }
#endif
  LCD_Clear(0, 0, LCD_X_LENGTH, LCD_Y_LENGTH);
  LCD_SetFgColor(GREEN);
  pStr = "Calibrate Success";
  //插入空格，居中显示
  sprintf(cStr, "%*c%s", ((LCD_X_LENGTH / (((sFONT *) LCD_GetFont())->Width)) - strlen(pStr)) / 2, ' ', pStr);
  LCD_DisplayStringLine_EN(LCD_Y_LENGTH >> 1, cStr);
  XPT2046_DelayUS(1000000);
  return SUCCESS;
Failure:
  LCD_Clear(0, 0, LCD_X_LENGTH, LCD_Y_LENGTH);
  LCD_SetFgColor(RED);
  pStr = "Calibrate fail";
  //插入空格，居中显示
  sprintf(cStr, "%*c%s", ((LCD_X_LENGTH / (((sFONT *) LCD_GetFont())->Width)) - strlen(pStr)) / 2, ' ', pStr);
  LCD_DisplayStringLine_EN(LCD_Y_LENGTH >> 1, cStr);
  pStr = "try again";
  //插入空格，居中显示
  sprintf(cStr, "%*c%s", ((LCD_X_LENGTH / (((sFONT *) LCD_GetFont())->Width)) - strlen(pStr)) / 2, ' ', pStr);
  LCD_DisplayStringLine_EN((LCD_Y_LENGTH >> 1) + (((sFONT *) LCD_GetFont())->Height), cStr);
  XPT2046_DelayUS(1000000);
  return ERROR;
}
