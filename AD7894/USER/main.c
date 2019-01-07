

#include "stm32f10x.h"
#include "led.h"
#include "uart.h"
#include "stm32f10x_usart.h"
#include "timer.h"
#include "ad7894_driver.h"

/**
  * ��������main
  * ����  ��������
  * ����  : ��
	* ���  ����
  */
	
void delay(int time)
{
		 while(time--);
}
	
int main(void)
{ 
	/* config the sysclock to 72m */      
  SystemInit();

	/* wait interrupt */
  /* USART1 config 115200 8-N-1 */
	USART1_Config();
	NVIC_Configuration();

  adc_gpio_init();
	
	for( ; ; )
	{	
		  GPIO_SetBits(GPIOA, GPIO_Pin_1);
		  delay(200000);
		  delay(200000);
			delay(200000);
			delay(200000);
			delay(200000);
			delay(200000);
			delay(200000);
		   delay(200000);
		delay(200000);
			delay(200000);
			delay(200000);
			delay(200000);
		   delay(200000);
		  GPIO_ResetBits(GPIOA, GPIO_Pin_1);
      adc_start();
			delay(200000);
	}
}

/******************* (C) COPYRIGHT 2011 Ұ��Ƕ��ʽ���������� *****END OF FILE****/
