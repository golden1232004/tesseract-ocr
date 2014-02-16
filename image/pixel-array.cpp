#include "allheaders.h"
#include <stdio.h>
#include "pixel-array.h"
/************************************************************************
 *For 24-bit color images, use 32 bpp data, leaving
 *           the fourth byte unused.  Within each 4 byte pixel, the
 *           colors are ordered from MSB to LSB, as follows:
 *
 *                |  MSB  |  2nd MSB  |  3rd MSB  |  LSB  |
 *                   red      green       blue      unused
 *                    0         1           2         3   (big-endian)
 *                    3         2           1         0   (little-endian)
 *
 *           Because we use MSB to LSB ordering within the 32-bit word,
 *           the individual 8-bit samples can be accessed with
 *           GET_DATA_BYTE and SET_DATA_BYTE macros, using the
 *           (implicitly big-ending) ordering
 *                 red:    byte 0  (MSB)
 *                 green:  byte 1  (2nd MSB)
 *                 blue:   byte 2  (3rd MSB)
 ************************************************************************/
tkPixelArray* imageFormatConvert_pix2pixelArray(PIX* pix)
{
#define RED_SHIFT   24
#define GREEN_SHIFT 16
#define BLUE_SHIFT  8
#define SIXTEEN_SHIFT 16
#define EIGHT_SHIFT   8

    int w;
    int h;
    int d;  // depth
    int wpl;
    int i;
    int rgb_w;
    int rgb_h;
    int rgb_ch; // channel number
    int rgb_wpl;
    unsigned int *data = NULL;
    unsigned int value;
    unsigned char *r = NULL;
    unsigned char *g = NULL;
    unsigned char *b = NULL;
    if (NULL == pix){
        fprintf(stderr, "Input argument pointer error!\n");
	return NULL;
    }
    w = pix->w;
    h = pix->h;
    d = pix->d;
    wpl = pix->wpl;
    data = pix->data;
    rgb_wpl = wpl;
    rgb_w = 32 / d * rgb_wpl;
    rgb_h = h;
    
    tkPixelArray *pa = (tkPixelArray *) malloc(sizeof(tkPixelArray));
    if (NULL == pa){
        fprintf(stderr, "malloc tkPixelArray error!\n");
	return NULL;
    }
    if (32 == d){
        rgb_ch = 3; 
	pa->format = IMAGE_FORMAT_RGB;
    }
    else if (8 == d){
        rgb_ch = 1;
	pa->format = IMAGE_FORMAT_GRAY;
    }
    unsigned char *rgb = (unsigned char*) malloc(rgb_w * rgb_h * rgb_ch * sizeof(unsigned char));
    if (NULL == rgb){
        fprintf(stderr, "malloc error!\n");
	return NULL;
    }
    pa->data[0] = rgb;
    pa->width = rgb_w;
    pa->height = rgb_h;
    r = rgb;
    g = rgb + 1;
    b = rgb + 2;
    //ToDo: how to get cpu store mode.
    if (32 == d){
      //little-endian
        for(i = 0; i < h * w; i++){
	    value = *(data + i);
	    *r = (value >> RED_SHIFT) & 0xff;
	    *g = (value >> GREEN_SHIFT) & 0xff;
	    *b = (value >> BLUE_SHIFT) & 0xff;
	    r += 3;
	    g += 3;
	    b += 3;
	}
	pa->data[1] = pa->data[0] + h * w;
	pa->data[2] = pa->data[1] + h * w;
	pa->pitch[0] = pa->width;
	pa->pitch[1] = pa->width;
	pa->pitch[2] = pa->width;
	pa->pitch[3] = 0;
    }
    else if (8 == d){
        unsigned char *a = rgb + 3;
        for (i = 0; i < h; i++){
	    for (int j = 0; j < wpl; j++){
  	        int idx = i * wpl + j;
	        value = *(data + idx);
		unsigned short high16 = (value >> SIXTEEN_SHIFT) & 0xffff;
		unsigned short low16 = value & 0xffff;
		*r = (high16 >> EIGHT_SHIFT) & 0xff;
		*g = high16 & 0xff;
		*b = (low16 >> EIGHT_SHIFT) & 0xff;
		*a = low16 & 0xff;
		r += 4;
		g += 4;
		b += 4;
		a += 4;
	    }
        }
	pa->pitch[0] = pa->width;
	pa->pitch[1] = 0;
	pa->pitch[2] = 0;
	pa->pitch[3] = 0;
    }
    else{
      // TODO
    }
    return pa;
}

void imageFormatConvert_destroy(tkPixelArray *pa)
{
  if (pa){
      if (pa->data[0]){
          free(pa->data[0]);
	  pa->data[0] = NULL;
      }
      free(pa);
      pa = NULL;
  }
}

////////////////////////////////////////////////////////////////////////////

#if !defined(NDEBUG) && defined(ENABLE_SAVE_TEMPORARY_IMAGE)
#include "opencv2/opencv.hpp"
using namespace cv;
void debug_imwrite(tkPixelArray pa, const char* fn)
{
    int format;
    if (pa.format == IMAGE_FORMAT_RGB){
        format = CV_8UC3;
    }
    if (pa.format == IMAGE_FORMAT_GRAY){
        format = CV_8UC1;
    }
    Mat img(pa.height, pa.width, format, pa.data[0], pa.pitch[0]);
    imwrite(fn, img);
}

#endif
