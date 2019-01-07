#include "stm32f10x.h"

#define USART_REC_LEN  			4000

extern u8 USART_RX_BUF[USART_REC_LEN];     //????,??USART_REC_LEN???.

#pragma pack(1)

typedef struct{
	u8      header;
	u16     opcode;
	u8      status;
	u32     len;
	u8*     pData;
	u16     crc;
}Cmd_Sruct;
#pragma pack()

extern Cmd_Sruct gCmdStruct;
extern u16 USART_RX_STA;       //??????
extern void USART1_Config(void);
extern void NVIC_Configuration(void);
