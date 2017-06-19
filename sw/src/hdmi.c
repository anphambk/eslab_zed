#include "hdmi.h"

//@
hdmi_t hdmi_open (char* fname) {
  hdmi_t hdmi;

  // Open the file for reading and writing
  hdmi.fbfd = open(fname, O_RDWR);
  if (hdmi.fbfd == -1) {
    perror("Error: cannot open framebuffer device");
    exit(1);
  }
  printf("The framebuffer device was opened successfully.\n");

  // Get fixed screen information
  if (ioctl(hdmi.fbfd, FBIOGET_FSCREENINFO, &hdmi.finfo) == -1) {
    perror("Error reading fixed information");
    exit(2);
  }

  // Get variable screen information
  if (ioctl(hdmi.fbfd, FBIOGET_VSCREENINFO, &hdmi.vinfo) == -1) {
    perror("Error reading variable information");
    exit(3);
  }

  //printf("%dx%d, %dbpp\n", hdmi.vinfo.xres, hdmi.vinfo.yres, hdmi.vinfo.bits_per_pixel);

  // Figure out the size of the screen in bytes
  hdmi.screensize = hdmi.vinfo.xres * hdmi.vinfo.yres * hdmi.vinfo.bits_per_pixel / 8;

  // Map the device to memory
  hdmi.fbp = (char *)mmap(0, hdmi.screensize, PROT_READ | PROT_WRITE, MAP_SHARED, hdmi.fbfd, 0);
  if ((int)hdmi.fbp == -1) {
    perror("Error: failed to map framebuffer device to memory");
    exit(4);
  }
  printf("The framebuffer device was mapped to memory successfully.\n");

  // Offsets
  hdmi.vinfo.xoffset = 0;
  hdmi.vinfo.yoffset = 0;

  return hdmi;
}

//@
void hdmi_info (hdmi_t hdmi) {
  printf("HDMI Info:\n");
  printf("- Screen size: %0d\n", hdmi.screensize);
  printf("- Offsets: x=%0d, y=%0d\n", hdmi.vinfo.xoffset, hdmi.vinfo.yoffset);
  printf("- Resolution: %0dx%0d\n", hdmi.vinfo.xres, hdmi.vinfo.yres);
  printf("- Bits per pixel: %0d\n", hdmi.vinfo.bits_per_pixel);
}

//@
void hdmi_close(hdmi_t hdmi) {
  munmap(hdmi.fbp, hdmi.screensize);
  fclose(hdmi.fbp);
}
