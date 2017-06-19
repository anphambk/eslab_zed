#include "final.h"

//@
void process(void)
{
    // 6. Image processing///////////////////////////////
  int i, j;
  unsigned char y1, y2, u, v;
  int r, g, b;
  int r1, g1, b1, r2, g2, b2;
  char *pointer;
  long int location;
  long int offset;

  pointer = buffers[0].start;

  for(i = 0; i < IMAGEHEIGHT; i++)
  {
    for(j = 0; j < (IMAGEWIDTH/2); j++)
    {
      y1 = *( pointer + (i*(IMAGEWIDTH/2)+j)*4);
      u  = *( pointer + (i*(IMAGEWIDTH/2)+j)*4 + 1);
      y2 = *( pointer + (i*(IMAGEWIDTH/2)+j)*4 + 2);
      v  = *( pointer + (i*(IMAGEWIDTH/2)+j)*4 + 3);

      location = (2*j + hdmi.vinfo.xoffset - 480) * (hdmi.vinfo.bits_per_pixel/8)
               + (  i + hdmi.vinfo.yoffset      ) * hdmi.finfo.line_length; //0x001f0031
      r = 1.402 * (v - 128);
      g = 0.344 * (u - 128) + 0.714 * (v - 128);
      b = 1.772 * (u - 128);
      if ((u >  CB_THRESH) && (v < CR_THRESH)) { // BG
        *(hdmi.fbp + location    ) = *(img.data + img.byte_per_pixel * (IMAGEWIDTH * (IMAGEHEIGHT-i) + 2*j) + 0);
        *(hdmi.fbp + location + 1) = *(img.data + img.byte_per_pixel * (IMAGEWIDTH * (IMAGEHEIGHT-i) + 2*j) + 1);
        *(hdmi.fbp + location + 2) = *(img.data + img.byte_per_pixel * (IMAGEWIDTH * (IMAGEHEIGHT-i) + 2*j) + 2);
        *(hdmi.fbp + location + 3) = 0;
        *(hdmi.fbp + location + 4) = *(img.data + img.byte_per_pixel * (IMAGEWIDTH * (IMAGEHEIGHT-i) + 2*j) + 3);
        *(hdmi.fbp + location + 5) = *(img.data + img.byte_per_pixel * (IMAGEWIDTH * (IMAGEHEIGHT-i) + 2*j) + 4);
        *(hdmi.fbp + location + 6) = *(img.data + img.byte_per_pixel * (IMAGEWIDTH * (IMAGEHEIGHT-i) + 2*j) + 5);
        *(hdmi.fbp + location + 7) = 0;
      }
      else { // FG
        r1 = y1 + r;
        g1 = y1 - g;
        b1 = y1 + b;
        r2 = y2 + r;
        g2 = y2 - g;
        b2 = y2 + b;

        r1 = (r1>255)? 255: ((r1<0)? 0: r1);
        b1 = (b1>255)? 255: ((b1<0)? 0: b1);
        g1 = (g1>255)? 255: ((g1<0)? 0: g1);
        r2 = (r2>255)? 255: ((r2<0)? 0: r2);
        b2 = (b2>255)? 255: ((b2<0)? 0: b2);
        g2 = (g2>255)? 255: ((g2<0)? 0: g2);
        
        *(hdmi.fbp + location    ) = (unsigned char)b1;
        *(hdmi.fbp + location + 1) = (unsigned char)g1;
        *(hdmi.fbp + location + 2) = (unsigned char)r1;
        *(hdmi.fbp + location + 3) = 0;
        *(hdmi.fbp + location + 4) = (unsigned char)b2;
        *(hdmi.fbp + location + 5) = (unsigned char)g2;
        *(hdmi.fbp + location + 6) = (unsigned char)r2;
        *(hdmi.fbp + location + 7) = 0;
      }
    }
  }
  /////////////////////////////////////////////////////
}

//@
void test_image () {
  int x = 0, y = 0;
  long int location = 0;
  
  for(y = 0; y != img.height; ++y) {
    for(x = 0; x != img.width; ++x) {
             location = (x+hdmi.vinfo.xoffset) * (hdmi.vinfo.bits_per_pixel/8) +
                 (y+hdmi.vinfo.yoffset) * hdmi.finfo.line_length;

	    *(hdmi.fbp + location)     = *(img.data + img.byte_per_pixel * (img.width * (img.height-y) + x) + 0);        // Some green
	    *(hdmi.fbp + location + 1) = *(img.data + img.byte_per_pixel * (img.width * (img.height-y) + x) + 1);     // A little blue
	    *(hdmi.fbp + location + 2) = *(img.data + img.byte_per_pixel * (img.width * (img.height-y) + x) + 2);    // A lot of red
	    *(hdmi.fbp + location + 3) = 0;      // No transparency
    }
  }

    fclose(img.fh);
    return 0;
}

//@
int main(int argc, char* argv[]) {
  img = image_open(argv[1]);
  hdmi = hdmi_open(DEV_HDMI);

  image_info(img);
  hdmi_info(hdmi);

  //test_image();
  while(TRUE) {
	  if (init_v4l2() == FALSE)
      return FALSE;
	  if (v4l2_grab() == FALSE)
      return FALSE;
    v4l2_dequeue();
    process();
    v4l2_enqueue();
    stop_streaming();
    close_v4l2();
  }

  //image_close(img);
  //hdmi_close(hdmi);
  return 0;
}
