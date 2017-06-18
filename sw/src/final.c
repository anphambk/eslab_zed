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

 int imshow(const char* fname)
 {
     int x = 0, y = 0;
     long int location = 0;
   
     x = 300; y = 100;       // Where we are going to put the pixel

    ///////////////////////////////////////////////////////////////////////////////////////////

   FILE          *fp_s = NULL;    // source file handler
   unsigned int  width, height;   // image width, image height
   unsigned char *image_s = NULL; // source image array
   unsigned char R, G, B;         // color of R, G, B
   unsigned int RGB=0;

   unsigned char header[54] = {
     0x42,        // identity : B
     0x4d,        // identity : M
     0, 0, 0, 0,  // file size
     0, 0,        // reserved1
     0, 0,        // reserved2
     54, 0, 0, 0, // RGB data offset
     40, 0, 0, 0, // struct BITMAPINFOHEADER size
     0, 0, 0, 0,  // bmp width
     0, 0, 0, 0,  // bmp height
     1, 0,        // planes
     24, 0,       // bit per pixel
     0, 0, 0, 0,  // compression
     0, 0, 0, 0,  // data size
     0, 0, 0, 0,  // h resolution
     0, 0, 0, 0,  // v resolution
     0, 0, 0, 0,  // used colors
     0, 0, 0, 0   // important colors
   };

   unsigned int file_size;           // file size
   unsigned int rgb_raw_data_offset; // RGB raw data offset
   unsigned short bit_per_pixel;     // bit per pixel
   unsigned short byte_per_pixel;    // byte per pixel

   fp_s = fopen(fname, "rb");
   if (fp_s == NULL) {
		printf("fopen fp_s error\n");
		return -1;
   }
   

   // move offset to 10 to find rgb raw data offset
   fseek(fp_s, 10, SEEK_SET);
   fread(&rgb_raw_data_offset, sizeof(unsigned int), 1, fp_s);
   // move offset to 18    to get width & height;
   fseek(fp_s, 18, SEEK_SET);
   fread(&width,  sizeof(unsigned int), 1, fp_s);
   fread(&height, sizeof(unsigned int), 1, fp_s);
   // get  bit per pixel
   fseek(fp_s, 28, SEEK_SET);
   fread(&bit_per_pixel, sizeof(unsigned short), 1, fp_s);
   byte_per_pixel = bit_per_pixel / 8;
   // move offset to rgb_raw_data_offset to get RGB raw data
   fseek(fp_s, rgb_raw_data_offset, SEEK_SET);

   image_s = (unsigned char *)malloc((size_t)width * height * byte_per_pixel);
   if (image_s == NULL) {
     printf("malloc images_s error\n");
     return -1;
   }

   fread(image_s, sizeof(unsigned char), (size_t)(long)width * height * byte_per_pixel, fp_s);
   printf("%d\n",bit_per_pixel);
   // vertical inverse algorithm


   for(y = 0; y != height; ++y) {
     for(x = 0; x != width; ++x) {
                    location = (x+hdmi.vinfo.xoffset) * (hdmi.vinfo.bits_per_pixel/8) +
                        (y+hdmi.vinfo.yoffset) * hdmi.finfo.line_length;

 		 if (hdmi.vinfo.bits_per_pixel == 32) {
//		    if ( x > width/2) {
		         *(hdmi.fbp + location)     = *(image_s + byte_per_pixel * (width * (height-y) + x) + 0);        // Some green
		         *(hdmi.fbp + location + 1) = *(image_s + byte_per_pixel * (width * (height-y) + x) + 1);     // A little blue
		         *(hdmi.fbp + location + 2) = *(image_s + byte_per_pixel * (width * (height-y) + x) + 2);    // A lot of red
		         *(hdmi.fbp + location + 3) = 0;      // No transparency
//		    }

             	} else  { //assume 16bpp
		         int b = 10;
		         int g = (x-100)/6;     // A little green
		         int r = 31-(y-100)/16;    // A lot of red
		         unsigned short int t = r<<11 | g << 5 | b;
		         *((unsigned short int*)(hdmi.fbp + location)) = t;
             	}

     }
   }

     fclose(fp_s);
     return 0;
}

int main(int argc, char* argv[])
{
  /*img = image_open(argv[1]);
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
  */

  hdmi = hdmi_open(DEV_HDMI);
  imshow(argv[1]);
  hdmi_close(hdmi);
  return 0;
}
