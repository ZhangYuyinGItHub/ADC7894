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
	 ADC_STATE_SUCCESS,
	 ADC_STATE_FAILED,
}adc_state;




void adc_gpio_init(void);
void adc_start(void);
unsigned short adc_read_value(void);
int adc_get_state(void);

#endif
