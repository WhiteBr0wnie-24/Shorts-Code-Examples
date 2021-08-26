#pragma once

class Image
{
    public:
        Image();
        Image(int width, int height);
        void setPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b);
        void getPixel(int x, int y, unsigned char* destination);
        int getWidth();
        int getHeight();
        void setWidth(int width);
        void setHeight(int height);
        void recalculateBuffers();
    private:
        void allocateBuffers();
        unsigned char** red;
        unsigned char** green;
        unsigned char** blue;
        int imageHeight;
        int imageWidth;
};