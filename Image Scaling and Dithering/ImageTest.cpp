#include "Bitmap.hpp"
#include "Image.hpp"
#include "ImageManipulator.hpp"
#include "DebugStruct.hpp"
#include "ArgumentParser.hpp"

int main(int argc, char* argv[])
{
    DebugStruct parameters = ArgumentParser::parse(argc, argv);
    Bitmap originalImage = Bitmap(parameters.fileName);
    Image reduced;

    if(parameters.displayHelp)
    {
        printf("\n");
        printf("---                      Image manipulator debug program                      ---\n\n");
        printf("    This program loads an image and manipulates it in the supplied way. All   \n");
        printf("    arguments are optional. Square brackets denote required parameters for an \n");
        printf("    argument. Parantheses denote the default value if that argument is not    \n");
        printf("    supplied.\n\n");
        printf("    -h or -help or --help       .... Display this information\n");
        printf("    -gray (off)                 .... Convert the input image to grayscale\n");
        printf("    -dithering [1-3] (off)      .... Use monochrome dithering\n");
        printf("    -height [0-x] (x)           .... Set target height. x = original image height\n");
        printf("    -width [0-x] (x)            .... Set target width. x = original image width\n");
        printf("    -out [path] (./result.bmp)  .... Path & name of the output file.\n");
        printf("    -in [path] (./example.bmp)  .... Path & name of the input file.\n\n");
        printf("---                               Usage Examples                              ---\n\n");
        printf("    ./scaler -dithering 3\n");
        printf("    Convert the image to grayscale and use dithering algorithm 3. Note: The   \n");
        printf("    dithering argument automatically also turns on grayscale mode.\n");
        printf("    Algorithm 1 is monochrome threshold, 2 is random dithering, 3 is a\n");
        printf("    modified implementation of the Floyd-Steinberg algorithm.\n\n");
        printf("    ./scaler\n");
        printf("    Outputs the original image without changes.\n\n");
        printf("    ./scaler -width 800 -height 600\n");
        printf("    Only rescales the image. Doesn't change the pallette.\n\n");
        printf("---                  The end. Thank you for using the scaler!                 ---\n\n");
        return 0;
    }

    if(parameters.convertToGrayScale || parameters.dithering)
        ImageManipulator::convertToGrayScale(&originalImage.imageData, &originalImage.imageData);

    int targetWidth = parameters.scaleX ? parameters.targetWidth : originalImage.imageData.getWidth();
    int targetHeight = parameters.scaleY ? parameters.targetHeight : originalImage.imageData.getHeight();

    ImageManipulator::reduce(&originalImage.imageData, &reduced, targetWidth, targetHeight);

    if(parameters.dithering)
    {
        switch (parameters.ditheringFunction)
        {
            case 1:
                ImageManipulator::performAverageDithering(&reduced, &reduced);
                break;

            case 2:
                ImageManipulator::performRandomDithering(&reduced, &reduced);
                break;

            case 3:
                ImageManipulator::performFloydSteinbergDithering(&reduced, &reduced);
                break;
            
            default:
                printf("Invalid dithering function supplied!\n");
                break;
        }
    }

    originalImage.setImageData(&reduced);
    originalImage.save(parameters.outFileName);

    return 0;
}