#include "OV7670_control.h"
#include <stdint.h>

void bit_shift(uint16_t* bit_origin);
void get_yellow_line(uint16_t src[IMG_ROWS*IMG_COLUMNS], uint16_t des[IMG_ROWS*IMG_COLUMNS/16]);
void get_origin_yellow_line(uint16_t src[IMG_ROWS*IMG_COLUMNS], uint16_t temp_1[IMG_ROWS*IMG_COLUMNS/16], uint16_t temp_2[IMG_ROWS*IMG_COLUMNS/16], uint16_t temp_3[IMG_ROWS*IMG_COLUMNS/16], uint16_t des[IMG_ROWS*IMG_COLUMNS/16]);
