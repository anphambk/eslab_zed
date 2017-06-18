#ifndef __IMAGE_H__
#define __IMAGE_H__

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

FILE          *fp_s = NULL;    // source file handler
unsigned int  width, height;   // image width, image height
unsigned char *image_s = NULL; // source image array
unsigned int rgb_raw_data_offset; // RGB raw data offset
unsigned short bit_per_pixel;     // bit per pixel
unsigned short byte_per_pixel;    // byte per pixel
int x_max;
int y_max;

/* Prototypes */
extern void init_image(void);

#endif /* __IMAGE_H__ */