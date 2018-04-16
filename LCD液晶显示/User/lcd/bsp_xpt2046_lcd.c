#include "lcd/bsp_xpt2046_lcd.h"
#include "lcd/bsp_ili9341_lcd.h"
#include <string.h>

//Ĭ�ϴ�����������ͬ����Ļ���в��죬�����µ��ô���У׼������ȡ
XPT2046_TouchPara strXPT2046_TouchPara[] = {
 -0.006464,   -0.073259,  280.358032,    0.074878,    0.002052,   -6.545977,  //ɨ�跽ʽ0
  0.086314,    0.001891,  -12.836658,   -0.003722,   -0.065799,  254.715714,  //ɨ�跽ʽ1
  0.002782,    0.061522,  -11.595689,    0.083393,    0.005159,  -15.650089,  //ɨ�跽ʽ2
  0.089743,   -0.000289,  -20.612209,   -0.001374,    0.064451,  -16.054003,  //ɨ�跽ʽ3
  0.000767,   -0.068258,  250.891769,   -0.085559,   -0.000195,  334.747650,  //ɨ�跽ʽ4
 -0.084744,    0.000047,  323.163147,   -0.002109,   -0.066371,  260.985809,  //ɨ�跽ʽ5
 -0.001848,    0.066984,  -12.807136,   -0.084858,   -0.000805,  333.395386,  //ɨ�跽ʽ6
 -0.085470,   -0.000876,  334.023163,   -0.003390,    0.064725,   -6.211169,  //ɨ�跽ʽ7
};

__IO uint8_t XPT2046_TouchFlag = 0;

void XPT2046_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  /* ����GPIOʱ�� */
  RCC_APB2PeriphClockCmd(XPT2046_SPI_GPIO_CLK | XPT2046_PENIRQ_GPIO_CLK, ENABLE);
  /* ģ��SPI GPIO��ʼ�� */
  GPIO_InitStructure.GPIO_Pin = XPT2046_SPI_CLK_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(XPT2046_SPI_CLK_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = XPT2046_SPI_CS_PIN;
  GPIO_Init(XPT2046_SPI_CS_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = XPT2046_SPI_MOSI_PIN;
  GPIO_Init(XPT2046_SPI_MOSI_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   // ��������
  GPIO_InitStructure.GPIO_Pin = XPT2046_SPI_MISO_PIN;
  GPIO_Init(XPT2046_SPI_MISO_PORT, &GPIO_InitStructure);

  //�����������ź�ָʾ���ţ���ʹ���ж�
  GPIO_InitStructure.GPIO_Pin = XPT2046_PENIRQ_GPIO_PIN;
  GPIO_Init(XPT2046_PENIRQ_GPIO_PORT, &GPIO_InitStructure);

  /* ����Ƭѡ��ѡ��XPT2046 */
  XPT2046_CS_LOW();
}

/* ���� XPT2046 �ļ�΢�뼶��ʱ���� */
static void XPT2046_DelayUS(__IO uint32_t count)
{
  uint32_t i;
  for (i = 0; i < count; ++i) {
    uint8_t uc = 12;     // ����ֵΪ12����Լ��1΢��
    while ( uc -- );     // ��1΢��
  }
}

/* XPT2046 ��д������ */
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

/* XPT2046 �Ķ�ȡ���� */
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

/* �� XPT2046 ѡ��һ��ģ��ͨ��������ADC������ADC������� */
static uint16_t XPT2046_ReadAdc(uint8_t channel)
{
  XPT2046_WriteCMD(channel);
  return XPT2046_ReadCMD();
}

/* ��ȡ XPT2046 ��Xͨ����Yͨ����ADֵ(12 bit�����ֵ4096) */
static void XPT2046_ReadAdc_XY(int16_t *X, int16_t *Y)
{
  *X = XPT2046_ReadAdc(XPT2046_CHANNEL_X);
  XPT2046_DelayUS(1);
  *Y = XPT2046_ReadAdc(XPT2046_CHANNEL_Y);
}

/* �ڴ��� XPT2046 ��Ļʱ��ȡһ�������ADֵ�����Ը���������˲� */
static ErrorStatus XPT2046_ReadAdc_Smooth_XY(XPT2046_Coordinate *pScreenCoordinate)
{
  uint8_t i;
  int16_t buff[2][10];              //����X��Y���ж�β���
  uint8_t minX, minY, maxX, maxY;   //�洢�����е���Сֵ�����ֵ
  /* ѭ������10�� */
  for (i = 0; i < 10; ++i) {
    //�û����������ʱ��TP_INT_IN�ź�Ϊ��
    if (XPT2046_PENIRQ_Read() != XPT2046_PENIRQ_ActiveLevel) break;
    XPT2046_ReadAdc_XY(&buff[0][i], &buff[1][i]);
  }
  /* ������ʵ��� */
  if (XPT2046_PENIRQ_Read() != XPT2046_PENIRQ_ActiveLevel)
    XPT2046_TouchFlag = 0;      //�жϱ�־��λ
  /* ����ɹ�����10������ */
  if (i ==10) {
    maxX = minX = maxY = minY = 0;
    for (i = 1; i < 10; ++i) {
      if (buff[0][i] < buff[0][minX]) minX = i;
      else if (buff[0][i] > buff[0][maxX]) maxX = i;
      if (buff[1][i] < buff[1][minY]) minY = i;
      else if (buff[1][i] > buff[1][maxY]) maxY = i;
    }
    pScreenCoordinate->x = pScreenCoordinate->y = 0;
    /*ȥ����Сֵ�����ֵ֮����ƽ��ֵ*/
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

/* ��ȡ XPT2046 �����㣨У׼�󣩵����� */
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

/* ���������״̬�� */
static uint8_t XPT2046_TouchDetect(void)
{
  static TouchState touch_state = XPT2046_STATE_RELEASE;
  static uint32_t i;
  uint8_t res = TOUCH_NOT_PRESSED;
  switch (touch_state) {
    case XPT2046_STATE_RELEASE:
      if (XPT2046_PENIRQ_Read() == XPT2046_PENIRQ_ActiveLevel) {
        // ��һ�γ��ִ����ź�
        touch_state = XPT2046_STATE_WAITING;
        res =TOUCH_NOT_PRESSED;
      } else {
        // �޴���
        touch_state = XPT2046_STATE_RELEASE;
        res =TOUCH_NOT_PRESSED;
      }
      break;
    case XPT2046_STATE_WAITING:
      if (XPT2046_PENIRQ_Read() == XPT2046_PENIRQ_ActiveLevel) {
        i++;
        //�ȴ�ʱ�������ֵ����Ϊ����������
        //����ʱ�� = DURIATION_TIME * �����������õ�ʱ����
        //���ڶ�ʱ���е��ã�ÿ10ms����һ�Σ�������ʱ��Ϊ��DURIATION_TIME*10ms
        if (i > DURIATION_TIME) {
          i = 0;
          touch_state = XPT2046_STATE_PRESSED;
          res = TOUCH_PRESSED;
        } else {
          //�ȴ�ʱ���ۼ�
          touch_state = XPT2046_STATE_WAITING;
          res = TOUCH_NOT_PRESSED;
        }
      } else {
        //�ȴ�ʱ��ֵδ�ﵽ��ֵ��Ϊ��Ч��ƽ�����ɶ�������
        i = 0;
        touch_state = XPT2046_STATE_RELEASE;
        res = TOUCH_NOT_PRESSED;
      }
      break;
    case XPT2046_STATE_PRESSED:
      if (XPT2046_PENIRQ_Read() == XPT2046_PENIRQ_ActiveLevel) {
        //������������
        touch_state = XPT2046_STATE_PRESSED;
        res = TOUCH_PRESSED;
      } else {
        //�����ͷ�
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

/* �����������µ�ʱ�����ñ����� */
void XPT2046_TouchDown(XPT2046_Coordinate *touch)
{
  //��Ϊ��ֵ��ʾ֮ǰ�Ѵ����
  if(touch->pre_x == -1 && touch->pre_x == -1)
    return;
  {
    extern void Touch_Trace(XPT2046_Coordinate *touch, uint8_t);
    Touch_Trace(touch, 1);
  }
}

/* �������ͷŵ�ʱ�����ñ����� */
void XPT2046_TouchUp(XPT2046_Coordinate *touch)
{
  //��Ϊ��ֵ��ʾ֮ǰ�Ѵ����
  if(touch->pre_x == -1 && touch->pre_x == -1)
    return;
  {
    extern void Touch_Trace(XPT2046_Coordinate *touch, uint8_t);
    Touch_Trace(touch, 0);
  }
}

/* ��⵽�����ж�ʱ���õĴ�����,ͨ��������tp_down ��tp_up�㱨������ */
void XPT2046_TouchEvenHandler(void)
{
  static XPT2046_Coordinate cinfo = {-1, -1, -1, -1};
  if (XPT2046_TouchDetect() == TOUCH_PRESSED) {
    // ��ȡ��������
    XPT2046_Get_TouchedPoint(&cinfo, strXPT2046_TouchPara);
    // ���������Ϣ������
    XPT2046_DEBUG("x=%d, y=%d", cinfo.x, cinfo.y);
    // ���ô���������ʱ�Ĵ����������ڸú�����д�Լ��Ĵ������´������
    XPT2046_TouchDown(&cinfo);
    /* ���´�����Ϣ�� pre_x, pre_y */
    cinfo.pre_x = cinfo.x;
    cinfo.pre_y = cinfo.y;
  } else {
    // ���ô������ͷ�ʱ�Ĵ����������ڸú�����д�Լ��Ĵ����ͷŴ������
    XPT2046_TouchUp(&cinfo);
    /* �����ͷţ��� x, y ����Ϊ�� */
    cinfo.x = -1;
    cinfo.y = -1; 
    cinfo.pre_x = -1;
    cinfo.pre_y = -1;
  }
}

/* �� LCD ����ʾУ������ʱ��Ҫ��ʮ�� */
static void LCD_DrawCross(uint16_t x, uint16_t y)
{
  LCD_DrawLine(x - 10, y, x + 10, y);
  LCD_DrawLine(x, y - 10, x, y + 10);
}

/* ���� XPT2046 ��������У��ϵ��
 * ��ע�⣺ֻ����LCD�ʹ�����������Ƕȷǳ�Сʱ,�����������湫ʽ��
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

/* XPT2046 ������У׼ */
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
  //����ɨ�跽�򣬺���
  LCD_GramScan(LCD_Mode);
  /* �趨��ʮ���ֽ��������� */
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
    //����ո񣬾�����ʾ
    sprintf(cStr, "%*c%s", ((LCD_X_LENGTH / (((sFONT *) LCD_GetFont())->Width)) - strlen(pStr)) / 2, ' ', pStr);
    LCD_DisplayStringLine_EN(LCD_Y_LENGTH >> 1, cStr);
    //����ո񣬾�����ʾ
    sprintf(cStr, "%*c%d", ((LCD_X_LENGTH / (((sFONT *) LCD_GetFont())->Width)) - 1) / 2, ' ', i + 1);
    LCD_DisplayStringLine_EN((LCD_Y_LENGTH >> 1) - (((sFONT *) LCD_GetFont())->Height), cStr);
    XPT2046_DelayUS(300000);    //�ʵ�����ʱ���б�Ҫ
    LCD_DrawCross(crossCoordinate[i].x, crossCoordinate[i].y);    //��ʾУ���õġ�ʮ����
    while (!XPT2046_ReadAdc_Smooth_XY(&screenSample[i]));    //��ȡXPT2046���ݵ�����pCoordinate����ptrΪ��ʱ��ʾû�д��㱻����
  }
  XPT2046_Calculate_CalibrationFactor(crossCoordinate, screenSample, &CalibrationFactor); //��ԭʼ��������� ԭʼ�����������ת��ϵ��
  if (CalibrationFactor.Divider == 0)
    goto Failure;
  test_x = ((CalibrationFactor.An * screenSample[3].x) + (CalibrationFactor.Bn * screenSample[3].y) + CalibrationFactor.Cn) / CalibrationFactor.Divider;  //ȡһ�������Xֵ
  test_y = ((CalibrationFactor.Dn * screenSample[3].x) + (CalibrationFactor.En * screenSample[3].y) + CalibrationFactor.Fn) / CalibrationFactor.Divider;  //ȡһ�������Yֵ
  gap_x = (test_x > crossCoordinate[3].x) ? (test_x - crossCoordinate[3].x) : (crossCoordinate[3].x - test_x); //ʵ��X�������������ľ��Բ�
  gap_y = (test_y > crossCoordinate[3].y) ? (test_y - crossCoordinate[3].y) : (crossCoordinate[3].y - test_y); //ʵ��Y�������������ľ��Բ�
  if ((gap_x > 15) || (gap_y > 15))
    goto Failure;               //����ͨ���޸�������ֵ�Ĵ�С����������
  /* У׼ϵ��Ϊȫ�ֱ��� */
  strXPT2046_TouchPara[LCD_Mode].dX_X = (CalibrationFactor.An * 1.0) / CalibrationFactor.Divider;
  strXPT2046_TouchPara[LCD_Mode].dX_Y = (CalibrationFactor.Bn * 1.0) / CalibrationFactor.Divider;
  strXPT2046_TouchPara[LCD_Mode].dX = (CalibrationFactor.Cn * 1.0) / CalibrationFactor.Divider;
  strXPT2046_TouchPara[LCD_Mode].dY_X = (CalibrationFactor.Dn * 1.0) / CalibrationFactor.Divider;
  strXPT2046_TouchPara[LCD_Mode].dY_Y = (CalibrationFactor.En * 1.0) / CalibrationFactor.Divider;
  strXPT2046_TouchPara[LCD_Mode].dY = (CalibrationFactor.Fn * 1.0) / CalibrationFactor.Divider;
#if 0                           //���������Ϣ��ע��Ҫ�ȳ�ʼ������
  {
    float *ulHeadAddres;
    /* ��ӡУУ׼ϵ�� */
    XPT2046_INFO("��ʾģʽ��%d��У׼ϵ�����£�", LCD_Mode);

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
  //����ո񣬾�����ʾ
  sprintf(cStr, "%*c%s", ((LCD_X_LENGTH / (((sFONT *) LCD_GetFont())->Width)) - strlen(pStr)) / 2, ' ', pStr);
  LCD_DisplayStringLine_EN(LCD_Y_LENGTH >> 1, cStr);
  XPT2046_DelayUS(1000000);
  return SUCCESS;
Failure:
  LCD_Clear(0, 0, LCD_X_LENGTH, LCD_Y_LENGTH);
  LCD_SetFgColor(RED);
  pStr = "Calibrate fail";
  //����ո񣬾�����ʾ
  sprintf(cStr, "%*c%s", ((LCD_X_LENGTH / (((sFONT *) LCD_GetFont())->Width)) - strlen(pStr)) / 2, ' ', pStr);
  LCD_DisplayStringLine_EN(LCD_Y_LENGTH >> 1, cStr);
  pStr = "try again";
  //����ո񣬾�����ʾ
  sprintf(cStr, "%*c%s", ((LCD_X_LENGTH / (((sFONT *) LCD_GetFont())->Width)) - strlen(pStr)) / 2, ' ', pStr);
  LCD_DisplayStringLine_EN((LCD_Y_LENGTH >> 1) + (((sFONT *) LCD_GetFont())->Height), cStr);
  XPT2046_DelayUS(1000000);
  return ERROR;
}
