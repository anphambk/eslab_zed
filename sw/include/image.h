#ifndef __IMAGE_H__
#define __IMAGE_H__

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

typedef struct {
  FILE* fh;
  unsigned int  width, height;   // image width, image height
  unsigned char *data;
  unsigned int rgb_raw_data_offset; // RGB raw data offset
  unsigned short bit_per_pixel;     // bit per pixel
  unsigned short byte_per_pixel;    // byte per pixel
} image_t;

extern image_t image_open (char* fname);
extern void image_test (image_t* img);

#endif /* __IMAGE_H__ */
