/* 一个简单的通过指针操作控制寄存器点亮LED灯的例子
 * 控制 PB0 输出低电平即可点亮LED灯
 */
int main(void)
{
  /* 打开 GPIOB 端口的时钟，操作 RCC_APB2ENR 寄存器*/
  *(unsigned int *)0x40021018 |= 0x08;
  
  /* 配置 IO 口为输出，操作 GPIOB_CRL 寄存器 */
  *(unsigned int*)0x40010C00 &= ~0x0F;
  *(unsigned int*)0x40010C00 |= 0x01;
  
  /* 控制 ODR 寄存器，输出低电平点亮LED灯 */
  *(unsigned int*)0x40010C0C &= ~0x01;
}

void SystemInit(void)
{
  
}
