#include "Bitmap.hpp"

Bitmap::Bitmap(const char* fileName)
{
    this->loaded = this->load(fileName);

    if (this->loaded)
    {
        this->extractPixels();
    }
}

bool Bitmap::load(const char* imageLocation)
{
    std::ifstream file(imageLocation);

    if (file)
    {
        file.seekg(0, std::ios::end);
        std::streampos length = file.tellg();
        file.seekg(0, std::ios::beg);

        this->bitmapData = new char[length];
        file.read(&bitmapData[0], length);

        this->file_header = (PBITMAPFILEHEADER) (&bitmapData[0]);
        this->info_header = (PBITMAPINFOHEADER) (&bitmapData[0] + sizeof(BITMAPFILEHEADER));

        int imageHeight = info_header->biHeight;
        int imageWidth = info_header->biWidth;

        this->bufferSize = file_header->bfSize;
        this->headerOffset = file_header->bfOffBits;

        if(imageHeight < 0)
            imageHeight = -imageHeight;

        if(imageWidth < 0)
            imageWidth = -imageWidth;

        this->imageData = Image(imageWidth, imageHeight);

        return true;
    }
    
    return false;
}

void Bitmap::extractPixels()
{
    int extra = 0;

    for (int y = 0; y < imageData.getHeight(); y++)
    {
        for (int x = 0; x < imageData.getWidth(); x++)
        {
            unsigned char red   = this->bitmapData[this->headerOffset + y * imageData.getWidth() + x + extra + 2];
            unsigned char green = this->bitmapData[this->headerOffset + y * imageData.getWidth() + x + extra + 1];
            unsigned char blue  = this->bitmapData[this->headerOffset + y * imageData.getWidth() + x + extra + 0];

            this->imageData.setPixel(x, y, red, green, blue);

            extra = extra + 3;
        }
    }
}

void Bitmap::save(const char* fileName)
{
    std::ofstream fos(fileName);

    if (!fos)
        return;

    int extra = 0;

    for (int y = 0; y < this->imageData.getHeight(); y++)
    {
        unsigned char pixel[3];

        for (int x = 0; x < this->imageData.getWidth(); x++)
        {
            this->imageData.getPixel(x, y, &pixel[0]);

            this->bitmapData[this->headerOffset + y * imageData.getWidth() + x + extra + 2] = pixel[0];
            this->bitmapData[this->headerOffset + y * imageData.getWidth() + x + extra + 1] = pixel[1];
            this->bitmapData[this->headerOffset + y * imageData.getWidth() + x + extra + 0] = pixel[2];

            extra = extra + 3;
        }
    }

    fos.write(this->bitmapData, bufferSize);
}

void Bitmap::setImageData(Image *image)
{
    this->imageData = *image;

    for(int x = 0; x < image->getWidth(); x++)
    {
        int top = 0;
        int bottom = (image->getHeight() - 1);

        while(top < bottom)
        {
            unsigned char topPixel[3];
            unsigned char bottomPixel[3];

            image->getPixel(x, top, &topPixel[0]);
            image->getPixel(x, bottom, &bottomPixel[0]);

            this->imageData.setPixel(x, top, bottomPixel[0], bottomPixel[1], bottomPixel[2]);
            this->imageData.setPixel(x, bottom, topPixel[0], topPixel[1], topPixel[2]);

            top += 1;
            bottom -= 1;
        }
    }

    // Updating these references automatically also updates the entire bitmap data buffer
    // Therefore, there's no need to write them to the buffer separately.

    this->info_header->biHeight = this->imageData.getHeight();
    this->info_header->biWidth = this->imageData.getWidth();
    this->file_header->bfSize = file_header->bfOffBits + this->imageData.getHeight() * this->imageData.getWidth() * 4;
    this->bufferSize = this->file_header->bfSize;
}