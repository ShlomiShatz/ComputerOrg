//Shlomo Shatz 316093202
#ifndef __myfunc1_h
#define __myfunc1_h

#include "myutil.h"
#include "readBMP.h"


void doConvolution(Image *image, int kernelSize, int kernel[kernelSize][kernelSize], int kernelScale);

void doConvolutionFilter(Image *image, int kernelSize, int kernel[kernelSize][kernelSize], int kernelScale);



#endif

