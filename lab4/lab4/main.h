#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED


typedef struct
{
    unsigned char   bfType[2];
    unsigned long   bfSize;
    unsigned short  bfReserved1;
    unsigned short  bfReserved2;
    unsigned long   bfOffBits;
} BITMAPFILEHEADER;

typedef struct
{
    unsigned long   biSize;
    long            biWidth;
    long            biHeight;
    unsigned short  biPlanes;
    unsigned short  biBitCount;
    unsigned long   biCompression;
    unsigned long   biSizeImage;
    long            biXPelsPerMeter;
    long            biYPelsPerMeter;
    unsigned long   biClrUsed;
    unsigned long   biClrImportant;
} BITMAPINFOHEADER;

typedef struct
{
    unsigned char  blue;
    unsigned char  green;
    unsigned char  red;
    unsigned char  alpha;
} bgrQUAD;


static unsigned short   read_u16(FILE* fp);
static unsigned int     read_u32(FILE* fp);
static signed long      read_s32(FILE* fp);

#endif // MAIN_H_INCLUDEDs