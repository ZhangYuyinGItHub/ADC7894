

#include "ad7894_driver.h"
#include "uart.h"
#include "stm32f10x_usart.h"

#define  ADC_CONVST_PIN   GPIO_Pin_15   //PA15, out
#define  ADC_SCLK_PIN     GPIO_Pin_3    //PB3, out
#define  ADC_DATA_PIN     GPIO_Pin_4    //PB4, in
#define  ADC_BUSY_PIN     GPIO_Pin_5    //PB5, in

#define  ADC_CONVST_SET()      GPIO_SetBits(GPIOA, ADC_CONVST_PIN)
#define  ADC_CONVST_RESET()    GPIO_ResetBits(GPIOA, ADC_CONVST_PIN)
#define  ADC_CLK_SET()         GPIO_SetBits(GPIOB, ADC_SCLK_PIN)
#define  ADC_CLK_RESET()       GPIO_ResetBits(GPIOB, ADC_SCLK_PIN)
#define  ADC_GET_DATA_VALUE()  GPIO_ReadInputDataBit(GPIOB, ADC_DATA_PIN)
#define  ADC_GET_BUSY_VALUE()  GPIO_ReadInputDataBit(GPIOB, ADC_BUSY_PIN)

unsigned short adc_value = 0;
static void delay(int time);

adc_state state = ADC_STATE_IDLE;

/* I/O线中断，中断线为PB5 */
void EXTI9_5_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line5) != RESET) //确保是否产生了EXTI Line中断
  {
		 if (state == ADC_STATE_START)
		 {
			 state = ADC_STATE_CONVERTING;
			 
			 adc_value = adc_read_value();
			 
			 if ((adc_value & 0xc000) == 0x00)
			 {
				   state = ADC_STATE_SUCCESS;
			 }
			 else
			 {
				   state = ADC_STATE_FAILED;
			 }
			 
			 uart_send_by_dec(adc_value);

		 }
     EXTI_ClearITPendingBit(EXTI_Line5);     //清除中断标志位
  }
}

static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Configure one bit for preemption priority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  
  /* 配置P[A|B|C|D|E]0为中断源 */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

void adc_gpio_init()
{
	  GPIO_InitTypeDef GPIO_InitStructure;
	  EXTI_InitTypeDef EXTI_InitStructure;
	
		RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA 
	                        | RCC_APB2Periph_GPIOB
                         	| RCC_APB2Periph_AFIO, ENABLE);
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);

		GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); 
	
	//Test Pin Config
	  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1;	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		GPIO_ResetBits(GPIOA, GPIO_Pin_1);
		
  	//CONVST Pin Config
		GPIO_InitStructure.GPIO_Pin =  ADC_CONVST_PIN;	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
	  ADC_CONVST_SET();
	
	  //CLK Pin Config
	  GPIO_InitStructure.GPIO_Pin =  ADC_SCLK_PIN;	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		ADC_CLK_RESET();
	
	  //Data Pin Config
	  GPIO_InitStructure.GPIO_Pin = ADC_DATA_PIN | ADC_BUSY_PIN;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		
		//Busy Pin Config
	  GPIO_InitStructure.GPIO_Pin = ADC_DATA_PIN | ADC_BUSY_PIN;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//GPIO_Mode_IN_FLOATING	
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		
		/* EXTI line(PB5) mode config */
		NVIC_Configuration();
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource5); 
		EXTI_InitStructure.EXTI_Line = EXTI_Line5;
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿中断
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		EXTI_Init(&EXTI_InitStructure); 

}

void adc_start(void)
{
	  //keep inpulse 2us
	  ADC_CONVST_RESET();
	  delay(7);
	  ADC_CONVST_SET();	 

    state = ADC_STATE_START;	
}

void adc_reset(void)
{
	  //keep inpulse 2us
	  ADC_CONVST_RESET();
	  delay(7);
	  ADC_CONVST_SET();	 

    state = ADC_STATE_START;	
}

unsigned short adc_read_value(void)
{
	  unsigned short value = 0;
	  unsigned char index = 0;
	  for (index = 0; index < 16; index ++)
	  {
			  ADC_CLK_SET();
//			  delay(30);
//			  ADC_CLK_RESET();
			  
			  delay(10);
			  ADC_CLK_RESET();
			  value |= ADC_GET_DATA_VALUE() << (15-index);
			  delay(10);
		}
		
		adc_reset();
	  return value;
}

static void delay(int time)
{
	  while(time --);
}

int adc_get_state(void)
{
	  return state;
}
