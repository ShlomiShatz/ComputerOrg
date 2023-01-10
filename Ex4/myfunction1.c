//Shlomo Shatz 316093202

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "myfunction1.h"
#include "showBMP.h"

/*
	The code has been optimized by several things that will be explained along the way.
	The first and main one - divided the doConvolution function to several different functions
	each one made for specific situation(different kernels, filters etc.).
	Also, the used functions(smooth etc.) were deleted and embedded to the doConvolution code itself.
*/

//This function is used for the Blur kernel, without filter
void doConvolutionBlur(Image *image) {

	int row, col, locn = n;
	//Allocating maximum needed space, based on the exercise's specs(nxn, 100 <= n <= 1000)
	int nn = 1000000 * sizeof(pixel);
	pixel* pixelsImg = malloc(nn);
	pixel* backupOrg = malloc(nn);

	//Coping the image locally(functions 'pixelsToChars' and 'CopyPixles') using memcpy
	int aloc = 0 - locn;
	for (row = 0 ; row < locn ; row++) {
		aloc += locn;
		int bloc = aloc + aloc + aloc;
		for (col = 0 ; col < locn ; col++) {
			int ploc = aloc + col;
			int iloc = bloc + col + col + col;
			memcpy(pixelsImg + ploc, image->data + iloc, 3 * sizeof(char));
			memcpy(backupOrg + ploc, pixelsImg + ploc, 3* sizeof(char));
		}
	}

	//Locally implemented 'smooth' function
	int i, j;
	int dimReduce = locn - 1;
	int locf = 0;
	for (i = 1 ; i < dimReduce; i++) {
		locf += locn;
		int iimax = i-1, iimin = i+1;
		for (j =  1 ; j < dimReduce ; j++) {
			int loc = locf + j;
			//Locally implemented 'applyKernel' function

			int ii, jj;
			pixel_sum sum;
			//Arbitrary value that is higher than maximum possible intensity, which is 255*3=765
			int min_intensity = 766;
			//Arbitrary value that is lower than minimum possible intensity, which is 0
			int max_intensity = -1;
			int min_row, min_col, max_row, max_col;
			pixel loop_pixel;

			//Zeroing sum using memset
			memset(&sum, 0, sizeof(pixel_sum));

			//Inner loop for applying the kernel and sum the pixels by weight
			int jjmax = j-1, jjmin = j+1;
			for(ii = iimax; ii <= iimin; ii++) {
				int loc2 = ii * locn;
				for(jj = jjmax; jj <= jjmin; jj++) {

					//The function 'sum_pixels_by_weight' implemented here, the kernel is all 1s

					int loc1 = loc2 + jj;
					
					sum.red += ((int)backupOrg[loc1].red);
					sum.green += ((int)backupOrg[loc1].green);
					sum.blue += ((int)backupOrg[loc1].blue);
				}
				
			}

			//Divide by kernel's scale, here 9
			sum.red /= 9;
			sum.green /= 9;
			sum.blue /= 9;
			
			//Truncate each pixel's color values to match the range [0,255]
			int maxsum = 0;
			//Checks sum.red limits
			if (sum.red > 0) {
				if (sum.red <= 255) maxsum = sum.red;
				else maxsum = 255;
			}
			int minsumred = maxsum;
			//Checks sum.green limits
			if (sum.green > 0) {
				if (sum.green <= 255) maxsum = sum.green;
				else maxsum = 255;
			} else maxsum = 0;
			int minsumgreen = maxsum;
			//Checks sum.blue limits
			if (sum.blue > 0) {
				if (sum.blue <= 255) maxsum = sum.blue;
				else maxsum = 255;
			} else maxsum = 0;

			//Copying final value to pixelsImg
			pixelsImg[loc].blue = (unsigned char) maxsum;
			pixelsImg[loc].red = (unsigned char) minsumred;
			pixelsImg[loc].green = (unsigned char) minsumgreen;
		}
	}

	//Implementing the function 'pixelsToChars' using memcpy
	aloc = 0 - locn;
	for (row = 0 ; row < locn ; row++) {
		aloc += locn;
		int bloc = aloc + aloc + aloc;
		for (col = 0 ; col < locn ; col++) {
			int ploc = aloc + col;
			int iloc = bloc + col + col + col;
			memcpy(image->data + iloc, pixelsImg + ploc, 3 * sizeof(char));
		}
	}

	//Freeing allocated memory
	free(pixelsImg);
	free(backupOrg);
}

//This function is used for the Blur kernel, with filter
void doConvolutionFilterBlur(Image *image) {

	int row, col, locn = n;
	//Allocating maximum needed space, based on the exercise's specs(nxn, 100 <= n <= 1000)
	int nn = 1000000 * sizeof(pixel);
	pixel* pixelsImg = malloc(nn);
	pixel* backupOrg = malloc(nn);

	//Coping the image locally(functions 'pixelsToChars' and 'CopyPixles') using memcpy
	int aloc = 0 - locn;
	for (row = 0 ; row < locn ; row++) {
		aloc += locn;
		int bloc = aloc + aloc + aloc;
		for (col = 0 ; col < locn ; col++) {
			int ploc = aloc + col;
			int iloc = bloc + col + col + col;
			memcpy(pixelsImg + ploc, image->data + iloc, 3 * sizeof(char));
			memcpy(backupOrg + ploc, pixelsImg + ploc, 3* sizeof(char));
		}
	}

	//Locally implemented 'smooth' function
	int i, j;
	int dimReduce = locn - 1;
	int locf = 0;
	for (i = 1 ; i < dimReduce; i++) {
		locf += locn;
		int iimax = i-1, iimin = i+1;
		for (j =  1 ; j < dimReduce ; j++) {
			int loc = locf + j;
			//Locally implemented 'applyKernel' function

			int ii, jj;
			pixel_sum sum;
			//Arbitrary value that is higher than maximum possible intensity, which is 255*3=765
			int min_intensity = 766;
			//Arbitrary value that is lower than minimum possible intensity, which is 0
			int max_intensity = -1;
			int min_row, min_col, max_row, max_col;
			pixel loop_pixel;

			//Zeroing sum using memset
			memset(&sum, 0, sizeof(pixel_sum));

			//Inner loop for applying the kernel and sum the pixels by weight

			int jjmax = j-1, jjmin = j+1;
			for(ii = iimax; ii <= iimin; ii++) {
				int loc2 = ii * locn;
				for(jj = jjmax; jj <= jjmin; jj++) {

					//The 'sum_pixels_by_weight' function implemented here, kernel is all 1s

					int loc1 = loc2 + jj;
					
					sum.red += ((int)backupOrg[loc1].red);
					sum.green += ((int)backupOrg[loc1].green);
					sum.blue += ((int)backupOrg[loc1].blue);
					
					//Checking max and min intensities
					int loopcond = (int)(backupOrg[loc1].red + backupOrg[loc1].green + backupOrg[loc1].blue);
					if (loopcond > min_intensity) {
						
					} else {
						min_intensity = loopcond;
						min_row = ii;
						min_col = jj;
					}
					if (loopcond <= max_intensity) {
						
					} else {
						max_intensity = loopcond;
						max_row = ii;
						max_col = jj;
					}
				}
				
			}

			//Filter out min and max
			int loc1 = min_row * locn + min_col;
			int loc2 = max_row * locn + max_col;
		
			//Subtracts the needed locations and pixels
			sum.red -= (int)(backupOrg[loc1].red + backupOrg[loc2].red);
			sum.green -= (int)(backupOrg[loc1].green + backupOrg[loc2].green);
			sum.blue -= (int)(backupOrg[loc1].blue + backupOrg[loc2].blue);
			
			
			//Divide by kernel's scale, 7 here
			sum.red /= 7;
			sum.green /= 7;
			sum.blue /= 7;

			//Truncate each pixel's color values to match the range [0,255]
			int maxsum = 0;
			//Checks sum.red limits
			if (sum.red > 0) {
				if (sum.red <= 255) maxsum = sum.red;
				else maxsum = 255;
			}
			int minsumred = maxsum;
			//Checks sum.green limits
			if (sum.green > 0) {
				if (sum.green <= 255) maxsum = sum.green;
				else maxsum = 255;
			} else maxsum = 0;
			int minsumgreen = maxsum;
			//Checks sum.blue limits
			if (sum.blue > 0) {
				if (sum.blue <= 255) maxsum = sum.blue;
				else maxsum = 255;
			} else maxsum = 0;

			//Copying final value to pixelsImg
			pixelsImg[loc].blue = (unsigned char) maxsum;
			pixelsImg[loc].red = (unsigned char) minsumred;
			pixelsImg[loc].green = (unsigned char) minsumgreen;
		}
	}

	//Implementing the function 'pixelsToChars' using memcpy
	aloc = 0 - locn;
	for (row = 0 ; row < locn ; row++) {
		aloc += locn;
		int bloc = aloc + aloc + aloc;
		for (col = 0 ; col < locn ; col++) {
			int ploc = aloc + col;
			int iloc = bloc + col + col + col;
			memcpy(image->data + iloc, pixelsImg + ploc, 3 * sizeof(char));
		}
	}

	//Freeing allocated memory
	free(pixelsImg);
	free(backupOrg);
}

//This function is used for the Sharp kernel, without filter
void doConvolutionSharp(Image *image) {

int row, col, locn = n;
	//Allocating maximum needed space, based on the exercise's specs(nxn, 100 <= n <= 1000)
	int nn = 1000000 * sizeof(pixel);
	pixel* pixelsImg = malloc(nn);
	pixel* backupOrg = malloc(nn);

	//Coping the image locally(functions 'pixelsToChars' and 'CopyPixles') using memcpy
	int aloc = 0 - locn;
	for (row = 0 ; row < locn ; row++) {
		aloc += locn;
		int bloc = aloc + aloc + aloc;
		for (col = 0 ; col < locn ; col++) {
			int ploc = aloc + col;
			int iloc = bloc + col + col + col;
			memcpy(pixelsImg + ploc, image->data + iloc, 3 * sizeof(char));
			memcpy(backupOrg + ploc, pixelsImg + ploc, 3* sizeof(char));
		}
	}

	//Locally implemented 'smooth' function
	int i, j;
	int dimReduce = locn - 1;
	int locf = 0;
	for (i = 1 ; i < dimReduce; i++) {
		locf += locn;
		int iimax = i-1, iimin = i+1;
		for (j =  1 ; j < dimReduce ; j++) {
			int loc = locf + j;
			//Locally implemented 'applyKernel' function

			int ii, jj;
			pixel_sum sum;
			//Arbitrary value that is higher than maximum possible intensity, which is 255*3=765
			int min_intensity = 766;
			//Arbitrary value that is lower than minimum possible intensity, which is 0
			int max_intensity = -1;
			int min_row, min_col, max_row, max_col;
			pixel loop_pixel;

			//Zeroing sum using memset
			memset(&sum, 0, sizeof(pixel_sum));

			//Inner loop for applying the kernel and sum the pixels by weight

			int jjmax = j-1, jjmin = j+1;
			int loc2 = iimax * locn;

			//Applies the kernel, witout loops - pixel by pixel. kernel is: {-1,-1,-1},{-1,9,-1},{-1,-1,-1}
			int loc1 = loc2 + jjmax;

			//-1
			sum.red -= ((int)backupOrg[loc1].red);
			sum.green -= ((int)backupOrg[loc1].green);
			sum.blue -= ((int)backupOrg[loc1].blue);

			loc1++;
			//-1
			sum.red -= ((int)backupOrg[loc1].red);
			sum.green -= ((int)backupOrg[loc1].green);
			sum.blue -= ((int)backupOrg[loc1].blue);

			loc1++;
			//-1
			sum.red -= ((int)backupOrg[loc1].red);
			sum.green -= ((int)backupOrg[loc1].green);
			sum.blue -= ((int)backupOrg[loc1].blue);

			loc2 += locn;

			loc1 = loc2 + jjmax;
			//-1
			sum.red -= ((int)backupOrg[loc1].red);
			sum.green -= ((int)backupOrg[loc1].green);
			sum.blue -= ((int)backupOrg[loc1].blue);

			loc1++;
			//9
			sum.red += ((int)backupOrg[loc1].red * 9);
			sum.green += ((int)backupOrg[loc1].green * 9);
			sum.blue += ((int)backupOrg[loc1].blue * 9);

			loc1++;
			//-1
			sum.red -= ((int)backupOrg[loc1].red);
			sum.green -= ((int)backupOrg[loc1].green);
			sum.blue -= ((int)backupOrg[loc1].blue);

			loc2 += locn;

			loc1 = loc2 + jjmax;
			//-1
			sum.red -= ((int)backupOrg[loc1].red);
			sum.green -= ((int)backupOrg[loc1].green);
			sum.blue -= ((int)backupOrg[loc1].blue);

			loc1++;
			//-1
			sum.red -= ((int)backupOrg[loc1].red);
			sum.green -= ((int)backupOrg[loc1].green);
			sum.blue -= ((int)backupOrg[loc1].blue);

			loc1++;
			//-1
			sum.red -= ((int)backupOrg[loc1].red);
			sum.green -= ((int)backupOrg[loc1].green);
			sum.blue -= ((int)backupOrg[loc1].blue);

			//Truncate each pixel's color values to match the range [0,255]
			int maxsum = 0;
			//Checks sum.red limits
			if (sum.red > 0) {
				if (sum.red <= 255) maxsum = sum.red;
				else maxsum = 255;
			}
			int minsumred = maxsum;
			//Checks sum.green limits
			if (sum.green > 0) {
				if (sum.green <= 255) maxsum = sum.green;
				else maxsum = 255;
			} else maxsum = 0;
			int minsumgreen = maxsum;
			//Checks sum.blue limits
			if (sum.blue > 0) {
				if (sum.blue <= 255) maxsum = sum.blue;
				else maxsum = 255;
			} else maxsum = 0;

			//Copying final value to pixelsImg
			pixelsImg[loc].blue = (unsigned char) maxsum;
			pixelsImg[loc].red = (unsigned char) minsumred;
			pixelsImg[loc].green = (unsigned char) minsumgreen;
		}
	}

	//Implementing the function 'pixelsToChars' using memcpy
	aloc = 0 - locn;
	for (row = 0 ; row < locn ; row++) {
		aloc += locn;
		int bloc = aloc + aloc + aloc;
		for (col = 0 ; col < locn ; col++) {
			int ploc = aloc + col;
			int iloc = bloc + col + col + col;
			memcpy(image->data + iloc, pixelsImg + ploc, 3 * sizeof(char));
		}
	}

	//Freeing allocated memory
	free(pixelsImg);
	free(backupOrg);
}

//This function is used for the Row Blur kernel, without filter
void doConvolutionRowBlur(Image *image) {

int row, col, locn = n;
	//Allocating maximum needed space, based on the exercise's specs(nxn, 100 <= n <= 1000)
	int nn = 1000000 * sizeof(pixel);
	pixel* pixelsImg = malloc(nn);
	pixel* backupOrg = malloc(nn);

	//Coping the image locally(functions 'pixelsToChars' and 'CopyPixles') using memcpy
	int aloc = 0 - locn;
	for (row = 0 ; row < locn ; row++) {
		aloc += locn;
		int bloc = aloc + aloc + aloc;
		for (col = 0 ; col < locn ; col++) {
			int ploc = aloc + col;
			int iloc = bloc + col + col + col;
			memcpy(pixelsImg + ploc, image->data + iloc, 3 * sizeof(char));
			memcpy(backupOrg + ploc, pixelsImg + ploc, 3* sizeof(char));
		}
	}

	//Locally implemented 'smooth' function
	int i, j;
	int dimReduce = locn - 1;
	int locf = 0;
	for (i = 1 ; i < dimReduce; i++) {
		locf += locn;
		int iimax = i-1, iimin = i+1;
		for (j =  1 ; j < dimReduce ; j++) {
			int loc = locf + j;
			//Locally implemented 'applyKernel' function

			int ii, jj;
			pixel_sum sum;
			//Arbitrary value that is higher than maximum possible intensity, which is 255*3=765
			int min_intensity = 766;
			//Arbitrary value that is lower than minimum possible intensity, which is 0
			int max_intensity = -1;
			int min_row, min_col, max_row, max_col;
			pixel loop_pixel;

			//Zeroing sum using memset
			memset(&sum, 0, sizeof(pixel_sum));

			//Inner loop for applying the kernel and sum the pixels by weight

			int jjmax = j-1, jjmin = j+1;
			int loc2 = iimax * locn;

			//Applies the kernel, witout loops - pixel by pixel. kernel is: {0, 0, 0}, {1, 2, 1}, {0, 0, 0}
			loc2 += locn;

			//No need to sum 0s
			int loc1 = loc2 + jjmax;
			//1
			sum.red += ((int)backupOrg[loc1].red);
			sum.green += ((int)backupOrg[loc1].green);
			sum.blue += ((int)backupOrg[loc1].blue);

			loc1++;
			//2
			sum.red += ((int)backupOrg[loc1].red * 2);
			sum.green += ((int)backupOrg[loc1].green * 2);
			sum.blue += ((int)backupOrg[loc1].blue * 2);

			loc1++;
			//1
			sum.red += ((int)backupOrg[loc1].red);
			sum.green += ((int)backupOrg[loc1].green);
			sum.blue += ((int)backupOrg[loc1].blue);

			//No need to sum 0s

			//Divides by kernel's scale, 4 here
			sum.red /= 4;
			sum.green /= 4;
			sum.blue /= 4;
			
			//Truncate each pixel's color values to match the range [0,255]
			int maxsum = 0;
			//Checks sum.red limits
			if (sum.red > 0) {
				if (sum.red <= 255) maxsum = sum.red;
				else maxsum = 255;
			}
			int minsumred = maxsum;
			//Checks sum.green limits
			if (sum.green > 0) {
				if (sum.green <= 255) maxsum = sum.green;
				else maxsum = 255;
			} else maxsum = 0;
			int minsumgreen = maxsum;
			//Checks sum.blue limits
			if (sum.blue > 0) {
				if (sum.blue <= 255) maxsum = sum.blue;
				else maxsum = 255;
			} else maxsum = 0;

			//Copying final value to pixelsImg
			pixelsImg[loc].blue = (unsigned char) maxsum;
			pixelsImg[loc].red = (unsigned char) minsumred;
			pixelsImg[loc].green = (unsigned char) minsumgreen;
		}
	}

	//Implementing the function 'pixelsToChars' using memcpy
	aloc = 0 - locn;
	for (row = 0 ; row < locn ; row++) {
		aloc += locn;
		int bloc = aloc + aloc + aloc;
		for (col = 0 ; col < locn ; col++) {
			int ploc = aloc + col;
			int iloc = bloc + col + col + col;
			memcpy(image->data + iloc, pixelsImg + ploc, 3 * sizeof(char));
		}
	}

	//Freeing allocated memory
	free(pixelsImg);
	free(backupOrg);
}

//This function is used for the Row Sharp kernel, without filter
void doConvolutionRowSharp(Image *image) {

int row, col, locn = n;
	//Allocating maximum needed space, based on the exercise's specs(nxn, 100 <= n <= 1000)
	int nn = 1000000 * sizeof(pixel);
	pixel* pixelsImg = malloc(nn);
	pixel* backupOrg = malloc(nn);

	//Coping the image locally(functions 'pixelsToChars' and 'CopyPixles') using memcpy
	int aloc = 0 - locn;
	for (row = 0 ; row < locn ; row++) {
		aloc += locn;
		int bloc = aloc + aloc + aloc;
		for (col = 0 ; col < locn ; col++) {
			int ploc = aloc + col;
			int iloc = bloc + col + col + col;
			memcpy(pixelsImg + ploc, image->data + iloc, 3 * sizeof(char));
			memcpy(backupOrg + ploc, pixelsImg + ploc, 3* sizeof(char));
		}
	}

	//Locally implemented 'smooth' function
	int i, j;
	int dimReduce = locn - 1;
	int locf = 0;
	for (i = 1 ; i < dimReduce; i++) {
		locf += locn;
		int iimax = i-1, iimin = i+1;
		for (j =  1 ; j < dimReduce ; j++) {
			int loc = locf + j;
			//Locally implemented 'applyKernel' function

			int ii, jj;
			pixel_sum sum;
			//Arbitrary value that is higher than maximum possible intensity, which is 255*3=765
			int min_intensity = 766;
			//Arbitrary value that is lower than minimum possible intensity, which is 0
			int max_intensity = -1;
			int min_row, min_col, max_row, max_col;
			pixel loop_pixel;

			//Zeroing sum using memset
			memset(&sum, 0, sizeof(pixel_sum));

			//Inner loop for applying the kernel and sum the pixels by weight

			int jjmax = j-1, jjmin = j+1;
			int loc2 = iimax * locn;

			//Applies the kernel, witout loops - pixel by pixel. kernel is: {0, 0, 0}, {1, 2, 1}, {0, 0, 0}

			//No need to sum 0s
			loc2 += locn;

			int loc1 = loc2 + jjmax;
			//1
			sum.red -= ((int)backupOrg[loc1].red * 2);
			sum.green -= ((int)backupOrg[loc1].green * 2);
			sum.blue -= ((int)backupOrg[loc1].blue * 2);

			loc1++;
			//2
			sum.red += ((int)backupOrg[loc1].red * 6);
			sum.green += ((int)backupOrg[loc1].green * 6);
			sum.blue += ((int)backupOrg[loc1].blue * 6);

			loc1++;
			//1
			sum.red -= ((int)backupOrg[loc1].red * 2);
			sum.green -= ((int)backupOrg[loc1].green * 2);
			sum.blue -= ((int)backupOrg[loc1].blue * 2);

			//Divides by kernel's scale, 2 here
			sum.red /= 2;
			sum.green /= 2;
			sum.blue /= 2;
			
			//Truncate each pixel's color values to match the range [0,255]
			int maxsum = 0;
			//Checks sum.red limits
			if (sum.red > 0) {
				if (sum.red <= 255) maxsum = sum.red;
				else maxsum = 255;
			}
			int minsumred = maxsum;
			//Checks sum.green limits
			if (sum.green > 0) {
				if (sum.green <= 255) maxsum = sum.green;
				else maxsum = 255;
			} else maxsum = 0;
			int minsumgreen = maxsum;
			//Checks sum.blue limits
			if (sum.blue > 0) {
				if (sum.blue <= 255) maxsum = sum.blue;
				else maxsum = 255;
			} else maxsum = 0;

			//Copying final value to pixelsImg
			pixelsImg[loc].blue = (unsigned char) maxsum;
			pixelsImg[loc].red = (unsigned char) minsumred;
			pixelsImg[loc].green = (unsigned char) minsumgreen;
		}
	}

	//Implementing the function 'pixelsToChars' using memcpy
	aloc = 0 - locn;
	for (row = 0 ; row < locn ; row++) {
		aloc += locn;
		int bloc = aloc + aloc + aloc;
		for (col = 0 ; col < locn ; col++) {
			int ploc = aloc + col;
			int iloc = bloc + col + col + col;
			memcpy(image->data + iloc, pixelsImg + ploc, 3 * sizeof(char));
		}
	}

	//Freeing allocated memory
	free(pixelsImg);
	free(backupOrg);
}
