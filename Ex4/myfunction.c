//Shlomo Shatz 316093202
#include <stdbool.h> 
#include "myutil.h"
#include "myfunction1.h"
#include "myfunction.h"
#include "writeBMP.h"

/*
	All kernels were implemented inside the called functions
*/
void myfunction(Image *image, char* srcImgpName, char* blurRsltImgName, char* sharpRsltImgName, char* rowBlurRsltImgName, char* rowSharpRsltImgName, char* filteredBlurRsltImgName, char* filteredSharpRsltImgName, char flag) {

	if (flag == '1') {	
		// blur image
		doConvolutionBlur(image);

		// write result image to file
		writeBMP(image, srcImgpName, blurRsltImgName);	

		// sharpen the resulting image
		doConvolutionSharp(image);
		
		// write result image to file
		writeBMP(image, srcImgpName, sharpRsltImgName);	
    } else if (flag == '2') {	
		// blur image with row-blurring kernel
		doConvolutionRowBlur(image);

		// write result image to file
		writeBMP(image, srcImgpName, rowBlurRsltImgName);

		// sharpen the resulting image with row-sharpening kernel
		doConvolutionRowSharp(image);

		// write result image to file
		writeBMP(image, srcImgpName, rowSharpRsltImgName);	
	} else {
		// apply extermum filtered kernel to blur image
		doConvolutionFilterBlur(image);

		// write result image to file
		writeBMP(image, srcImgpName, filteredBlurRsltImgName);

		// sharpen the resulting image
		doConvolutionSharp(image);

		// write result image to file
		writeBMP(image, srcImgpName, filteredSharpRsltImgName);	
	}
}

