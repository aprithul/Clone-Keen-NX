#include <stdio.h>
#include <stdlib.h>

#ifndef TGA_H
#define TGA_H

#define TGA_NONE		0
#define TGA_INDEXED		1
#define TGA_RGB			2
#define TGA_GREYSCALE	3

typedef struct
{
    unsigned char identsize;           // size of ID field that follows 18 uchar header (usually 0)
    unsigned char colourmaptype;       // type of colour map 0=none, 1=has palette
    unsigned char imagetype;           // type of image 0=none,1=indexed,2=rgb,3=grey,+8=rle packed

    unsigned int colourmapstart;       // first colour map entry in palette
    unsigned int colourmaplength;      // number of colours in palette
    unsigned char colourmapbits;       // number of bits per palette entry 15,16,24,32

    unsigned int xstart;               // image x origin
    unsigned int ystart;               // image y origin
    unsigned int width;                // image width in pixels
    unsigned int height;               // image height in pixels
    unsigned char bpp;	               // image bits per pixel 8,16,24,32
    unsigned char descriptor;          // image descriptor bits (vh flip bits)
    
    // pixel data follows header    
} TGA_HEADER;

#endif
