#include "image.h"

void init_image(FILE* fh) {
  if (fh == NULL) {
    printf("fopen fh error\n");
    exit(-1);
  }
  // move offset to 10 to find rgb raw data offset
  fseek(fh, 10, SEEK_SET);
  fread(&rgb_raw_data_offset, sizeof(unsigned int), 1, fh);
  // move offset to 18    to get width & height;
  fseek(fh, 18, SEEK_SET);
  fread(&width,  sizeof(unsigned int), 1, fh);
  fread(&height, sizeof(unsigned int), 1, fh);
  // get  bit per pixel
  fseek(fh, 28, SEEK_SET);
  fread(&bit_per_pixel, sizeof(unsigned short), 1, fh);
  byte_per_pixel = bit_per_pixel / 8;
  // move offset to rgb_raw_data_offset to get RGB raw data
  fseek(fh, rgb_raw_data_offset, SEEK_SET);

  image_s = (unsigned char *)malloc((size_t)width * height * byte_per_pixel);
  if (image_s == NULL) {
  printf("malloc images_s error\n");
}