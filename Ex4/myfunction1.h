//Shlomo Shatz 316093202
#ifndef __myfunc1_h
#define __myfunc1_h

#include "myutil.h"
#include "readBMP.h"


void doConvolution(Image *image, int kernel[3][3], int kernelScale);

void doConvolutionFilter(Image *image, int kernel[3][3], int kernelScale);



#endif

