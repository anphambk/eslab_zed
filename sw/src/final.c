#include "final.h"

//void process(void)
//{
//    // 6. Image processing///////////////////////////////
//	int           i, j;
//  unsigned char y1, y2, u, v;
//  int r1, g1, b1, r2, g2, b2;
//  char *pointer;
//  long int location, location2;
//  long int offset;
//
//	pointer = buffers[0].start;
//
//  for(i = 0; i < IMAGEHEIGHT; i++)
//  {
//  	for(j = 0; j < (IMAGEWIDTH/2); j++)
//  	{
//  		y1 = *( pointer + (i*(IMAGEWIDTH/2)+j)*4);
//  		u  = *( pointer + (i*(IMAGEWIDTH/2)+j)*4 + 1);
//  		y2 = *( pointer + (i*(IMAGEWIDTH/2)+j)*4 + 2);
//  		v  = *( pointer + (i*(IMAGEWIDTH/2)+j)*4 + 3);
//
//		  /*lab3.1_YUYV2RGB*/
//      r1 = y1 + 1.042 * (v - 128);
//      g1 = y1 - 0.344 * (u - 128) - 0.714 * (v - 128);
//      b1 = y1 + 1.772 * (u - 128);
//      r2 = y2 + 1.042 * (v - 128);
//      g2 = y2 - 0.344 * (u - 128) - 0.714 * (v - 128);
//      b2 = y2 + 1.772 * (u - 128);
//
//      r1 = (r1>255)? 255: ((r1<0)? 0: r1);
//      b1 = (b1>255)? 255: ((b1<0)? 0: b1);
//      g1 = (g1>255)? 255: ((g1<0)? 0: g1);
//      r2 = (r2>255)? 255: ((r2<0)? 0: r2);
//      b2 = (b2>255)? 255: ((b2<0)? 0: b2);
//      g2 = (g2>255)? 255: ((g2<0)? 0: g2);
//
//      location = (2*j + vinfo.xoffset - 480) * (vinfo.bits_per_pixel/8)
//               + (  i + vinfo.yoffset      ) * finfo.line_length; //0x001f0031
// 	    *(fbp + location    ) = (unsigned char)b1;
// 	    *(fbp + location + 1) = (unsigned char)g1;
// 	    *(fbp + location + 2) = (unsigned char)r1;
// 	    *(fbp + location + 3) = 0;
// 	    *(fbp + location + 4) = (unsigned char)b2;
// 	    *(fbp + location + 5) = (unsigned char)g2;
// 	    *(fbp + location + 6) = (unsigned char)r2;
// 	    *(fbp + location + 7) = 0;
//      
//      /* CHROMA-KEY */
//      //location2 = (2*j + vinfo.xoffset + 480) * (vinfo.bits_per_pixel/8)
//      //          + (  i + vinfo.yoffset      ) * finfo.line_length; //0x001f0031
//      //*(fbp + location2    ) = *(image_s + byte_per_pixel * (IMAGEWIDTH * i + 2*j) + 1);
//      //*(fbp + location2 + 1) = *(image_s + byte_per_pixel * (IMAGEWIDTH * i + 2*j) + 0);
//      //*(fbp + location2 + 2) = *(image_s + byte_per_pixel * (IMAGEWIDTH * i + 2*j) + 2);
//      //*(fbp + location2 + 3) = 0;
//      //*(fbp + location2 + 4) = *(image_s + byte_per_pixel * (IMAGEWIDTH * i + 2*j) + 5);
//      //*(fbp + location2 + 5) = *(image_s + byte_per_pixel * (IMAGEWIDTH * i + 2*j) + 4);
//      //*(fbp + location2 + 6) = *(image_s + byte_per_pixel * (IMAGEWIDTH * i + 2*j) + 6);
//      //*(fbp + location2 + 7) = 0;
//      /* CHROMA-KEY */
//  	}
//  }
//  /////////////////////////////////////////////////////
//}

void test_image() {
  int x, y;
  long int location;

  printf("Test: %0dx%0d\n", img.width, img.height);
  for (y = 0; y < img.height; y++) {
    for (x = 0; x < img.width; x++) {
      location = (x+hdmi.vinfo.xoffset) * (hdmi.vinfo.bits_per_pixel/8) +
                 (y+hdmi.vinfo.yoffset) * hdmi.finfo.line_length;
      *(hdmi.fbp + location    ) = *(img.data + img.byte_per_pixel * (img.width * y + x) + 1); // blue
      *(hdmi.fbp + location + 1) = 200; // green 
      *(hdmi.fbp + location + 2) = *(img.data + img.byte_per_pixel * (img.width * y + x) + 2); // red
      *(hdmi.fbp + location + 3) = 0;      // No transparency
    }
  }
}

int main(int argc, char* argv[])
{
  img = image_open(argv[1]);
  hdmi = hdmi_open(DEV_HDMI);

  image_info(img);
  hdmi_info(hdmi);

  test_image();
  //while(TRUE) {
	//  if (init_v4l2() == FALSE)
  //    return FALSE;
	//  if (v4l2_grab() == FALSE)
  //    return FALSE;
  //  v4l2_dequeue();
  //  process();
  //  v4l2_enqueue();
  //  stop_streaming();
  //  close_v4l2();
  //}

  image_close(img);
  hdmi_close(hdmi);
  return 0;
}
