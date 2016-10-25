/*
 * Image.cpp
 *
 *  Created on: 7 cze 2015
 *      Author: grzeslaff
 */

#include "Image.h"
#define GLM_FORCE_RADIANS
Image::Image(int w, int h, int bpp) {
	width = w;
	height = h;
	FreeImage_Initialise();

	bitmap = FreeImage_Allocate(width, height, bpp);




}

Image::~Image() {
	FreeImage_DeInitialise();
}

//void Image::saveImage(float *pixel_buffer , int step) {
//	for (int i = 0; i< width; i++) {
//		for (int j = 0; j < height; j++) {
//			color.rgbRed = 	color.rgbGreen = color.rgbBlue = pixel_buffer[j*width + i]*255.;
//			FreeImage_SetPixelColor(bitmap, i, j, &color);
//		}
//	}
//
//	char name[20], step_str[10];
//	sprintf(name, "./%d.png", step);
//
//
//	FreeImage_Save(FIF_PNG, bitmap, name, 0);
//}

void Image::saveImage(float *pixel_buffer , int image_id)
{
	for (int i = 0; i< width; i++) {
		for (int j = 0; j < height; j++) {
			color.rgbRed =
				color.rgbGreen = color.rgbBlue = pixel_buffer[j*width + i]*255.;
			FreeImage_SetPixelColor(bitmap, i, j, &color);
		}
	}

	char name[100];//step_str[10];
	sprintf(name, "./film/%d.png", image_id);


	FreeImage_Save(FIF_PNG, bitmap, name, 0);
}

void Image::saveImageRgb(float *pixel_buffer_r, float *pixel_buffer_g,
		float *pixel_buffer_b, int image_id)
{
	for (int i = 0; i< width; i++) {
		for (int j = 0; j < height; j++) {
			color.rgbRed = pixel_buffer_r[j*width + i]*255.;
			color.rgbGreen =  pixel_buffer_g[j*width + i]*255.;
			color.rgbBlue = pixel_buffer_b[j*width + i]*255.;

			FreeImage_SetPixelColor(bitmap, i, j, &color);
		}
	}

	char name[100];//step_str[10];
	sprintf(name, "./film/%d.png", image_id);


	FreeImage_Save(FIF_PNG, bitmap, name, 0);
}

void Image::saveImageRgb(float *pixel_buffer_r, float *pixel_buffer_g,
		float *pixel_buffer_b, const char *filename)
{
	for (int i = 0; i< width; i++) {
		for (int j = 0; j < height; j++) {
			color.rgbRed = pixel_buffer_r[j*width + i]*255.;
			color.rgbGreen =  pixel_buffer_g[j*width + i]*255.;
			color.rgbBlue = pixel_buffer_b[j*width + i]*255.;

			FreeImage_SetPixelColor(bitmap, i, j, &color);
		}
	}

	char name[100];//step_str[10];
	sprintf(name, "./%s.png", filename);


	FreeImage_Save(FIF_PNG, bitmap, name, 0);
}

