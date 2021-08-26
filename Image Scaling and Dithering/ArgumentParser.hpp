#pragma once

#include "DebugStruct.hpp"
#include <iostream>
#include <string>
#include <string.h>

class ArgumentParser
{
    public:
        static DebugStruct parse(int argc, char* argv[]);
};