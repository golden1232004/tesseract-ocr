#ifndef __PIXEL_DATA_H__
#define __PIXEL_DATA_H__

#include "allheaders.h"

#define IMAGE_FORMAT_RGB    888
#define IMAGE_FORMAT_GRAY     8
#define IMAGE_FORMAT_NV21    21

typedef struct PixelArray_tag{
    int format;
    int width;
    int height;
    unsigned char* data[4];
    int pitch[4];
}tkPixelArray;

#ifdef __cplusplus
extern "C"{
#endif



tkPixelArray* imageFormatConvert_pix2pixelArray(PIX *pix);
  void imageFormatConvert_destroy(tkPixelArray *pa);

#if !defined(NDEBUG) && defined(ENABLE_SAVE_TEMPORARY_IMAGE)
void debug_imwrite(tkPixelArray pa, const char* fn);
#endif

#ifdef __cplusplus
}
#endif

#endif // __PIXEL_DATA_H__
