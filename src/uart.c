#include "uart.h"
#include <string.h>
#include <stdio.h>
extern int Delay(int time);
void set_uart()
{
   USART_InitTypeDef USART_InitStruct;
   GPIO_InitTypeDef GPIO_InitStructure;
   
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
   
   /*UART3 <-> Wifi settings*/
   /*PB10 = TX, PB11 = RX */
   /*BaudRate 38400, 8bit word length, Stop bits =1, No parity*/
   GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);
   GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10| GPIO_Pin_11;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
   GPIO_Init(GPIOB, &GPIO_InitStructure);
   
   USART_InitStruct.USART_BaudRate = 38400;
   USART_InitStruct.USART_WordLength = USART_WordLength_8b;
   USART_InitStruct.USART_StopBits = USART_StopBits_1;
   USART_InitStruct.USART_Parity = USART_Parity_No;
   USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
   USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; 
   USART_Init(USART3, &USART_InitStruct);
   USART_Cmd(USART3, ENABLE);
   
   /*UART2 <-> COM settings*/
   /*PD5 = TX, PD6 = RX */
   /*BaudRate 9600, 8bit word length, Stop bits =1, No parity*/
   GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_USART2);
   GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_USART2);
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
   GPIO_Init(GPIOD, &GPIO_InitStructure);
   
   USART_InitStruct.USART_BaudRate = 9600;
   USART_InitStruct.USART_WordLength = USART_WordLength_8b;
   USART_InitStruct.USART_StopBits = USART_StopBits_1;
   USART_InitStruct.USART_Parity = USART_Parity_No;
   USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
   USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; 
   USART_Init(USART2, &USART_InitStruct);
   USART_Cmd(USART2, ENABLE);
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
void USART_AT_String_Send(USART_TypeDef* USARTx, char *str) 
{
   while(*str != '\n')
   {
       while(!(USART_GetFlagStatus(USARTx, USART_FLAG_TXE)));
       USART_SendData(USARTx, *str);
       str++;
   }
   while(!(USART_GetFlagStatus(USARTx, USART_FLAG_TXE)));
       USART_SendData(USARTx, '\r');
   while(!(USART_GetFlagStatus(USARTx, USART_FLAG_TXE)));
       USART_SendData(USARTx, '\n');
}
void motionSend(uint8_t value)
{
   char str[31];
   str[0] = 'A';
   str[1] = 'T';
   str[2] = '+';
   str[3] = 'S';
   str[4] = 'E';
   str[5] = 'N';
   str[6] = 'D';
   str[7] = 'B';
   str[8] = '=';
   str[9] = '1';
   str[10] = '0';
   str[11] = ':';
   str[12] = '0';
   str[13] = '0';
   str[14] = '0';
   str[15] = '0';
   str[16] = '0';
   str[17] = '0';
   str[18] = '0';
   str[19] = '0';
   str[20] = '0';
   str[21] = '0';
   str[22] = '0';
   str[23] = '6';
   str[24] = '1';
   str[25] = '2';
   str[26] = '0';
   str[27] = '0';
   str[28] = '0';
   if(value == 1)
      str[29] = '1';
   else
      str[29] = '0';
   str[30] = '\n';
   USART_AT_String_Send(USART3, str);
}
void gassend(uint8_t value)
{
   char str[31];
   str[0] = 'A';
   str[1] = 'T';
   str[2] = '+';
   str[3] = 'S';
   str[4] = 'E';
   str[5] = 'N';
   str[6] = 'D';
   str[7] = 'B';
   str[8] = '=';
   str[9] = '1';
   str[10] = '5';
   str[11] = ':';
   str[12] = '0';
   str[13] = '0';
   str[14] = '0';
   str[15] = '0';
   str[16] = '0';
   str[17] = '0';
   str[18] = '0';
   str[19] = '0';
   str[20] = '0';
   str[21] = '0';
   str[22] = '1';
   str[23] = '0';
   str[24] = '1';
   str[25] = '2';
   str[26] = '0';
   str[27] = '0';
   str[28] = '0';
   if(value == 1)
      str[29] = '1';
   else
      str[29] = '0';
   str[30] = '\n';
   USART_AT_String_Send(USART3, str);
}
void adcSend(uint16_t value)
{
   char str[31];
   int decimal = value;
   int hexadecimal[50];
   uint8_t i,j,k;
   
   memset(str,0,30);
   memset(hexadecimal, 0, 50);
   str[0] = 'A';
   str[1] = 'T';
   str[2] = '+';
   str[3] = 'S';
   str[4] = 'E';
   str[5] = 'N';
   str[6] = 'D';
   str[7] = 'B';
   str[8] = '=';
   str[9] = '2';
  str[10] = '0';
  str[11] = ':';
   str[12] = '0';
   str[13] = '0';
   str[14] = '0';
   str[15] = '0';
   str[16] = '0';
   str[17] = '0';
   str[18] = '0';
   str[19] = '0';
   str[20] = '0';
   str[21] = '0';
   str[22] = '1';
   str[23] = '9';
   str[24] = '1';
   str[25] = '2';
   
   for(i=0; i<4; i++) {
      if(decimal>15) {
         k=decimal%16;
         hexadecimal[i] = k;
      }
      else {
         hexadecimal[i] = decimal;
      }
      decimal = decimal/16;
   }
   for(j=0; j < 4; j++) {
      if(hexadecimal[3-j] >= 10)
         sprintf(str+26+j,"%c",hexadecimal[3-j]+55);
      else
         sprintf(str+26+j,"%d",hexadecimal[3-j]);
   }
   str[30] = '\n';
   USART_AT_String_Send(USART3, str);
}
void LoRa_send(uint8_t motion, uint8_t gas, uint16_t adc)
{
   char str[60];
   
   // for ADC
   int decimal = adc;
   int hexadecimal[50];
   uint8_t i,j,k;
   
   memset(str,0,60);
   
   str[0] = 'A';
   str[1] = 'T';
   str[2] = '+';
   str[3] = 'S';
   str[4] = 'E';
   str[5] = 'N';
   str[6] = 'D';
   str[7] = 'B';
   str[8] = '=';
   str[9] = '2';
   str[10] = '0';
   str[11] = ':';
   str[12] = '0';
   str[13] = '0';
   str[14] = '0';
   str[15] = '0';
   str[16] = '0';
   str[17] = '0';
   str[18] = '0';
   str[19] = '0';
   str[20] = '0';
   str[21] = '0';

   // Motion
   str[22] = '0';
   str[23] = '6';
   str[24] = '1';
   str[25] = '2';
   str[26] = '0';
   str[27] = '0';
   str[28] = '0';
   if(motion == 1)
      str[29] = '1';
   else
      str[29] = '0';

   /*
   // temperature sensor
   str[30] = '0';
   str[31] = '1';
   str[32] = '1';
   str[33] = '2';
   str[34] = '2';
   str[35] = '7';
   str[36] = '1';
   str[37] = '0';
   */


   // Adc
   str[30] = '1';
   str[31] = '9';
   str[32] = '1';
   str[33] = '3';
   for(i=0; i<6; i++) {
      if(decimal>15) {
         k=decimal%16;
         hexadecimal[i] = k;
      }
      else {
         hexadecimal[i] = decimal;
      }
      decimal = decimal/16;
   }
   for(j=0; j < 6; j++) {
      if(hexadecimal[5-j] >= 10)
         sprintf(str+40+j,"%c",hexadecimal[5-j]+55);
      else
         sprintf(str+40+j,"%d",hexadecimal[5-j]);
   }
   str[46] = '\n';
   USART_AT_String_Send(USART3, str);   
}
void LoRa_cv_send(uint32_t cv)
{
   char str[35];
   memset(str,0,35);
	// CV
   str[0] = 'A';
   str[1] = 'T';
   str[2] = '+';
   str[3] = 'S';
   str[4] = 'E';
   str[5] = 'N';
   str[6] = 'D';
   str[7] = 'B';
   str[8] = '=';
   str[9] = '2';
   str[10] = '0';
   str[11] = ':';
   str[12] = '0';
   str[13] = '0';
   str[14] = '0';
   str[15] = '0';
   str[16] = '0';
   str[17] = '0';
   str[18] = '0';
   str[19] = '0';
   str[20] = '0';
   str[21] = '0';
   str[22] = '0';
   str[23] = '1';
   str[24] = '1';
   str[25] = '2';
   str[26] = '1';
   if(cv > camera_threshould)
      str[27] = 'F';
   else
      str[27] = '0';
   str[28]='0';
   str[29]='0';
   str[30] = '\n';
   USART_AT_String_Send(USART3, str);
}