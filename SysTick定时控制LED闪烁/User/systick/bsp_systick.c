#include "bsp_systick.h"

__IO uint32_t TimingDelay;

void SysTick_Init(void)
{
#ifdef SYSTICK_CNT_US
  if (SysTick_Config(SYSCLK_FREQ / 1000000))
#endif
#ifdef SYSTICK_CNT_MS
  if (SysTick_Config(SYSCLK_FREQ / 1000))
#endif
  {
    // Init failed
    while(1);
  }
}

#ifdef SYSTICK_CNT_US
void SysTick_Delay_us(uint32_t us)
{
#ifndef USE_SYSTICK_INT
  SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
  while (us--) {
    while(!(SysTick->CTRL & (1 << 16)));
  }
  SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
#else
  TimingDelay = us;
  while (TimingDelay) {}
#endif
}
#endif

void SysTick_Delay_ms(uint32_t ms)
{
#ifdef SYSTICK_CNT_US
  ms *= 1000;
#endif
#ifndef USE_SYSTICK_INT
  SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
  while (ms--) {
    while(!(SysTick->CTRL & (1 << 16)));
  }
  SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
#else
  TimingDelay = ms;
  while (TimingDelay) {}
#endif
}
