#ifndef __FINAL_H__
#define __FINAL_H__

#include "hdmi.h"
#include "v4l2grab.h"
#include "image.h"

#define DEV_HDMI "/dev/fb0"
#define DEV_WEBCAM "/dev/video0"

image_t img;
hdmi_t hdmi;

#endif /* __FINAL_H__ */
