#include "stm32f10x.h"
#include "stm32f10x_usart.h"
#include "uart.h"



void USART1_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	
	/* config USART1 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
	
	/* USART1 GPIO config */
	/* Configure USART1 Tx (PA.09) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);    
	/* Configure USART1 Rx (PA.10) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* USART1 mode config */
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//????
	USART_Init(USART1, &USART_InitStructure);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//??????
	
	USART_Cmd(USART1, ENABLE);
}
void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	/* Configure the NVIC Preemption Priority Bits */  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	
	/* Enable the USARTy Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void uart_send_by_dec(short value)
{
			 value = value * 10000 /128/64; //convert to voltage value
			 
			 //send value
			 USART_SendData(USART1, (u8)(value%10000 / 1000) + '0');
			 while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
			 
			 USART_SendData(USART1, (u8)(value % 1000 /100) + '0');
			 while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
			 
			 USART_SendData(USART1, (u8)(value % 100 /10) + '0');
			 while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
			 
			 USART_SendData(USART1, (u8)(value % 10 ) + '0');
			 while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
			 
			 USART_SendData(USART1, 'm');
			 while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
			 
			 USART_SendData(USART1, 'V');
			 while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
			 
			 USART_SendData(USART1, ' ');
			 while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
}
