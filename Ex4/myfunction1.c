//Shlomo Shatz 316093202

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "myfunction1.h"
#include "showBMP.h"

void doConvolutionBlur(Image *image) {

	int row, col, locn = n;
	int nn = 1000000 * sizeof(pixel);

	pixel* pixelsImg = malloc(nn);
	pixel* backupOrg = malloc(nn);

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

	// smooth(m, backupOrg, pixelsImg, kernelSize, kernel, kernelScale, filter);
	int i, j;
	int dimReduce = locn - 1;
	int locf = 0;
	for (i = 1 ; i < dimReduce; i++) {
		locf += locn;
		int iimax = i-1, iimin = i+1;
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
			int jjmax = j-1, jjmin = j+1;
			for(ii = iimax; ii <= iimin; ii++) {
				int kCol = 0;
				int loc2 = ii * locn;
				for(jj = jjmax; jj <= jjmin; jj++) {
					// apply kernel on pixel at [ii,jj]
					// sum_pixels_by_weight(&sum, src[calcIndex(ii, jj, dim)], kernel[kRow][kCol]);


					int loc1 = loc2 + jj;
					
					// memcpy(&sum, backupOrg + loc1, 3 * sizeof(char));
					sum.red += ((int)backupOrg[loc1].red);
					sum.green += ((int)backupOrg[loc1].green);
					sum.blue += ((int)backupOrg[loc1].blue);
					
				}
				
			}

			// divide by kernel's weight
			sum.red /= 9;
			sum.green /= 9;
			sum.blue /= 9;
			
			// truncate each pixel's color values to match the range [0,255]
			int maxsum = 0;
			if (sum.red > 0) {
				if (sum.red <= 255) maxsum = sum.red;
				else maxsum = 255;
			}
			int minsumred = maxsum;
			if (sum.green > 0) {
				if (sum.green <= 255) maxsum = sum.green;
				else maxsum = 255;
			} else maxsum = 0;
			int minsumgreen = maxsum;
			if (sum.blue > 0) {
				if (sum.blue <= 255) maxsum = sum.blue;
				else maxsum = 255;
			} else maxsum = 0;
			pixelsImg[loc].blue = (unsigned char) maxsum;
			pixelsImg[loc].red = (unsigned char) minsumred;
			pixelsImg[loc].green = (unsigned char) minsumgreen;
		}
	}

	// pixelsToChars(pixelsImg, image);

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

	free(pixelsImg);
	free(backupOrg);
}

void doConvolutionFilterBlur(Image *image) {

	int row, col, locn = n;
	int nn = 1000000 * sizeof(pixel);

	pixel* pixelsImg = malloc(nn);
	pixel* backupOrg = malloc(nn);

	int aloc = 0 - locn;
	for (row = 0 ; row < locn ; row++) {
		aloc += locn;
		int bloc = aloc + aloc + aloc;
		for (col = 0 ; col < locn ; col++) {
			int ploc = aloc + col;
			int iloc = bloc + col + col + col;
			memcpy(pixelsImg + ploc, image->data + iloc, 3 * sizeof(char));
			memcpy(backupOrg + ploc, pixelsImg + ploc, 3 * sizeof(char));
		}
	}

	// smooth(m, backupOrg, pixelsImg, kernelSize, kernel, kernelScale, filter);
	int i, j;
	int dimReduce = locn - 1;
	int locf = 0;
	for (i = 1 ; i < dimReduce; i++) {
		locf += locn;
		int iimax = i-1, iimin = i+1;
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

			int jjmax = j-1, jjmin = j+1;
			for(ii = iimax; ii <= iimin; ii++) {
				int loc2 = ii * locn;
				for(jj = jjmax; jj <= jjmin; jj++) {
					// apply kernel on pixel at [ii,jj]
					// sum_pixels_by_weight(&sum, src[calcIndex(ii, jj, dim)], kernel[kRow][kCol]);


					int loc1 = loc2 + jj;
					
					sum.red += ((int)backupOrg[loc1].red);
					sum.green += ((int)backupOrg[loc1].green);
					sum.blue += ((int)backupOrg[loc1].blue);
					
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


			
			// filter out min and max
			// sum_pixels_by_weight(&sum, src[calcIndex(min_row, min_col, dim)], -1);
			// sum_pixels_by_weight(&sum, src[calcIndex(max_row, max_col, dim)], -1);
			int loc1 = min_row * locn + min_col;
			int loc2 = max_row * locn + max_col;

			
		
			sum.red -= (int)(backupOrg[loc1].red + backupOrg[loc2].red);
			sum.green -= (int)(backupOrg[loc1].green + backupOrg[loc2].green);
			sum.blue -= (int)(backupOrg[loc1].blue + backupOrg[loc2].blue);
			
			
			// divide by kernel's weight
			sum.red /= 7;
			sum.green /= 7;
			sum.blue /= 7;

			// truncate each pixel's color values to match the range [0,255]
			int maxsum = 0;
			if (sum.red > 0) {
				if (sum.red <= 255) maxsum = sum.red;
				else maxsum = 255;
			}
			int minsumred = maxsum;
			if (sum.green > 0) {
				if (sum.green <= 255) maxsum = sum.green;
				else maxsum = 255;
			} else maxsum = 0;
			int minsumgreen = maxsum;
			if (sum.blue > 0) {
				if (sum.blue <= 255) maxsum = sum.blue;
				else maxsum = 255;
			} else maxsum = 0;
			pixelsImg[loc].blue = (unsigned char) maxsum;
			pixelsImg[loc].red = (unsigned char) minsumred;
			pixelsImg[loc].green = (unsigned char) minsumgreen;
		}
	}

	// pixelsToChars(pixelsImg, image);
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

	free(pixelsImg);
	free(backupOrg);
}

void doConvolutionSharp(Image *image) {


	int row, col, locn = n;
	int nn = 1000000 * sizeof(pixel);

	pixel* pixelsImg = malloc(nn);
	pixel* backupOrg = malloc(nn);

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

	// smooth(m, backupOrg, pixelsImg, kernelSize, kernel, kernelScale, filter);
	int i, j;
	int dimReduce = locn - 1;
	int locf = 0;
	for (i = 1 ; i < dimReduce; i++) {
		locf += locn;
		int iimax = i-1, iimin = i+1;
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

			int jjmax = j-1, jjmin = j+1;
			int loc2 = iimax * locn;


			int loc1 = loc2 + jjmax;
		
			sum.red -= ((int)backupOrg[loc1].red);
			sum.green -= ((int)backupOrg[loc1].green);
			sum.blue -= ((int)backupOrg[loc1].blue);

			loc1++;
			
			sum.red -= ((int)backupOrg[loc1].red);
			sum.green -= ((int)backupOrg[loc1].green);
			sum.blue -= ((int)backupOrg[loc1].blue);

			loc1++;
			
			sum.red -= ((int)backupOrg[loc1].red);
			sum.green -= ((int)backupOrg[loc1].green);
			sum.blue -= ((int)backupOrg[loc1].blue);

			loc2 += locn;

			loc1 = loc2 + jjmax;
			
			sum.red -= ((int)backupOrg[loc1].red);
			sum.green -= ((int)backupOrg[loc1].green);
			sum.blue -= ((int)backupOrg[loc1].blue);

			loc1++;
			
			sum.red += ((int)backupOrg[loc1].red * 9);
			sum.green += ((int)backupOrg[loc1].green * 9);
			sum.blue += ((int)backupOrg[loc1].blue * 9);

			loc1++;
			
			sum.red -= ((int)backupOrg[loc1].red);
			sum.green -= ((int)backupOrg[loc1].green);
			sum.blue -= ((int)backupOrg[loc1].blue);

			loc2 += locn;

			loc1 = loc2 + jjmax;
			
			sum.red -= ((int)backupOrg[loc1].red);
			sum.green -= ((int)backupOrg[loc1].green);
			sum.blue -= ((int)backupOrg[loc1].blue);

			loc1++;
			
			sum.red -= ((int)backupOrg[loc1].red);
			sum.green -= ((int)backupOrg[loc1].green);
			sum.blue -= ((int)backupOrg[loc1].blue);

			loc1++;
			
			sum.red -= ((int)backupOrg[loc1].red);
			sum.green -= ((int)backupOrg[loc1].green);
			sum.blue -= ((int)backupOrg[loc1].blue);

			// truncate each pixel's color values to match the range [0,255]
			int maxsum = 0;
			if (sum.red > 0) {
				if (sum.red <= 255) maxsum = sum.red;
				else maxsum = 255;
			}
			int minsumred = maxsum;
			if (sum.green > 0) {
				if (sum.green <= 255) maxsum = sum.green;
				else maxsum = 255;
			} else maxsum = 0;
			int minsumgreen = maxsum;
			if (sum.blue > 0) {
				if (sum.blue <= 255) maxsum = sum.blue;
				else maxsum = 255;
			} else maxsum = 0;
			pixelsImg[loc].blue = (unsigned char) maxsum;
			pixelsImg[loc].red = (unsigned char) minsumred;
			pixelsImg[loc].green = (unsigned char) minsumgreen;
		}
	}

	// pixelsToChars(pixelsImg, image);

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

	free(pixelsImg);
	free(backupOrg);
}

void doConvolutionRowBlur(Image *image) {


	int row, col, locn = n;
	int nn = 1000000 * sizeof(pixel);

	pixel* pixelsImg = malloc(nn);
	pixel* backupOrg = malloc(nn);

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

	// smooth(m, backupOrg, pixelsImg, kernelSize, kernel, kernelScale, filter);
	int i, j;
	int dimReduce = locn - 1;
	int locf = 0;
	for (i = 1 ; i < dimReduce; i++) {
		locf += locn;
		int iimax = i-1, iimin = i+1;
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

			int jjmax = j-1, jjmin = j+1;
			int loc2 = iimax * locn;

			loc2 += locn;

			int loc1 = loc2 + jjmax;
			
			sum.red += ((int)backupOrg[loc1].red);
			sum.green += ((int)backupOrg[loc1].green);
			sum.blue += ((int)backupOrg[loc1].blue);

			loc1++;
			
			sum.red += ((int)backupOrg[loc1].red * 2);
			sum.green += ((int)backupOrg[loc1].green * 2);
			sum.blue += ((int)backupOrg[loc1].blue * 2);

			loc1++;
			
			sum.red += ((int)backupOrg[loc1].red);
			sum.green += ((int)backupOrg[loc1].green);
			sum.blue += ((int)backupOrg[loc1].blue);

			sum.red /= 4;
			sum.green /= 4;
			sum.blue /= 4;
			
			// truncate each pixel's color values to match the range [0,255]
			int maxsum = 0;
			if (sum.red > 0) {
				if (sum.red <= 255) maxsum = sum.red;
				else maxsum = 255;
			}
			int minsumred = maxsum;
			if (sum.green > 0) {
				if (sum.green <= 255) maxsum = sum.green;
				else maxsum = 255;
			} else maxsum = 0;
			int minsumgreen = maxsum;
			if (sum.blue > 0) {
				if (sum.blue <= 255) maxsum = sum.blue;
				else maxsum = 255;
			} else maxsum = 0;
			pixelsImg[loc].blue = (unsigned char) maxsum;
			pixelsImg[loc].red = (unsigned char) minsumred;
			pixelsImg[loc].green = (unsigned char) minsumgreen;
		}
	}

	// pixelsToChars(pixelsImg, image);

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

	free(pixelsImg);
	free(backupOrg);
}

void doConvolutionRowSharp(Image *image) {


	int row, col, locn = n;
	int nn = 1000000 * sizeof(pixel);

	pixel* pixelsImg = malloc(nn);
	pixel* backupOrg = malloc(nn);

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

	// smooth(m, backupOrg, pixelsImg, kernelSize, kernel, kernelScale, filter);
	int i, j;
	int dimReduce = locn - 1;
	int locf = 0;
	for (i = 1 ; i < dimReduce; i++) {
		locf += locn;
		int iimax = i-1, iimin = i+1;
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

			int jjmax = j-1, jjmin = j+1;
			int loc2 = iimax * locn;

			loc2 += locn;

			int loc1 = loc2 + jjmax;
			
			sum.red -= ((int)backupOrg[loc1].red * 2);
			sum.green -= ((int)backupOrg[loc1].green * 2);
			sum.blue -= ((int)backupOrg[loc1].blue * 2);

			loc1++;
			
			sum.red += ((int)backupOrg[loc1].red * 6);
			sum.green += ((int)backupOrg[loc1].green * 6);
			sum.blue += ((int)backupOrg[loc1].blue * 6);

			loc1++;
			
			sum.red -= ((int)backupOrg[loc1].red * 2);
			sum.green -= ((int)backupOrg[loc1].green * 2);
			sum.blue -= ((int)backupOrg[loc1].blue * 2);

			sum.red /= 2;
			sum.green /= 2;
			sum.blue /= 2;
			
			// truncate each pixel's color values to match the range [0,255]
			int maxsum = 0;
			if (sum.red > 0) {
				if (sum.red <= 255) maxsum = sum.red;
				else maxsum = 255;
			}
			int minsumred = maxsum;
			if (sum.green > 0) {
				if (sum.green <= 255) maxsum = sum.green;
				else maxsum = 255;
			} else maxsum = 0;
			int minsumgreen = maxsum;
			if (sum.blue > 0) {
				if (sum.blue <= 255) maxsum = sum.blue;
				else maxsum = 255;
			} else maxsum = 0;
			pixelsImg[loc].blue = (unsigned char) maxsum;
			pixelsImg[loc].red = (unsigned char) minsumred;
			pixelsImg[loc].green = (unsigned char) minsumgreen;
		}
	}

	// pixelsToChars(pixelsImg, image);

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

	free(pixelsImg);
	free(backupOrg);
}
