#ifndef _AD7894_H_
#define _AD7894_H_

#include "stm32f10x.h"
#include "stm32f10x_spi.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"


typedef enum
{
	 ADC_STATE_IDLE,
	 ADC_STATE_START,
	 ADC_STATE_CONVERTING,
	 ADC_STATE_COMPLETE,
	 ADC_STATE_COLLECTING,
}adc_state;

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

void adc_gpio_init(void);
void adc_start(void);
unsigned short adc_read_value(void);

#endif
