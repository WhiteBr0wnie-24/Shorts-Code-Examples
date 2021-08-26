#pragma once

#include "Image.hpp"
#include <iostream>
#include <unistd.h>
#include <fstream>

using std::ofstream;
using std::ifstream;

#pragma pack(push, 1)

typedef int LONG;               // 4 Byte
typedef unsigned short WORD;    // 2 Byte
typedef unsigned DWORD;         // 4 Byte

typedef struct tagBITMAPFILEHEADER {
    WORD bfType;
    DWORD bfSize;
    WORD bfReserved1;
    WORD bfReserved2;
    DWORD bfOffBits;
} BITMAPFILEHEADER, *PBITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER {
    DWORD biSize;
    LONG biWidth;
    LONG biHeight;
    WORD biPlanes;
    WORD biBitCount;
    DWORD biCompression;
    DWORD biSizeImage;
    LONG biXPelsPerMeter;
    LONG biYPelsPerMeter;
    DWORD biClrUsed;
    DWORD biClrImportant;
} BITMAPINFOHEADER, *PBITMAPINFOHEADER;

#pragma pack(pop)

class Bitmap
{
    public:
        Bitmap(const char* fileName);
        void save(const char* fileName);
        Image imageData;      // This class only contains the pixel information of the image
        bool loaded;
        void setImageData(Image* image);

    private:
        bool load(const char* imageLocation);
        void extractPixels();
        char* bitmapData;     // This buffer stores the entire bitmap file (headers + pixels) as raw bytes
        int bufferSize;
        DWORD headerOffset;
        PBITMAPFILEHEADER file_header;
        PBITMAPINFOHEADER info_header;
};