#include <stdint.h>

#define FIREWORK_FRAME_COUNT 4
#define FIREWORK_FRAME_WIDTH 8
#define FIREWORK_FRAME_HEIGHT 8

/* Piskel data for "firework" */

static const uint32_t firework_data1[4][64] = {
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0xff000000, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0xff000000, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000
};
static const uint32_t firework_data2[1][64] = {
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0xff000000, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0xff000000, 0x00000000, 0x00000000, 0xff000000, 0x00000000, 0x00000000, 
0x00000000, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000, 0x00000000, 
0x00000000, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000, 0x00000000, 
0x00000000, 0x00000000, 0xff000000, 0x00000000, 0x00000000, 0xff000000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0xff000000, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000
};
static const uint32_t firework_data3[1][64] = {
0x00000000, 0x00000000, 0xff000000, 0x00000000, 0x00000000, 0xff000000, 0x00000000, 0x00000000, 
0x00000000, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000, 0x00000000, 
0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000, 
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000, 
0x00000000, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000, 0x00000000, 
0x00000000, 0x00000000, 0xff000000, 0x00000000, 0x00000000, 0xff000000, 0x00000000, 0x00000000
};
static const uint32_t firework_data4[1][64] = {
0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000, 
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000
};