#include "OV7670_control.h"
#include "cv.h"
#include <stdint.h>

void bit_shift(uint16_t* bit_origin)
{
	uint16_t bit = *bit_origin;
	if(bit == 0 | bit == 1)
	{
		bit = 0x8000;
		*bit_origin =  bit;
		return;
	}
	bit = (bit >> 1);
	*bit_origin = bit;
}
void yellow_filter(uint16_t src[IMG_ROWS*IMG_COLUMNS], uint16_t des[IMG_ROWS*IMG_COLUMNS/16])
{
	int h,s,v;
	uint16_t b,g,r;
	uint32_t n;
	uint8_t MAX_color; // Blue 0, Green 1, Red 2
	uint8_t min;
	uint16_t bit = 0;
	
	for (n = 0; n < ILI9341_PIXEL; n++) {
		
			bit_shift(&bit);
	
			r = ( src[n] & 0xF800 ) >> 8;
			g = ( src[n] & 0x7E0 ) >> 3;
			b = ( src[n] & 0x1F) << 3;
			
			v = r;
			MAX_color = 2;
			if(g > v) { v = g; MAX_color = 1; }
			if(b > v) { v = b; MAX_color = 0; }
			
			min = r;
			if(g < min) min = g;
			if(b < min) min = b;
			
			if(v==0) s=0;
			else s=255*(float)(v-min)/v;
			switch(MAX_color)
			{
				 case 2 : h = 240 + (float)60 * (r - g) / (v - min); break;
				 case 1 : h = 120 + (float)60 * (b - r) / (v - min); break;
				 case 0 : h =       (float)60 * (g - b) / (v - min); break;
				 default : h = 0;
			}
			if(h<0) h = h+360;
			h = h/2;
			if( ( H_min < h && h < H_max ) && ( S_min < s && s < S_max ) && ( V_min < v && v < 255) )
					des[n/16] |= bit;
			else des[n/16] &= ~bit;
	 }
}
int compare(uint16_t src[IMG_ROWS*IMG_COLUMNS], uint16_t temp_1[IMG_ROWS*IMG_COLUMNS/16], uint16_t origin[IMG_ROWS*IMG_COLUMNS/16])
{
	int h,s,v;
	uint16_t b,g,r;
	uint32_t n,i,value;
	uint8_t MAX_color; // Blue 0, Green 1, Red 2
	uint8_t min;
	uint16_t bit = 0;
	value = 0;
	
	for(n = 0; n < ILI9341_PIXEL/16; n++)
		temp_1[n] = origin[n];
	for(i = 0; i < 10; i++) {
		DCMI_CaptureCmd(ENABLE);
		for (n = 0; n < ILI9341_PIXEL; n++) {
				
			bit_shift(&bit);

			r = ( src[n] & 0xF800 ) >> 8;
			g = ( src[n] & 0x7E0 ) >> 3;
			b = ( src[n] & 0x1F) << 3;
			
			v = r;
			MAX_color = 2;
			if(g > v) { v = g; MAX_color = 1; }
			if(b > v) { v = b; MAX_color = 0; }
			
			min = r;
			if(g < min) min = g;
			if(b < min) min = b;
			
			if(v==0) s=0;
			else s=255*(float)(v-min)/v;
			switch(MAX_color)
			{
				 case 2 : h = 240 + (float)60 * (r - g) / (v - min); break;
				 case 1 : h = 120 + (float)60 * (b - r) / (v - min); break;
				 case 0 : h =       (float)60 * (g - b) / (v - min); break;
				 default : h = 0;
			}
			if(h<0) h = h+360;
			h = h/2;
			if( !( 15 < h && h < 95 ) && ( 25 < s && s < 255 ) && ( 10 < v && v < 255) )
				temp_1[n/16] &= ~bit;
		}
	}
	bit = 0;
	for(n = 0; n < ILI9341_PIXEL/16; n++) {
		bit_shift(&bit);
		if( (origin[n/16] & bit) && !( temp_1[n/16] & bit ) ) value++;
	}
	return value;
}
void get_originYellow(uint16_t src[IMG_ROWS*IMG_COLUMNS], uint16_t b1[IMG_ROWS*IMG_COLUMNS/16], uint16_t b2[IMG_ROWS*IMG_COLUMNS/16], uint16_t origin[IMG_ROWS*IMG_COLUMNS/16])
{
	uint32_t i,j,k;
	uint16_t bit = 0;
	
	DCMI_CaptureCmd(ENABLE);
	yellow_filter(src, b1);
	
	for(i = 0; i < 3; i++) {
		for(j = 0; j < 3; j++) {
			DCMI_CaptureCmd(ENABLE);
			yellow_filter(src, b2);
			
			for(k = 0; k < ILI9341_PIXEL/16; k++) {
				bit_shift(&bit);
				if( !((b1[k/16] & bit) && (b2[k/16] & bit)) )
				  b1[k/16] &= ~bit;
			}
		}
		for(i = 0; i < ILI9341_PIXEL/16; i++)
			origin[i] |= b1[i];
	}
}
