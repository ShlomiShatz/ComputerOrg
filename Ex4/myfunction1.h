//Shlomo Shatz 316093202
#ifndef __myfunc1_h
#define __myfunc1_h

#include "myutil.h"
#include "readBMP.h"

void doConvolutionBlur(Image *image);

void doConvolutionFilterBlur(Image *image);

void doConvolutionSharp(Image *image);

void doConvolutionRowBlur(Image *image);

void doConvolutionRowSharp(Image *image);


#endif

