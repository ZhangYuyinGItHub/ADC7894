

#include "stm32f10x.h"
#include "uart.h"
#include "stm32f10x_usart.h"
#include "timer.h"
#include "ad7894_driver.h"

unsigned short value = 0;

void delay(int time)
{
		 while(time--);
}

/**
  * 函数名：main
  * 描述  ：主函数
  * 输入  : 无
	* 输出  ：无
  */
	
int main(void)
{ 
	/* config the sysclock to 72m */      
  SystemInit();

  /* USART1 config 115200 8-N-1 */
	USART1_Config();
	NVIC_Configuration();

	/* adc config*/
	/*1. init should be execute once*/
  adc_gpio_init();
	
	for( ; ; )
	{	
		
		  /*2. start a new adc converse*/
  		adc_start();
		  
		  /*3. start a new adc converse*/
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
		  delay(200000);
		  delay(200000);
		  
		
		  /*4. get adc converse state whether finish and success*/
		  if (ADC_STATE_SUCCESS == adc_get_state())
			{
				  /*5. if success, then get the adc new value*/
				  value = adc_read_value();
			}
			
	}
}

/******************* (C) COPYRIGHT 2011 野火嵌入式开发工作室 *****END OF FILE****/
