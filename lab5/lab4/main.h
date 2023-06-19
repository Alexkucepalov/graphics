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



static unsigned short read_u16(FILE* fp)
{
    unsigned char b0, b1;

    b0 = getc(fp);
    b1 = getc(fp);

    return (unsigned short)((b1 << 8) | b0);
}


static unsigned int read_u32(FILE* fp)
{
    unsigned char b0, b1, b2, b3;

    b0 = getc(fp);
    b1 = getc(fp);
    b2 = getc(fp);
    b3 = getc(fp);

    return (unsigned int)((((((b3 << 8) | b2) << 8) | b1) << 8) | b0);
}


static signed long read_s32(FILE* fp)
{
    unsigned char b0, b1, b2, b3;

    b0 = (signed char)getc(fp);
    b1 = (signed char)getc(fp);
    b2 = (signed char)getc(fp);
    b3 = (signed char)getc(fp);

    return (int)((((((b3 << 8) | b2) << 8) | b1) << 8) | b0);
}

#endif // MAIN_H_INCLUDEDs