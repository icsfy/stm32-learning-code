/* һ���򵥵�ͨ��ָ��������ƼĴ�������LED�Ƶ�����
 * ���� PB0 ����͵�ƽ���ɵ���LED��
 */
int main(void)
{
  /* �� GPIOB �˿ڵ�ʱ�ӣ����� RCC_APB2ENR �Ĵ���*/
  *(unsigned int *)0x40021018 |= 0x08;
  
  /* ���� IO ��Ϊ��������� GPIOB_CRL �Ĵ��� */
  *(unsigned int*)0x40010C00 &= ~0x0F;
  *(unsigned int*)0x40010C00 |= 0x01;
  
  /* ���� ODR �Ĵ���������͵�ƽ����LED�� */
  *(unsigned int*)0x40010C0C &= ~0x01;
}

void SystemInit(void)
{
  
}
