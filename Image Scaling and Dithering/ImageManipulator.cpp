#include "ImageManipulator.hpp"

unsigned char ditheringFilter[4] = { 6, 3, 5, 2 };

// Important: source must be different from result!
// This function destroys the original contents of result
// targetWidth must be less than or equal to source->getWidth()
void ImageManipulator::reduce(Image* source, Image* result, int targetWidth, int targetHeight)
{
    int copiedColumns = 0;
    int copiedLines = 0;

    // Calculate the ration between the target and source image
    // We'll use this value later to determine how many lines/columns we need to skip before
    // copying another line/column to the target buffer
    float horizontalRatio = (float)targetWidth / (float)source->getWidth();
    float verticalRatio = (float)targetHeight / (float)source->getHeight();

    // 'current' values = arbitrary number that tells the program when to copy a pixel to the
    // target image. The program copies a pixel whenever one of these values is at least 1.
    // We start with both values set to 1 so that we keep the first column and line of the
    // source image.
    float horizontalCurrent = 1.0f;
    float verticalCurrent = 1.0f;

    // Set the bitmap data so that other programs can read the resulting *.bmp file
    result->setWidth(targetWidth);
    result->setHeight(targetHeight);
    result->recalculateBuffers();

    // Iterate over all columns of the source image
    for(int x = 0; x < source->getWidth(); x++)
    {
        // If we reached the target width, abort
        if(copiedColumns == targetWidth)
            break;
        
        // Copy the current column to the resulting image if the current value is at least 1
        if(horizontalCurrent >= 1.0f)
        {
            // Iterate over each pixel in the current column (from the top of the image to the bottom)
            for(int y = 0; y < source->getHeight(); y++)
            {
                if(copiedLines == targetHeight)
                    break;
                
                // But make sure to only copy the needed pixels of the current column
                if(verticalCurrent >= 1.0f)
                {
                    unsigned char pixel[3];

                    source->getPixel(x, y, &pixel[0]);
                    result->setPixel(copiedColumns, copiedLines, pixel[0], pixel[1], pixel[2]);

                    copiedLines += 1;
                    verticalCurrent -= 1.0f;
                }

                verticalCurrent += verticalRatio;
            }

            copiedLines = 0;
            copiedColumns += 1;
            horizontalCurrent -= 1.0f;
            verticalCurrent = 1.0f;
        }

        horizontalCurrent += horizontalRatio;
    }
}

/**
 * Only works for grayscale images!
 **/
void ImageManipulator::performFloydSteinbergDithering(Image* source, Image* result)
{
    for(int y = 0; y < source->getHeight(); y++)
    {
        for(int x = 0; x < source->getWidth(); x++)
        {
            if(x - 1 >= 0 && y + 1 < source->getHeight() && x + 1 < source->getWidth())
            {
                unsigned char oldPixel[3];
                unsigned char newPixel = 0xFF;
                unsigned char others[4][3];
                unsigned char otherValues[4];
                char error = 0x00;
                //unsigned float accumulatedValue = 0f;

                source->getPixel(x, y, &oldPixel[0]);
                source->getPixel(x+1, y, &others[0][0]);
                source->getPixel(x-1, y+1, &others[1][0]);
                source->getPixel(x, y+1, &others[2][0]);
                source->getPixel(x+1, y+1, &others[3][0]);

                if(oldPixel[0] < 0x80)
                    newPixel = 0x00;

                result->setPixel(x, y, newPixel, newPixel, newPixel);
                error = oldPixel[0] - newPixel;

                for(int i = 0; i < sizeof(ditheringFilter) / sizeof(unsigned char); i++)
                {
                    otherValues[i] = others[i][0] + error * ditheringFilter[i] * 0.0625;
                }

                result->setPixel(x+1, y,   otherValues[0], otherValues[0], otherValues[0]);
                result->setPixel(x-1, y+1, otherValues[1], otherValues[1], otherValues[1]);
                result->setPixel(x  , y+1, otherValues[2], otherValues[2], otherValues[2]);
                result->setPixel(x+1, y+1, otherValues[3], otherValues[3], otherValues[3]);
            }
        }
    }
}

/**
 * Only works for grayscale images!
 **/
void ImageManipulator::performAverageDithering(Image* source, Image* result)
{
    unsigned char sourcePixel[3];

    for (int y = 0; y < source->getHeight(); y++)
    {
        for (int x = 0; x < source->getWidth(); x++)
        {
            source->getPixel(x, y, &sourcePixel[0]);

            if(sourcePixel[0] <= 0x7F)
                result->setPixel(x, y, 0x00, 0x00, 0x00);
            else
                result->setPixel(x, y, 0xFF, 0xFF, 0xFF);
        }
    }
}

/**
 * Only works for grayscale images!
 **/
void ImageManipulator::performRandomDithering(Image* source, Image* result)
{
    unsigned char sourcePixel[3];

    for (int y = 0; y < source->getHeight(); y++)
    {
        for (int x = 0; x < source->getWidth(); x++)
        {
            source->getPixel(x, y, &sourcePixel[0]);
            unsigned char randomValue = 0 + ( std::rand() % ( 256 - 0 + 1 ) );

            if(randomValue <= sourcePixel[0])
                result->setPixel(x, y, 0xFF, 0xFF, 0xFF);
            else
                result->setPixel(x, y, 0x00, 0x00, 0x00);
        }
    }
}

void ImageManipulator::convertToGrayScale(Image* source, Image* result)
{
    unsigned char sourcePixel[3];

    for (int y = 0; y < source->getHeight(); y++)
    {
        for (int x = 0; x < source->getWidth(); x++)
        {
            source->getPixel(x, y, &sourcePixel[0]);
            unsigned char pixelValue = sourcePixel[0] * 0.3 + sourcePixel[2] * 0.12 + sourcePixel[1] * 0.58;
            result->setPixel(x, y, pixelValue, pixelValue, pixelValue);
        }
    }
}

void ImageManipulator::generateColorTestPattern(Image* result)
{
    for (int i = 0; i < result->getWidth() / 3; i++)
    {
        for(int u = 0; u < 100; u++)
        {
            result->setPixel(i, u, 0xFF, 0x00, 0x00);
        }   
    }

    for (int i = result->getWidth() / 3; i < 2 * (result->getWidth() / 3); i++)
    {
        for(int u = 0; u < 100; u++)
        {
            result->setPixel(i, u, 0x00, 0xFF, 0x00);
        }   
    }

    for (int i = 2 * (result->getWidth() / 3); i < result->getWidth(); i++)
    {
        for(int u = 0; u < 100; u++)
        {
            result->setPixel(i, u, 0x00, 0x00, 0xFF);
        }   
    }

    for (int i = 0; i < result->getWidth(); i++)
    {
        for(int u = 100; u < 125; u++)
        {
            result->setPixel(i, u, 0xFF, 0xFF, 0xFF);
        }   
    }
}