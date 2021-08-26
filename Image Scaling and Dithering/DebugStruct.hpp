#pragma once

typedef struct DebugStruct {
    bool displayHelp;
    bool containsFileName;
    bool containsOutFileName;
    const char* fileName;
    const char* outFileName;
    bool convertToGrayScale;
    bool dithering;
    int ditheringFunction;
    bool scaleX;
    bool scaleY;
    int targetWidth;
    int targetHeight;
} DebugStruct;