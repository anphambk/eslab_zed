#include "image.h"

//@
image_t image_open (char* fname) {
  image_t img;

  img.fh = fopen(fname, "rb");
  if (img.fh == NULL) {
    printf("fopen fh error\n");
    return;
  }
  // move offset to 10 to find rgb raw data offset
  fseek(img.fh, 10, SEEK_SET);
  fread(&img.rgb_raw_data_offset, sizeof(unsigned int), 1, img.fh);
  // move offset to 18    to get width & height;
  fseek(img.fh, 18, SEEK_SET);
  fread(&img.width,  sizeof(unsigned int), 1, img.fh);
  fread(&img.height, sizeof(unsigned int), 1, img.fh);
  // get  bit per pixel
  fseek(img.fh, 28, SEEK_SET);
  fread(&img.bit_per_pixel, sizeof(unsigned short), 1, img.fh);
  img.byte_per_pixel = img.bit_per_pixel / 8;
  // move offset to rgb_raw_data_offset to get RGB raw data
  fseek(img.fh, img.rgb_raw_data_offset, SEEK_SET);

  img.data = (unsigned char *)malloc((size_t)img.width * img.height * img.byte_per_pixel);
  if (img.data == NULL) {
    printf("malloc images_s error\n");
  }
  fread(img.data, sizeof(unsigned char), (size_t)(long)img.width * img.height * img.byte_per_pixel, img.fh);

  //
  return img;
}

//@
void image_close (image_t img) {
  fclose(img.fh);
}

//@
void image_info (image_t img) {
  printf("IMAGE Info:\n");
  printf("- Size: %0dx%0d\n", img.width, img.height);
  printf("- Byte per pixel: %0d\n", img.byte_per_pixel);
}
