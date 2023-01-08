//Shlomo Shatz 316093202

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "myfunction1.h"
#include "showBMP.h"

void doConvolution(Image *image, int kernel[3][3], int kernelScale) {

	int row, col, locm = m, locn = n;

	pixel* pixelsImg = malloc(locm*locn*sizeof(pixel));
	pixel* backupOrg = malloc(locm*locn*sizeof(pixel));

	for (row = 0 ; row < locm ; row++) {
		int aloc = row*locn;
		int bloc = 3*aloc;
		for (col = 0 ; col < locn ; col++) {
			int ploc = aloc + col;
			int iloc = bloc + 3*col;
			memcpy(pixelsImg + ploc, image->data + iloc, 3);
			memcpy(backupOrg + ploc, pixelsImg + ploc, 3);
		}
	}

	// smooth(m, backupOrg, pixelsImg, kernelSize, kernel, kernelScale, filter);
	int i, j;
	int dimReduce = locm - 1;
	for (i = 1 ; i < dimReduce; i++) {
		int locf = i * locm;
		for (j =  1 ; j < dimReduce ; j++) {
			int loc = locf + j;
			//  pixelsImg[loc] = applyKernel(locm, i, j, backupOrg, kernelSize, kernel, kernelScale, filter);
		
			int ii, jj;
			pixel_sum sum;
			int min_intensity = 766; // arbitrary value that is higher than maximum possible intensity, which is 255*3=765
			int max_intensity = -1; // arbitrary value that is lower than minimum possible intensity, which is 0
			int min_row, min_col, max_row, max_col;
			pixel loop_pixel;

			memset(&sum, 0, 3*sizeof(int));

			int kRow = 0;
			int iimax = (i-1 > 0 ? i-1 : 0), iimin = (i+1 < locm-1 ? i+1 : locm-1), jjmax = (j-1 > 0 ? j-1 : 0), jjmin = (j+1 < locm-1 ? j+1 : locm-1);
			for(ii = iimax; ii <= iimin; ii++) {
				int kCol = 0;
				int loc2 = ii * locm;
				for(jj = jjmax; jj <= jjmin; jj++) {
					// apply kernel on pixel at [ii,jj]
					// sum_pixels_by_weight(&sum, src[calcIndex(ii, jj, dim)], kernel[kRow][kCol]);


					int loc1 = loc2 + jj;
					int weight = kernel[kRow][kCol];
					sum.red += (backupOrg[loc1].red & 0xff) * weight;
					sum.green += (backupOrg[loc1].green & 0xff) * weight;
					sum.blue += (backupOrg[loc1].blue & 0xff) * weight;
					kCol = (kCol + 1) % 3;
				}
				kRow = (kRow + 1) % 3;
			}

			// divide by kernel's weight
			sum.red /= kernelScale;
			sum.green /= kernelScale;
			sum.blue /= kernelScale;
			
			// truncate each pixel's color values to match the range [0,255]
			int maxsum = sum.red > 0 ? sum.red : 0;
			int minsumred = maxsum < 255 ? maxsum : 255;
			maxsum = sum.green > 0 ? sum.green : 0;
			int minsumgreen = maxsum < 255 ? maxsum : 255;
			maxsum = sum.blue > 0 ? sum.blue : 0;
			int minsumblue = maxsum < 255 ? maxsum : 255;
			pixelsImg[loc].red = (unsigned char) (minsumred);
			pixelsImg[loc].green = (unsigned char) (minsumgreen);
			pixelsImg[loc].blue = (unsigned char) (minsumblue);
		}
	}

	// pixelsToChars(pixelsImg, image);
	for (row = 0; row < locm ; row++) {
		int aloc = row*locn;
		int bloc = 3*aloc;
		for (col = 0; col < locn ; col++) {
			int ploc = aloc + col;
			int iloc = bloc + 3*col;
			memcpy(image->data + iloc, pixelsImg + ploc, 3);
		}
	}

	free(pixelsImg);
	free(backupOrg);
}

void doConvolutionFilter(Image *image, int kernel[3][3], int kernelScale) {

	int row, col, locm = m, locn = n;

	pixel* pixelsImg = malloc(locm*locn*sizeof(pixel));
	pixel* backupOrg = malloc(locm*locn*sizeof(pixel));

	for (row = 0 ; row < locm ; row++) {
		int aloc = row*locn;
		int bloc = 3*aloc;
		for (col = 0 ; col < locn ; col++) {
			int ploc = aloc + col;
			int iloc = bloc + 3*col;
			memcpy(pixelsImg + ploc, image->data + iloc, 3);
			memcpy(backupOrg + ploc, pixelsImg + ploc, 3);
		}
	}

	// smooth(m, backupOrg, pixelsImg, kernelSize, kernel, kernelScale, filter);
	int i, j;
	int dimReduce = locm - 1;
	for (i = 1 ; i < dimReduce; i++) {
		for (j =  1 ; j < dimReduce ; j++) {
			int loc = i * locm + j;
			//  pixelsImg[loc] = applyKernel(locm, i, j, backupOrg, kernelSize, kernel, kernelScale, filter);
		
			int ii, jj;
			pixel_sum sum;
			int min_intensity = 766; // arbitrary value that is higher than maximum possible intensity, which is 255*3=765
			int max_intensity = -1; // arbitrary value that is lower than minimum possible intensity, which is 0
			int min_row, min_col, max_row, max_col;
			pixel loop_pixel;

			memset(&sum, 0, 3*sizeof(int));

			int kRow = 0;
			int iimax = (i-1 > 0 ? i-1 : 0), iimin = (i+1 < locm-1 ? i+1 : locm-1), jjmax = (j-1 > 0 ? j-1 : 0), jjmin = (j+1 < locm-1 ? j+1 : locm-1);
			for(ii = iimax; ii <= iimin; ii++) {
				int kCol = 0;
				int loc2 = ii * locm;
				for(jj = jjmax; jj <= jjmin; jj++) {
					// apply kernel on pixel at [ii,jj]
					// sum_pixels_by_weight(&sum, src[calcIndex(ii, jj, dim)], kernel[kRow][kCol]);


					int loc1 = loc2 + jj;
					int weight = kernel[kRow][kCol];
					sum.red += (backupOrg[loc1].red & 0xff) * weight;
					sum.green += (backupOrg[loc1].green & 0xff) * weight;
					sum.blue += (backupOrg[loc1].blue & 0xff) * weight;

					int loopcond = (backupOrg[loc1].red & 0xff) + (backupOrg[loc1].green & 0xff) + (backupOrg[loc1].blue & 0xff);
					if (loopcond <= min_intensity) {
						min_intensity = loopcond;
						min_row = ii;
						min_col = jj;
					}
					if (loopcond > max_intensity) {
						max_intensity = loopcond;
						max_row = ii;
						max_col = jj;
					}
					kCol = (kCol + 1) % 3;
				}
				kRow = (kRow + 1) % 3;
			}
	
			
			// filter out min and max
			// sum_pixels_by_weight(&sum, src[calcIndex(min_row, min_col, dim)], -1);
			// sum_pixels_by_weight(&sum, src[calcIndex(max_row, max_col, dim)], -1);
			int loc1 = min_row * locm + min_col;
			int loc2 = max_row * locm + max_col;

			
		
			sum.red -= (int)(backupOrg[loc1].red + backupOrg[loc2].red);
			sum.green -= (int)(backupOrg[loc1].green + backupOrg[loc2].green);
			sum.blue -= (int)(backupOrg[loc1].blue + backupOrg[loc2].blue);
			
			

			// assign kernel's result to pixel at [i,j]
			// assign_sum_to_pixel(&current_pixel, sum, kernelScale);
			// divide by kernel's weight
			sum.red /= kernelScale;
			sum.green /= kernelScale;
			sum.blue /= kernelScale;
			
			// truncate each pixel's color values to match the range [0,255]
			int maxsum = sum.red > 0 ? sum.red : 0;
			int minsumred = maxsum < 255 ? maxsum : 255;
			maxsum = sum.green > 0 ? sum.green : 0;
			int minsumgreen = maxsum < 255 ? maxsum : 255;
			maxsum = sum.blue > 0 ? sum.blue : 0;
			int minsumblue = maxsum < 255 ? maxsum : 255;
			
			pixelsImg[loc].red = minsumred & 0xff;
			pixelsImg[loc].green = minsumgreen & 0xff;
			pixelsImg[loc].blue = minsumblue & 0xff;
		}
	}

	// pixelsToChars(pixelsImg, image);
	for (row = 0; row < locm ; row++) {
		int aloc = row*locn;
		int bloc = 3*aloc;
		for (col = 0; col < locn ; col++) {
			int ploc = aloc + col;
			int iloc = bloc + 3*col;
			memcpy(image->data + iloc, pixelsImg + ploc, 3);
		}
	}

	free(pixelsImg);
	free(backupOrg);
}

