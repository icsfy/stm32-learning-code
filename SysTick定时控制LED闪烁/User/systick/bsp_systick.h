#ifndef __BSP_SYSTICK_H
#define __BSP_SYSTICK_H

#include "stm32f10x.h"

#define USE_SYSTICK_INT       // ʹ�� SysTick �ж�

#define SYSCLK_FREQ  72000000

//#define SYSTICK_CNT_US        // 1us Ϊ��ʱ��λ
#define SYSTICK_CNT_MS        // 1ms Ϊ��ʱ��λ

extern __IO uint32_t TimingDelay;

void SysTick_Init(void);
#ifdef SYSTICK_CNT_US
void SysTick_Delay_us(uint32_t us);
#endif
void SysTick_Delay_ms(uint32_t ms);

__STATIC_INLINE void TimingDelay_Decrement(void)
{
  if (TimingDelay) {
    TimingDelay--;
  }
}

#endif /* __BSP_SYSTICK_H */
