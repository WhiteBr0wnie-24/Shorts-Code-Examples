#include "Image.hpp"

Image::Image()
{
    this->imageHeight = this->imageWidth = 0;
    this->allocateBuffers();
}

Image::Image(int width, int height)
{
    this->imageHeight = height;
    this->imageWidth = width;

    this->allocateBuffers();
}

void Image::allocateBuffers()
{
    this->red = new unsigned char*[this->imageWidth];
    this->green = new unsigned char*[this->imageWidth];
    this->blue = new unsigned char*[this->imageWidth];

    for (int i = 0; i < imageWidth; i++)
    {
        this->red[i] = new unsigned char[this->imageHeight];
        this->green[i] = new unsigned char[this->imageHeight];
        this->blue[i] = new unsigned char[this->imageHeight];
    }

    // buffer[x][y]
}

void Image::setPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b)
{
    if(x < this->imageWidth && y < this->imageHeight)
    {
        this->red[x][y] = r;
        this->green[x][y] = g;
        this->blue[x][y] = b;
    }
}

void Image::getPixel(int x, int y, unsigned char* destination)
{
    if(x < this->imageWidth && y < this->imageHeight)
    {
        destination[0] = this->red[x][y];
        destination[1] = this->green[x][y];
        destination[2] = this->blue[x][y];
    }
}

int Image::getWidth()
{
    return this->imageWidth;
}

int Image::getHeight()
{
    return this->imageHeight;
}

void Image::setWidth(int width)
{
    this->imageWidth = width;
}

void Image::setHeight(int height)
{
    this->imageHeight = height;
}

// The client must call this method after changing the width/height of the image.
void Image::recalculateBuffers()
{
    this->allocateBuffers();
}