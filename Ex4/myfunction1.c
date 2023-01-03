//Shlomo Shatz 316093202

#include <stdlib.h>
#include <stdbool.h>
#include "myfunction1.h"
#include "showBMP.h"
/*
 * initialize_pixel_sum - Initializes all fields of sum to 0
 */
void initialize_pixel_sum(pixel_sum *sum) {
	sum->red = sum->green = sum->blue = 0;
	// sum->num = 0;
	return;
}

/*
 * assign_sum_to_pixel - Truncates pixel's new value to match the range [0,255]
 */
static void assign_sum_to_pixel(pixel *current_pixel, pixel_sum sum, int kernelScale) {

	// divide by kernel's weight
	sum.red = sum.red / kernelScale;
	sum.green = sum.green / kernelScale;
	sum.blue = sum.blue / kernelScale;

	// truncate each pixel's color values to match the range [0,255]
	current_pixel->red = (unsigned char) (min(max(sum.red, 0), 255));
	current_pixel->green = (unsigned char) (min(max(sum.green, 0), 255));
	current_pixel->blue = (unsigned char) (min(max(sum.blue, 0), 255));
	return;
}

/*
* sum_pixels_by_weight - Sums pixel values, scaled by given weight
*/
static void sum_pixels_by_weight(pixel_sum *sum, pixel p, int weight) {
	sum->red += ((int) p.red) * weight;
	sum->green += ((int) p.green) * weight;
	sum->blue += ((int) p.blue) * weight;
	// sum->num++;
	return;
}

/*
 *  Applies kernel for pixel at (i,j)
 */
static pixel applyKernel(int dim, int i, int j, pixel *src, int kernelSize, int kernel[kernelSize][kernelSize], int kernelScale, bool filter) {

	int ii, jj;
	int currRow, currCol;
	pixel_sum sum;
	pixel current_pixel;
	int min_intensity = 766; // arbitrary value that is higher than maximum possible intensity, which is 255*3=765
	int max_intensity = -1; // arbitrary value that is lower than minimum possible intensity, which is 0
	int min_row, min_col, max_row, max_col;
	pixel loop_pixel;

	sum.blue = 0;
	sum.green = 0;
	sum.red = 0;

	int iimax = (i-1 > 0 ? i-1 : 0), iimin = (i+1 < dim-1 ? i+1 : dim-1), jjmax = (j-1 > 0 ? j-1 : 0), jjmin = (j+1 < dim-1 ? j+1 : dim-1);
	for(ii = iimax; ii <= iimin; ii++) {
		for(jj = jjmax; jj <= jjmin; jj++) {

			int kRow, kCol;

			// compute row index in kernel
			if (ii < i) {
				kRow = 0;
			} else if (ii > i) {
				kRow = 2;
			} else {
				kRow = 1;
			}

			// compute column index in kernel
			if (jj < j) {
				kCol = 0;
			} else if (jj > j) {
				kCol = 2;
			} else {
				kCol = 1;
			}

			// apply kernel on pixel at [ii,jj]
			// sum_pixels_by_weight(&sum, src[calcIndex(ii, jj, dim)], kernel[kRow][kCol]);
			int loc1 = ii * dim + jj;
			int weight = kernel[kRow][kCol];
			sum.red += ((int) src[loc1].red) * weight;
			sum.green += ((int) src[loc1].green) * weight;
			sum.blue += ((int) src[loc1].blue) * weight;
		}
	}

	if (filter) {
		// find min and max coordinates
		for(ii = iimax; ii <= iimin; ii++) {
			for(jj = jjmax; jj <= jjmin; jj++) {
				// check if smaller than min or higher than max and update
				loop_pixel = src[ii * dim + jj];
				int loopred = (int) loop_pixel.red;
				int loopgreen = (int) loop_pixel.green;
				int loopblue = (int) loop_pixel.blue;
				if ((loopred + loopgreen + loopblue) <= min_intensity) {
					min_intensity = (loopred + loopgreen + loopblue);
					min_row = ii;
					min_col = jj;
				}
				if ((loopred + loopgreen + loopblue) > max_intensity) {
					max_intensity = (loopred + loopgreen + loopblue);
					max_row = ii;
					max_col = jj;
				}
			}
		}
		// filter out min and max
		// sum_pixels_by_weight(&sum, src[calcIndex(min_row, min_col, dim)], -1);
		// sum_pixels_by_weight(&sum, src[calcIndex(max_row, max_col, dim)], -1);
		int loc1 = min_row * dim + min_col;
		int loc2 = max_row * dim + max_col;
	
		sum.red += (int)(src[loc1].red + src[loc2].red) * -1;
		sum.green += (int)(src[loc1].green + src[loc2].green) * -1;;
		sum.blue += (int)(src[loc1].blue + src[loc2].blue) * -1;
	}

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
	current_pixel.red = (unsigned char) (minsumred);
	current_pixel.green = (unsigned char) (minsumgreen);
	current_pixel.blue = (unsigned char) (minsumblue);

	return current_pixel;
}

/*
* Apply the kernel over each pixel.
* Ignore pixels where the kernel exceeds bounds. These are pixels with row index smaller than kernelSize/2 and/or
* column index smaller than kernelSize/2
*/
void smooth(int dim, pixel *src, pixel *dst, int kernelSize, int kernel[kernelSize][kernelSize], int kernelScale, bool filter) {

	int i, j;
	int kernelDivide = kernelSize / 2;
	int dimReduce = dim - kernelDivide;
	for (i = kernelDivide ; i < dimReduce; i++) {
		for (j =  kernelDivide ; j < dimReduce ; j++) {
			int loc = i * dim + j;
			dst[loc] = applyKernel(dim, i, j, src, kernelSize, kernel, kernelScale, filter);
		}
	}
}

void charsToPixels(Image *charsImg, pixel* pixels) {


	int row, col, locm = m, locn = n;
	for (row = 0 ; row < locm ; row++) {
		for (col = 0 ; col < locn ; col++) {
			int ploc = row*locn + col;
			int iloc = 3*row*locn + 3*col;
			pixels[ploc].red = charsImg->data[iloc];
			pixels[ploc].green = charsImg->data[iloc + 1];
			pixels[ploc].blue = charsImg->data[iloc + 2];
		}
	}
}

void pixelsToChars(pixel* pixels, Image *charsImg) {

	int row, col;
	for (row = 0 ; row < m ; row++) {
		for (col = 0 ; col < n ; col++) {

			image->data[3*row*n + 3*col] = pixels[row*n + col].red;
			image->data[3*row*n + 3*col + 1] = pixels[row*n + col].green;
			image->data[3*row*n + 3*col + 2] = pixels[row*n + col].blue;
		}
	}
}

void copyPixels(pixel* src, pixel* dst) {

	int row, col;
	for (row = 0 ; row < m ; row++) {
		for (col = 0 ; col < n ; col++) {

			dst[row*n + col].red = src[row*n + col].red;
			dst[row*n + col].green = src[row*n + col].green;
			dst[row*n + col].blue = src[row*n + col].blue;
		}
	}
}

void doConvolution(Image *image, int kernelSize, int kernel[kernelSize][kernelSize], int kernelScale, bool filter) {

	pixel* pixelsImg = malloc(m*n*sizeof(pixel));
	pixel* backupOrg = malloc(m*n*sizeof(pixel));
	
	int row, col, locm = m, locn = n;
	for (row = 0 ; row < locm ; row++) {
		for (col = 0 ; col < locn ; col++) {
			int ploc = row*locn + col;
			int iloc = 3*row*locn + 3*col;
			pixelsImg[ploc].red = image->data[iloc];
			pixelsImg[ploc].green = image->data[iloc + 1];
			pixelsImg[ploc].blue = image->data[iloc + 2];
		}
	}

	for (row = 0 ; row < m ; row++) {
		for (col = 0 ; col < n ; col++) {
			int ploc = row*locn + col;
			backupOrg[ploc].red = pixelsImg[ploc].red;
			backupOrg[ploc].green = pixelsImg[ploc].green;
			backupOrg[ploc].blue = pixelsImg[ploc].blue;
		}
	}

	// smooth(m, backupOrg, pixelsImg, kernelSize, kernel, kernelScale, filter);
	int i, j;
	int kernelDivide = kernelSize / 2;
	int dimReduce = locm - kernelDivide;
	for (i = kernelDivide ; i < dimReduce; i++) {
		for (j =  kernelDivide ; j < dimReduce ; j++) {
			int loc = i * locm + j;
			//  pixelsImg[loc] = applyKernel(locm, i, j, backupOrg, kernelSize, kernel, kernelScale, filter);
		
			int ii, jj;
			int currRow, currCol;
			pixel_sum sum;
			pixel current_pixel;
			int min_intensity = 766; // arbitrary value that is higher than maximum possible intensity, which is 255*3=765
			int max_intensity = -1; // arbitrary value that is lower than minimum possible intensity, which is 0
			int min_row, min_col, max_row, max_col;
			pixel loop_pixel;

			sum.blue = 0;
			sum.green = 0;
			sum.red = 0;

			int iimax = (i-1 > 0 ? i-1 : 0), iimin = (i+1 < locm-1 ? i+1 : locm-1), jjmax = (j-1 > 0 ? j-1 : 0), jjmin = (j+1 < locm-1 ? j+1 : locm-1);
			for(ii = iimax; ii <= iimin; ii++) {
				for(jj = jjmax; jj <= jjmin; jj++) {

					int kRow, kCol;

					// compute row index in kernel
					if (ii < i) {
						kRow = 0;
					} else if (ii > i) {
						kRow = 2;
					} else {
						kRow = 1;
					}

					// compute column index in kernel
					if (jj < j) {
						kCol = 0;
					} else if (jj > j) {
						kCol = 2;
					} else {
						kCol = 1;
					}

					// apply kernel on pixel at [ii,jj]
					// sum_pixels_by_weight(&sum, src[calcIndex(ii, jj, dim)], kernel[kRow][kCol]);
					int loc1 = ii * locm + jj;
					int weight = kernel[kRow][kCol];
					sum.red += ((int) backupOrg[loc1].red) * weight;
					sum.green += ((int) backupOrg[loc1].green) * weight;
					sum.blue += ((int) backupOrg[loc1].blue) * weight;
				}
			}

			if (filter) {
				// find min and max coordinates
				for(ii = iimax; ii <= iimin; ii++) {
					for(jj = jjmax; jj <= jjmin; jj++) {
						// check if smaller than min or higher than max and update
						loop_pixel = backupOrg[ii * locm + jj];
						int loopred = (int) loop_pixel.red;
						int loopgreen = (int) loop_pixel.green;
						int loopblue = (int) loop_pixel.blue;
						if ((loopred + loopgreen + loopblue) <= min_intensity) {
							min_intensity = (loopred + loopgreen + loopblue);
							min_row = ii;
							min_col = jj;
						}
						if ((loopred + loopgreen + loopblue) > max_intensity) {
							max_intensity = (loopred + loopgreen + loopblue);
							max_row = ii;
							max_col = jj;
						}
					}
				}
				// filter out min and max
				// sum_pixels_by_weight(&sum, src[calcIndex(min_row, min_col, dim)], -1);
				// sum_pixels_by_weight(&sum, src[calcIndex(max_row, max_col, dim)], -1);
				int loc1 = min_row * locm + min_col;
				int loc2 = max_row * locm + max_col;
			
				sum.red += (int)(backupOrg[loc1].red + backupOrg[loc2].red) * -1;
				sum.green += (int)(backupOrg[loc1].green + backupOrg[loc2].green) * -1;;
				sum.blue += (int)(backupOrg[loc1].blue + backupOrg[loc2].blue) * -1;
			}
			

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
			current_pixel.red = (unsigned char) (minsumred);
			current_pixel.green = (unsigned char) (minsumgreen);
			current_pixel.blue = (unsigned char) (minsumblue);

			pixelsImg[loc] = current_pixel;
		}
	}

	// pixelsToChars(pixelsImg, image);
	for (row = 0 ; row < m ; row++) {
		for (col = 0 ; col < n ; col++) {
			int ploc = row*locn + col;
			int iloc = 3*row*locn + 3*col;
			image->data[iloc] = pixelsImg[ploc].red;
			image->data[iloc + 1] = pixelsImg[ploc].green;
			image->data[iloc + 2] = pixelsImg[ploc].blue;
		}
	}

	free(pixelsImg);
	free(backupOrg);
}

