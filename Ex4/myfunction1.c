//Shlomo Shatz 316093202

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "myfunction1.h"
#include "showBMP.h"

#include <stdio.h>

void doConvolution(Image *image, int kernel[3][3], int kernelScale) {

	int row, col, locn = n;

	pixel* pixelsImg = malloc(locn*locn*sizeof(pixel));
	pixel* backupOrg = malloc(locn*locn*sizeof(pixel));

	int aloc = 0 - locn;
	for (row = 0 ; row < locn ; row++) {
		aloc += locn;
		int bloc = aloc + aloc + aloc;
		for (col = 0 ; col < locn ; col += 2) {
			int ploc = aloc + col;
			int iloc = bloc + col + col + col;
			memcpy(pixelsImg + ploc, image->data + iloc, 3 * sizeof(char));
			memcpy(backupOrg + ploc, pixelsImg + ploc, 3* sizeof(char));
			int vloc = aloc + col + 1;
			int uloc = bloc + col + col + col + 3;
			memcpy(pixelsImg + vloc, image->data + uloc, 3* sizeof(char));
			memcpy(backupOrg + vloc, pixelsImg + vloc, 3* sizeof(char));
		}
	}

	// smooth(m, backupOrg, pixelsImg, kernelSize, kernel, kernelScale, filter);
	int i, j;
	int dimReduce = locn - 1;
	int locf = 0;
	for (i = 1 ; i < dimReduce; i++) {
		locf += locn;
		int iimax = (i - 1) * (i-1 > 0), iimin = (i+1 < locn-1 ? i+1 : locn-1);
		for (j =  1 ; j < dimReduce ; j++) {
			int loc = locf + j;
			//  pixelsImg[loc] = applyKernel(locn, i, j, backupOrg, kernelSize, kernel, kernelScale, filter);
		
			int ii, jj;
			pixel_sum sum;
			int min_intensity = 766; // arbitrary value that is higher than maximum possible intensity, which is 255*3=765
			int max_intensity = -1; // arbitrary value that is lower than minimum possible intensity, which is 0
			int min_row, min_col, max_row, max_col;
			pixel loop_pixel;

			memset(&sum, 0, sizeof(pixel_sum));

			int kRow = 0;
			int jjmax = (j - 1) * (j-1 > 0), jjmin = (j+1 < locn - 1 ? j+1 : locn - 1);
			for(ii = iimax; ii <= iimin; ii++) {
				int kCol = 0;
				int loc2 = ii * locn;
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
			int maxsum = sum.red * (sum.red > 0);
			int minsumred = maxsum < 255 ? maxsum : 255;
			maxsum = sum.green * (sum.green > 0);
			int minsumgreen = maxsum < 255 ? maxsum : 255;
			maxsum = sum.blue * (sum.blue > 0);
			int minsumblue = maxsum < 255 ? maxsum : 255;
			pixelsImg[loc].red = minsumred & 0xff;
			pixelsImg[loc].green = minsumgreen & 0xff;
			pixelsImg[loc].blue = minsumblue & 0xff;
		}
	}

	// pixelsToChars(pixelsImg, image);

	aloc = 0 - locn;
	for (row = 0 ; row < locn ; row++) {
		aloc += locn;
		int bloc = aloc + aloc + aloc;
		for (col = 0 ; col < locn ; col += 2) {
			int ploc = aloc + col;
			int iloc = bloc + col + col + col;
			memcpy(image->data + iloc, pixelsImg + ploc, 3 * sizeof(char));
			int vloc = aloc + col + 1;
			int uloc = bloc + col + col + col + 3;
			memcpy(image->data + uloc, pixelsImg + vloc, 3 * sizeof(char));
		}
	}

	free(pixelsImg);
	free(backupOrg);
}

void doConvolutionFilter(Image *image, int kernel[3][3], int kernelScale) {

	int row, col, locn = n;

	pixel* pixelsImg = malloc(locn*locn*sizeof(pixel));
	pixel* backupOrg = malloc(locn*locn*sizeof(pixel));

	int aloc = 0 - locn;
	for (row = 0 ; row < locn ; row++) {
		aloc += locn;
		int bloc = aloc + aloc + aloc;
		for (col = 0 ; col < locn ; col += 2) {
			int ploc = aloc + col;
			int iloc = bloc + col + col + col;
			memcpy(pixelsImg + ploc, image->data + iloc, 3 * sizeof(char));
			memcpy(backupOrg + ploc, pixelsImg + ploc, 3 * sizeof(char));
			int vloc = aloc + col + 1;
			int uloc = bloc + col + col + col + 3;
			memcpy(pixelsImg + vloc, image->data + uloc, 3 * sizeof(char));
			memcpy(backupOrg + vloc, pixelsImg + vloc, 3 * sizeof(char));
		}
	}

	// smooth(m, backupOrg, pixelsImg, kernelSize, kernel, kernelScale, filter);
	int i, j;
	int dimReduce = locn - 1;
	int locf = 0;
	for (i = 1 ; i < dimReduce; i++) {
		locf += locn;
		int iimax = (i - 1) * (i-1 > 0), iimin = (i+1 < locn-1 ? i+1 : locn-1);
		for (j =  1 ; j < dimReduce ; j++) {
			int loc = locf + j;
			//  pixelsImg[loc] = applyKernel(locn, i, j, backupOrg, kernelSize, kernel, kernelScale, filter);
		
			int ii, jj;
			pixel_sum sum;
			int min_intensity = 766; // arbitrary value that is higher than maximum possible intensity, which is 255*3=765
			int max_intensity = -1; // arbitrary value that is lower than minimum possible intensity, which is 0
			int min_row, min_col, max_row, max_col;
			pixel loop_pixel;

			memset(&sum, 0, sizeof(pixel_sum));

			int kRow = 0;
			int jjmax = (j - 1) * (j-1 > 0), jjmin = (j+1 < locn-1 ? j+1 : locn-1);
			for(ii = iimax; ii <= iimin; ii++) {
				int kCol = 0;
				int loc2 = ii * locn;
				for(jj = jjmax; jj <= jjmin; jj++) {
					// apply kernel on pixel at [ii,jj]
					// sum_pixels_by_weight(&sum, src[calcIndex(ii, jj, dim)], kernel[kRow][kCol]);

					int loc1 = loc2 + jj;
					int weight = kernel[kRow][kCol];
					sum.red += (backupOrg[loc1].red & 0xff) * weight;
					sum.green += (backupOrg[loc1].green & 0xff) * weight;
					sum.blue += (backupOrg[loc1].blue & 0xff) * weight;

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
					kCol = (kCol + 1) % 3;
				}
				kRow = (kRow + 1) % 3;
			}
	
			
			// filter out min and max
			// sum_pixels_by_weight(&sum, src[calcIndex(min_row, min_col, dim)], -1);
			// sum_pixels_by_weight(&sum, src[calcIndex(max_row, max_col, dim)], -1);
			int loc1 = min_row * locn + min_col;
			int loc2 = max_row * locn + max_col;

			
		
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
			int maxsum = sum.red * (sum.red > 0);
			int minsumred = maxsum < 255 ? maxsum : 255;
			maxsum = sum.green * (sum.green > 0);
			int minsumgreen = maxsum < 255 ? maxsum : 255;
			maxsum = sum.blue * (sum.blue > 0);
			int minsumblue = maxsum < 255 ? maxsum : 255;
			pixelsImg[loc].red = minsumred & 0xff;
			pixelsImg[loc].green = minsumgreen & 0xff;
			pixelsImg[loc].blue = minsumblue & 0xff;
		}
	}

	// pixelsToChars(pixelsImg, image);
	aloc = 0 - locn;
	for (row = 0 ; row < locn ; row++) {
		aloc += locn;
		int bloc = aloc + aloc + aloc;
		for (col = 0 ; col < locn ; col += 2) {
			int ploc = aloc + col;
			int iloc = bloc + col + col + col;
			memcpy(image->data + iloc, pixelsImg + ploc, 3 * sizeof(char));
			int vloc = aloc + col + 1;
			int uloc = bloc + col + col + col + 3;
			memcpy(image->data + uloc, pixelsImg + vloc, 3 * sizeof(char));
		}
	}

	free(pixelsImg);
	free(backupOrg);
}

