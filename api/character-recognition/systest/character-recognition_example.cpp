#include <stdlib.h>
#include <baseapi.h>
#include <allheaders.h>

#include "strngs.h"
#include "pixel-data.h"

#if ENABLE_TEMPORARY_IMAGE
#include "opencv2/opencv.hpp"
using namespace cv;
#endif

using namespace tesseract;
using namespace std;

#define SAVE_FILE 0

int main(int argc, char *argv[])
{
    char *outText;
    STRING fileName = argv[1];
    STRING dataPath = argv[2];
    STRING lang = argv[3];

    TessBaseAPI *api = new TessBaseAPI();
    /*
    STRING tessdata_prefix = "TESSDATA_PREFIX";
    char *dataPath = getenv(tessdata_prefix.string());
    */
    // Initialize tesseract-ocr with English, without specifying tessdata path
    if (api->Init(dataPath.string(), lang.string())) {
        fprintf(stderr, "Could not initialize tesseract.\n");
        exit(1);
    }

    // Open input image with leptonica library
    PIX *image = pixRead(fileName.string());

#if ENABLE_TEMPORARY_IMAGE
    //    Pix *gray_img = pixConvertRGBToLuminance(image);
    int w = image->w;
    int h = image->h;
    unsigned char *rgb = (unsigned char*) malloc(h*w*3*sizeof(unsigned char));
    imageFormatConvert_pix2RGB(image, rgb);
    Mat img(h, w, CV_8UC3, rgb);
    imshow("rgb", img);
    waitKey(0);
    free(rgb);
#endif

    api->SetImage(image);
    // Get OCR result
    outText = api->GetUTF8Text();
    printf("OCR output:\n%s", outText);

    // Destroy used object and release memory
    api->End();
    delete [] outText;
    pixDestroy(&image);

    return 0;
}
