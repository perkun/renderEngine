#ifndef RADARIMAGES_H_
#define RADARIMAGES_H_

#include <iostream>
#include <stdio.h>
#include <math.h>
#include <vector>

using namespace std;

class RadarImages
{
public:
	RadarImages(int frs);
	~RadarImages();
	void createRadarImage(float *pixel_buffer_red, float *pixel_buffer_green,
			float *pixel_buffer_blue, float *pixel_buffer,
			unsigned int *pixel_buffer_uint);
	void scaleImage( int x, int y, int image_id);
	void scaleAllImages(int x, int y);


	int frame_size;
	int num_images, num_scaled_images;

	float **radar_images, **scaled_radar_images;
	float *scaled_image;

	vector<unsigned int> **radar_images_triangles,
		**scaled_radar_images_triangles;
	vector<unsigned int> *scaled_image_triangles;


	bool all_scaled;

};

#endif /* RADARIMAGES_H_ */
