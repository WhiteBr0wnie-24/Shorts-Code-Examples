#pragma once

#include "Image.hpp"
#include <iostream>
#include <unistd.h>
#include <fstream>

class ImageManipulator
{
    public:
        static void performFloydSteinbergDithering(Image* source, Image* result);
        static void performAverageDithering(Image* source, Image* result);
        static void performRandomDithering(Image* source, Image* result);
        static void convertToGrayScale(Image* source, Image* result);
        static void generateColorTestPattern(Image* result);
        static void reduce(Image* source, Image* result, int targetWidth, int targetHeight);
};