#ifndef IMAGE_H_
#define IMAGE_H_

#include <FreeImage.h>
#include <string.h>
#include <stdio.h>
#define GLM_FORCE_RADIANS
class Image {
public:
	int width, height;
	FIBITMAP* bitmap;
	RGBQUAD color;

	Image(int, int, int);
	void saveImage(float*, int);
	void saveImageRgb(float*, float*, float*, int);
	void saveImageRgb(float*, float*, float*, const char*);
	virtual ~Image();

};

#endif /* IMAGE_H_ */
