#ifndef __HDMI_H__
#define __HDMI_H__

#include <fcntl.h>
#include <sys/mman.h>
#include <linux/fb.h>

typedef struct {
  int fbfd;
  struct fb_var_screeninfo vinfo;
  struct fb_fix_screeninfo finfo;
  long int screensize;
  char *fbp;
} hdmi_t;

/* Prototypes */
extern hdmi_t hdmi_open (char* fname);
extern void hdmi_close (hdmi_t hdmi);
extern void hdmi_info (hdmi_t hdmi);

#endif /* __HDMI_H__ */
