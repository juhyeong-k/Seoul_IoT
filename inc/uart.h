#include "stm32f4xx.h"
#include "stm32f4xx_usart.h"

void set_uart(void);
void USART_Send(USART_TypeDef* USARTx, char data);
void USART_String_Send(USART_TypeDef* USARTx, char *str);
