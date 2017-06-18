#ifndef __HDMI_H__
#define __HDMI_H__

#include <fcntl.h>
#include <sys/mman.h>
#include <linux/fb.h>

int fbfd;
struct fb_var_screeninfo vinfo;
struct fb_fix_screeninfo finfo;
long int screensize;
char *fbp;

/* Prototypes */
extern void init_hdmi();

#endif /* __HDMI_H__ */
