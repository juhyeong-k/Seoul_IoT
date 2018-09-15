#include "uart.h"

void set_uart()
{
	USART_InitTypeDef USART_InitStruct;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/*UART3 <-> Wifi settings*/
	/*PB10 = TX, PB11 = RX */
	/*BaudRate 9600, 8bit word length, Stop bits =1, No parity*/
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10| GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	USART_InitStruct.USART_BaudRate = 115200;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; 
	USART_Init(USART3, &USART_InitStruct);
	USART_Cmd(USART3, ENABLE);
}
/*Transmit 1byte data */
void USART_Send(USART_TypeDef* USARTx, char data)
{
	while(!(USART_GetFlagStatus(USARTx, USART_FLAG_TXE)));
	USART_SendData(USARTx, data);
}
/*Transmit String data. This fuction needs null character in the data */
void USART_String_Send(USART_TypeDef* USARTx, char *str) 
{
		while(*str != 0)
		{
			while(!(USART_GetFlagStatus(USARTx, USART_FLAG_TXE)));
			USART_SendData(USARTx, *str);
			str++;
		}
}