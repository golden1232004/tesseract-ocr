#include "allheaders.h"
#include <stdio.h>
#include "pixel-data.h"

int imageFormatConvert_pix2RGB(PIX *pix, unsigned char *rgb)
{
#define RED_SHIFT   24
#define GREEN_SHIFT 16
#define BLUE_SHIFT  8

    int w;
    int h;
    int i;
    unsigned int *data = NULL;
    unsigned int value;
    unsigned char *r = rgb;
    unsigned char *g = rgb + 1;
    unsigned char *b = rgb + 2;
    if (NULL == pix || NULL == rgb){
        fprintf(stderr, "Input argument pointer error!\n");
	return -1;
    }
    w = pix->w;
    h = pix->h;
    data = pix->data;
    for(i = 0; i < h * w; i++){
        value = *(data + i);
	*r = (value >> RED_SHIFT) & 0xff;
	*g = (value >> GREEN_SHIFT) & 0xff;
	*b = (value >> BLUE_SHIFT) & 0xff;
	r += 3;
	g += 3;
	b += 3;
    }
    return 1;
 }

