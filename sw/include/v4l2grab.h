/****************************************************/
/*                                                  */
/*                      v4lgrab.h                   */
/*                                                  */
/****************************************************/


#ifndef __V4LGRAB_H__
#define __V4LGRAB_H__

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <linux/types.h>
#include <linux/videodev2.h>

#define  FREE(x)    if((x)){free((x));(x)=NULL;}

typedef unsigned char   BYTE;
typedef unsigned short  WORD;
typedef unsigned long   DWORD;

/**/
#pragma pack(1)

typedef struct tagBITMAPFILEHEADER{
    WORD    bfType;            // the flag of bmp, value is "BM"
    DWORD   bfSize;            // size BMP file ,unit is bytes
    DWORD   bfReserved;        // 0
    DWORD   bfOffBits;         // must be 54

}BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER{
    DWORD   biSize;            // must be 0x28
    DWORD   biWidth;           //
    DWORD   biHeight;          //
    WORD    biPlanes;          // must be 1
    WORD    biBitCount;        //
    DWORD   biCompression;     //
    DWORD   biSizeImage;       //
    DWORD   biXPelsPerMeter;   //
    DWORD   biYPelsPerMeter;   //
    DWORD   biClrUsed;         //
    DWORD   biClrImportant;    //
}BITMAPINFOHEADER;

typedef struct tagRGBQUAD{
    BYTE    rgbBlue;
    BYTE    rgbGreen;
    BYTE    rgbRed;
    BYTE    rgbReserved;
}RGBQUAD;

#define  TRUE	1
#define  FALSE	0

#define FILE_VIDEO 	"/dev/video0"

#define  IMAGEWIDTH    640
#define  IMAGEHEIGHT   480

static  int     fd;
static  struct  v4l2_capability     cap;
        struct  v4l2_fmtdesc        fmtdesc;
        struct  v4l2_format         fmt,fmtack;
        struct  v4l2_streamparm     setfps;
        struct  v4l2_requestbuffers req;
        struct  v4l2_buffer         buf;
        enum    v4l2_buf_type       type;

BITMAPFILEHEADER bf;
BITMAPINFOHEADER bi;

struct buffer
{
	void * start;
	unsigned int length;
} *buffers;

/* Prototypes */
extern int init_v4l2(void); 
extern void v4l2_enqueue(void);
extern void v4l2_dequeue(void);
extern int v4l2_grab(void);
extern int close_v4l2(void);
extern void stop_streaming(void);

#endif //__V4LGRAB_H___
