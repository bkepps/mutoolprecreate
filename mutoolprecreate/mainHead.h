#pragma once
#include <iostream>
#include "FilesystemW.h"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#define PATHSEP "\\"
#else
#define PATHSEP "/"
#endif

int scanhead(FILE* infile, int* image_width, int* image_height);