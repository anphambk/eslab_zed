#ifndef __FINAL_H__
#define __FINAL_H__

#include "hdmi.h"
#include "v4l2grab.h"

FILE          *fp_s = NULL;    // source file handler
unsigned int  width, height;   // image width, image height
unsigned char *image_s = NULL; // source image array
unsigned int rgb_raw_data_offset; // RGB raw data offset
unsigned short bit_per_pixel;     // bit per pixel
unsigned short byte_per_pixel;    // byte per pixel
int x_max;
int y_max;

#endif /* __FINAL_H__ */
