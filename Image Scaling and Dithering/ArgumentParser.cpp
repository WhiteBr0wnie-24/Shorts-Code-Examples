#include "ArgumentParser.hpp"

DebugStruct ArgumentParser::parse(int argc, char* argv[])
{
    DebugStruct ret;

    ret.displayHelp = false;
    ret.containsFileName = false;
    ret.containsOutFileName = false;
    ret.scaleX = false;
    ret.scaleY = false;
    ret.convertToGrayScale = false;

    ret.fileName = "example.bmp";
    ret.outFileName = "output.bmp";

    for(int i = 0; i < argc; i++)
    {
        if(strcmp(argv[i], "-?") == 0 || strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "-help") == 0 || strcmp(argv[i], "--help") == 0)
        {
            ret.displayHelp = true;
            break;
        }

        if(strcmp(argv[i], "-in") == 0)
        {
            ret.containsFileName = true;
            ret.fileName = argv[i+1];
            continue;
        }

        if(strcmp(argv[i], "-out") == 0)
        {
            ret.containsOutFileName = true;
            ret.outFileName = argv[i+1];
            continue;
        }

        if(strcmp(argv[i], "-width") == 0)
        {
            ret.scaleX = true;
            ret.targetWidth = std::stoi(argv[i+1]);
            continue;
        }

        if(strcmp(argv[i], "-height") == 0)
        {
            ret.scaleY = true;
            ret.targetHeight = std::stoi(argv[i+1]);
            continue;
        }

        if(strcmp(argv[i], "-gray") == 0)
        {
            ret.convertToGrayScale = true;
            continue;
        }

        if(strcmp(argv[i], "-dithering") == 0)
        {
            ret.dithering = true;
            ret.ditheringFunction = std::stoi(argv[i+1]);
            continue;
        }
    }

    return ret;
}