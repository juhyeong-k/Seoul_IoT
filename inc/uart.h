#include "stm32f4xx.h"
#include "stm32f4xx_usart.h"
#include "cv.h"

void set_uart(void);
void USART_Send(USART_TypeDef* USARTx, char data);
void USART_String_Send(USART_TypeDef* USARTx, char *str);
void USART_AT_String_Send(USART_TypeDef* USARTx, char *str);
void motionSend(uint8_t value);
void gassend(uint8_t value);
void adcSend(uint16_t value);
void LoRa_send(uint8_t motion, uint8_t gas, uint16_t adc, uint32_t cv);