#include <iostream>
#include <cmath>
#include <string>
#include <cstring>

using namespace std;



short kernelThreeX[3][3] = 
	{
		{-1,  0,  1},
		{-2,  0,  2},
		{-1,  0,  1}
	};

short kernelThreeY[3][3] =
	{
		{-1, -2, -1},
		{ 0,  0,  0},
		{ 1,  2,  1}
	};

short kernelFiveX[5][5] =
	{
		{-1, -2,  0,  2,  1},
		{-2, -4,  0,  4,  2},
		{-4, -8,  0,  8,  4},
		{-2, -4,  0,  4,  2},
		{-1, -2,  0,  2,  1}
	};

short kernelFiveY[5][5] =
	{
		{-1, -2, -4, -2, -1},
		{-2, -4, -8, -4, -2},
		{ 0,  0,  0,  0,  0},
		{ 2,  4,  8,  4,  2},
		{ 1,  2,  4,  2,  1}
	};	

int main() {

	// Image Paramaters / Arrays
	const int h = 378, w = 672;
	unsigned char img[h][w];
	unsigned char imgEdge[h][w], imgX[h][w], imgY[h][w];
	short tempX[h][w], tempY[h][w];	// Holds Derivative Approximations

	// Edge Detection Threshold Parameters
	const unsigned char THRESHOLD = 12;
	const unsigned char POSITIVE = 128 + THRESHOLD;
	const bool GRADIENT = false;

	// Read Original Raw Image File
	FILE * input = fopen("city.raw", "rb");
	fread(img, 1, h * w, input);



	// 3x3 Sobel Filter

	short minX = 10000, minY = 10000, maxX = -10000, maxY=-10000;

	for(int i = 1; i < h; i++) {
		for(int j = 1; j < w; j++) {
			short x = 0, y = 0;

			// Calculates 3x3 Derivative Approximations per Pixel
			for(short k = 0; k < 3; k++)
				for(short l = 0; l < 3; l++){
					x += img[i-1+k][j-1+l] * kernelThreeX[k][l];
					y += img[i-1+k][j-1+l] * kernelThreeY[k][l];
				}

			// Finds Min/Max Values
			if(x < minX)
				minX = x;
			else if (x > maxX)
				maxX = x;

			if(y < minY)
				minY = y;
			else if (y > maxY)
				maxY = y;

			tempX[i][j] = x;
			tempY[i][j] = y;
		}
	}


	// Min/Max Normailization
	for(int i = 0; i < h; i++)
		for(int j = 0; j < w; j++){
			float ratioX = (tempX[i][j] - minX) / (float)(maxX - minX);
			float ratioY = (tempY[i][j] - minY) / (float)(maxY - minY);
			// Uses Min/Max Normaalization Ratio's to Determine Pixel Vaule Offsets from 128
			if(ratioX >= 0.50000)
				imgX[i][j] = ratioX * 255;
			else
				imgX[i][j] = (1.0 - ratioX) * 255;
			if(ratioY >= 0.50000)
				imgY[i][j] = ratioY * 255;
			else
				imgY[i][j] = (1.0 - ratioY) * 255;

			if (GRADIENT) {
				imgX[i][j] = sqrt(pow(imgX[i][j]-127,2) + pow(imgY[i][j]-127,2));
			}

			imgEdge[i][j] = 0;
		}


	// Edge Detection
	for(int i = 1; i < h-1; i++)
		for(int j = 1; j < w - 1; j++) {
			if((imgX[i][j] >= POSITIVE && imgX[i][j] > imgX[i][j-1] && imgX[i][j] > imgX[i][j+1]))
				imgEdge[i][j] = 200;
			if((imgY[i][j] >= POSITIVE && imgY[i][j] > imgY[i-1][j] && imgY[i][j] > imgY[i+1][j]))
				imgEdge[i][j] = 200;
		}

	// Write 3x3 Sobel Filter Images for X and Y Directions & Edge Detection Image
	FILE * output3Edge = fopen("output_edge_3X3.raw", "wb");
	FILE * output3X = fopen("output_3X3_X.raw", "wb");
	FILE * output3Y = fopen("output_3X3_Y.raw", "wb");
	fwrite(imgX, 1, h * w, output3X);
	fwrite(imgY, 1, h * w, output3Y);
	fwrite(imgEdge, 1, h * w, output3Edge);

	fclose(input);
	fclose(output3Edge);



	// 5x5 Sobel Filter
	
	minX = 10000, minY = 10000, maxX = -10000, maxY=-10000;

	for(int i = 2; i < h; i++) {
		for(int j = 2; j < w; j++) {
			short x = 0, y = 0;

			// Calculates 5x5 Derivative Approximations per Pixel
			for(short k = 0; k < 5; k++)
				for(short l = 0; l < 5; l++){ 
					x += img[i-2+k][j-2+l] * kernelFiveX[k][l];
					y += img[i-2+k][j-2+l] * kernelFiveY[k][l];
				}

			// Finds Min/Max Values
			if(x < minX)
				minX = x;
			else if (x > maxX)
				maxX = x;

			if(y < minY)
				minY = y;
			else if (y > maxY)
				maxY = y;

			tempX[i][j] = x;
			tempY[i][j] = y;
		}
	}

	// Min/Max Normailization
	for(int i = 0; i < h; i++)
		for(int j = 0; j < w; j++){
			float ratioX = (tempX[i][j] - minX) / (float)(maxX - minX);
			float ratioY = (tempY[i][j] - minY) / (float)(maxY - minY);
			// Uses Min/Max Normaalization Ratio's to Determine Pixel Vaule Offsets from 128
			if(ratioX >= 0.50000)
				imgX[i][j] = ratioX * 255;
			else
				imgX[i][j] = (1.0 - ratioX) * 255;
			if(ratioY >= 0.50000)
				imgY[i][j] = ratioY * 255;
			else
				imgY[i][j] = (1.0 - ratioY) * 255; 

			if (GRADIENT) {
				imgX[i][j] = sqrt(pow(imgX[i][j]-127,2) + pow(imgY[i][j]-127,2));
			}

			imgEdge[i][j] = 0;
		}

	// Edge Detection
	for(int i = 1; i < h-1; i++)
		for(int j = 1; j < w - 1; j++) {
			if((imgX[i][j] >= POSITIVE && imgX[i][j] > imgX[i][j-1] && imgX[i][j] > imgX[i][j+1]))
				imgEdge[i][j] = 200;
			if((imgY[i][j] >= POSITIVE && imgY[i][j] > imgY[i-1][j] && imgY[i][j] > imgY[i+1][j]))
				imgEdge[i][j] = 200;
		}
	
	// Write 5x5 Sobel Filter Images for X and Y Directions & Edge Detection Image
	FILE * output5Edge = fopen("output_edge_5X5.raw", "wb");
	FILE * output5X = fopen("output_5X5_X.raw", "wb");
	FILE * output5Y = fopen("output_5X5_Y.raw", "wb");
	fwrite(imgX, 1, h * w, output5X);
	fwrite(imgY, 1, h * w, output5Y);
	fwrite(imgEdge, 1, h * w, output5Edge);

	fclose(output5Edge);
	

	return 0;
}