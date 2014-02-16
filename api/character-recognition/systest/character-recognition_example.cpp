#include <stdlib.h>
#include <baseapi.h>
#include <allheaders.h>

#include "strngs.h"
#include "pixel-array.h"

#include "opencv2/opencv.hpp"
using namespace cv;

using namespace tesseract;
using namespace std;

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

#if ENABLE_SHOW_TEMPORARY_IMAGE

    PIX* gray = pixConvertRGBToLuminance(image);
    tkPixelArray *pixelArray = imageFormatConvert_pix2pixelArray(gray);
    int fmt;
    if (IMAGE_FORMAT_RGB == pixelArray->format){
        fmt = CV_8UC3;
    }
    else if (IMAGE_FORMAT_GRAY == pixelArray->format){
        fmt = CV_8UC1;
    }
    Mat img(pixelArray->height, pixelArray->width, fmt, pixelArray->data[0]);
    imwrite("rgb.jpg", img);
    imshow("rgb", img);  
    waitKey(0);
    free(pixelArray->data[0]);
    free(pixelArray);

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
