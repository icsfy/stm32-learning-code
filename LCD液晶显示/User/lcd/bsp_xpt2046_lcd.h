#ifndef __BSP_XPT2046_LCD_H
#define __BSP_XPT2046_LCD_H

#include "stm32f10x.h"
#include <stdio.h>

/* XPT2046 �����������ź�ָʾ���Ŷ���(��ʹ���ж�) */
#define XPT2046_PENIRQ_GPIO_CLK           RCC_APB2Periph_GPIOE   
#define XPT2046_PENIRQ_GPIO_PORT          GPIOE
#define XPT2046_PENIRQ_GPIO_PIN           GPIO_Pin_4

/* �����ź���Ч��ƽ */
#define XPT2046_PENIRQ_ActiveLevel        0
#define XPT2046_PENIRQ_Read()             GPIO_ReadInputDataBit(XPT2046_PENIRQ_GPIO_PORT, XPT2046_PENIRQ_GPIO_PIN)

/* XPT2046 ������ģ��SPI���Ŷ��� */
#define XPT2046_SPI_GPIO_CLK              (RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOD)

#define XPT2046_SPI_CS_PIN                GPIO_Pin_13
#define XPT2046_SPI_CS_PORT               GPIOD

#define XPT2046_SPI_CLK_PIN               GPIO_Pin_0
#define XPT2046_SPI_CLK_PORT              GPIOE

#define XPT2046_SPI_MOSI_PIN              GPIO_Pin_2
#define XPT2046_SPI_MOSI_PORT             GPIOE

#define XPT2046_SPI_MISO_PIN              GPIO_Pin_3
#define XPT2046_SPI_MISO_PORT             GPIOE

#define XPT2046_CS_HIGH()                 GPIO_SetBits(XPT2046_SPI_CS_PORT, XPT2046_SPI_CS_PIN)    
#define XPT2046_CS_LOW()                  GPIO_ResetBits(XPT2046_SPI_CS_PORT, XPT2046_SPI_CS_PIN)  

#define XPT2046_CLK_HIGH()                GPIO_SetBits(XPT2046_SPI_CLK_PORT, XPT2046_SPI_CLK_PIN)    
#define XPT2046_CLK_LOW()                 GPIO_ResetBits(XPT2046_SPI_CLK_PORT, XPT2046_SPI_CLK_PIN) 

#define XPT2046_MOSI_1()                  GPIO_SetBits(XPT2046_SPI_MOSI_PORT, XPT2046_SPI_MOSI_PIN) 
#define XPT2046_MOSI_0()                  GPIO_ResetBits(XPT2046_SPI_MOSI_PORT, XPT2046_SPI_MOSI_PIN)

#define XPT2046_MISO()                    GPIO_ReadInputDataBit(XPT2046_SPI_MISO_PORT, XPT2046_SPI_MISO_PIN)

#define XPT2046_CHANNEL_X                 0x90    // ͨ��Y+��ѡ�������  
#define XPT2046_CHANNEL_Y                 0xd0    // ͨ��X+��ѡ�������

/* ��Ϣ��� */
#define XPT2046_DEBUG_ON                  0

#define XPT2046_INFO(fmt,arg...)          printf("<<-XPT2046-INFO->> "fmt"\n",##arg)
#define XPT2046_ERROR(fmt,arg...)         printf("<<-XPT2046-ERROR->> "fmt"\n",##arg)
#define XPT2046_DEBUG(fmt,arg...)         do { \
                                            if(XPT2046_DEBUG_ON) \
                                              printf("<<-XPT2046-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg); \
                                          }while(0)

/* ���� XPT2046 ��ص��������� */
typedef struct {        //Һ������ṹ�� 
  /* ����ֵ��ʾ�������� */
  int16_t x, y;         //��¼���µĴ�������ֵ
  /* ���ڼ�¼��������ʱ(����)����һ�δ���λ�� */
  int16_t pre_x, pre_y;
} XPT2046_Coordinate;

typedef struct {        //У׼ϵ���ṹ�壨����ʹ�ã�
  float dX_X, dX_Y, dX, dY_X, dY_Y, dY;
} XPT2046_TouchPara;

typedef struct {        //У׼���ӽṹ��
   float An, Bn, Cn, Dn, En, Fn, Divider;
} XPT2046_Calibration;

/* ����״̬�� */
typedef enum {
  XPT2046_STATE_RELEASE = 0,  //�����ͷ�
  XPT2046_STATE_WAITING,      //��������
  XPT2046_STATE_PRESSED,      //��������
} TouchState;

#define TOUCH_PRESSED       1
#define TOUCH_NOT_PRESSED   0
//����������ֵ
#define DURIATION_TIME      2

void XPT2046_Init(void);
void XPT2046_TouchEvenHandler(void);
ErrorStatus XPT2046_Touch_Calibrate(uint8_t LCD_Mode);
ErrorStatus XPT2046_Get_TouchedPoint(XPT2046_Coordinate *pDisplayCoordinate, XPT2046_TouchPara *pTouchPara);

#endif /* __BSP_XPT2046_LCD_H */
