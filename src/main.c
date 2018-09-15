/*
*	==========================================================================
*   main.c	
*   (c) 2014, Petr Machala
*
*   Description:
*   OptRec sensor system main file.
*   Optimized for 32F429IDISCOVERY board.
*
*   This program is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   any later version.
*   
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*  
*   You should have received a copy of the GNU General Public License
*   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*	==========================================================================
*/
#include "stm32f4xx.h"
#include "system_control.h"
#include "OV7670_control.h"
#include "lcd_ili9341.h"
#include "lcd_fonts.h"
#include "lcd_spi.h"
#include "cv.h"
#include "adc.h"
#include "uart.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
static volatile uint8_t STM_mode = 0;
static volatile bool btn_pressed = false;
static volatile bool sett_mode = true;
static volatile bool frame_flag = false;

uint16_t b1[4800];
uint16_t b2[4800];
uint16_t origin[4800];

int main(void){
	bool err;
	int i;
	char camera_value[10];
	// System init
	SystemInit();
	STM_LedInit();
	STM_ButtonInit();
	STM_TimerInit();
	MCO1_init();
	SCCB_init();
	DCMI_DMA_init();
	LCD_ILI9341_Init();
	set_adc();
	set_uart();
	
	memset(origin, 0, 4800);
	
	// LCD init page
  LCD_ILI9341_Rotate(LCD_ILI9341_Orientation_Landscape_2);
  LCD_ILI9341_Fill(ILI9341_COLOR_BLACK);
	
	LCD_ILI9341_Puts(20, 55, "Configuring camera", &LCD_Font_16x26, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK);
	LCD_ILI9341_DrawRectangle(99, 110, 221, 130, ILI9341_COLOR_WHITE);
	
	// OV7670 configuration
	if(0) {
		err = OV7670_init();
		if (err == true){
			LCD_ILI9341_Puts(100, 165, "Failed", &LCD_Font_16x26, ILI9341_COLOR_RED, ILI9341_COLOR_BLACK);
			LCD_ILI9341_Puts(20, 200, "Push reset button", &LCD_Font_16x26, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK);
			while(1){
			}
		}		
		else{
			LCD_ILI9341_Puts(100, 165, "Success", &LCD_Font_16x26, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK);
		}
	}
	
	// LCD welcome page
	LCD_ILI9341_Fill(ILI9341_COLOR_BLACK);
  LCD_ILI9341_Puts(60, 110, "TAKE OUT", &LCD_Font_16x26, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLUE);
	
	// Increse SPI baudrate
	LCD_SPI_BaudRateUp();
	
	// Init
	DCMI_CaptureCmd(ENABLE);
	LCD_ILI9341_Rotate(LCD_ILI9341_Orientation_Landscape_1);
	LCD_ILI9341_DisplayImage((uint16_t*) frame_buffer);
	
  get_originYellow((uint16_t*) frame_buffer, b1, b2, origin);
	while(1){
		// Yellow display
	  DCMI_CaptureCmd(ENABLE);
		yellow_filter((uint16_t*) frame_buffer, b1);
	  LCD_ILI9341_Rotate(LCD_ILI9341_Orientation_Landscape_1);
		LCD_ILI9341_Display_bit_Image(b1);
		
		// Origin display
	  LCD_ILI9341_Rotate(LCD_ILI9341_Orientation_Landscape_1);
		LCD_ILI9341_Display_bit_Image(origin);
		
		// RGB display
	  LCD_ILI9341_Rotate(LCD_ILI9341_Orientation_Landscape_1);
		LCD_ILI9341_DisplayImage((uint16_t*) frame_buffer);
		
		memset(camera_value, 0, 10);
		sprintf(camera_value,"%d",compare((uint16_t*) frame_buffer, b1, b2, origin));
		USART_String_Send(USART2, camera_value);
		USART_String_Send(USART2, "\n\r");
	}
}
void TIM3_IRQHandler(void){
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET){		
		static uint8_t old_state = 0xFF;
		uint8_t new_state = STM_ButtonPressed();
		
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
		
		// Button state
		if (new_state > old_state){
				if (STM_mode == 0){
					TIM_Cmd(TIM4, ENABLE);
				}
				sett_mode = false;
		}
		if (sett_mode == false){
			if (new_state < old_state){
				btn_pressed = true;
				if (STM_mode == 0)
					TIM_Cmd(TIM4, DISABLE);
			}
		}
		old_state = new_state;
	}
}

void TIM4_IRQHandler(void){	
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET){				
		static bool init = false; 
		
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
		
		if (init == true){
			// MODE 3 - SETTINGS
			TIM_Cmd(TIM4, DISABLE);
			
			sett_mode = true;
			STM_mode = 3;
			STM_LedOn(LED_GREEN);
			STM_LedOn(LED_RED);
		}
		else{
			init = true;
		}
	}
}

void DMA2_Stream1_IRQHandler(void){
	// DMA complete
	if(DMA_GetITStatus(DMA2_Stream1,DMA_IT_TCIF1) != RESET){
		DMA_ClearITPendingBit(DMA2_Stream1,DMA_IT_TCIF1);
		
		DMA_Cmd(DMA2_Stream1, ENABLE);
		frame_flag = true;
	}
}
