#include "OV7670_control.h"
#include <stdint.h>

#define H_min 25
#define H_max 75
#define S_min 25
#define S_max 255
#define V_min 10
#define V_max 255

void bit_shift(uint16_t* bit_origin);
void yellow_filter(uint16_t src[IMG_ROWS*IMG_COLUMNS], uint16_t des[IMG_ROWS*IMG_COLUMNS/16]);
int compare(uint16_t src[IMG_ROWS*IMG_COLUMNS], uint16_t b1[IMG_ROWS*IMG_COLUMNS/16], uint16_t b2[IMG_ROWS*IMG_COLUMNS/16], uint16_t origin[IMG_ROWS*IMG_COLUMNS/16]);
void get_originYellow(uint16_t src[IMG_ROWS*IMG_COLUMNS], uint16_t b1[IMG_ROWS*IMG_COLUMNS/16], uint16_t b2[IMG_ROWS*IMG_COLUMNS/16], uint16_t origin[IMG_ROWS*IMG_COLUMNS/16]);
