#include <stdlib.h>
#include <baseapi.h>
#include <allheaders.h>

#include "strngs.h"

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
    Pix *image = pixRead(fileName.string());

#if SAVE_FILE
    FILE *fp = fopen("image_data.txt","w");
    if (fp){
      int w = image->w;
      int h = image->h;
      int i ;
      for (i = 0; i < w * h; i++){
	fprintf(fp, "%c ", (unsigned char)image->data[i]);
      }
    }
    fclose(fp);
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
