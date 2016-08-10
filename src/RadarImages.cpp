#include "RadarImages.h"


RadarImages::RadarImages(int frs)
{
	num_images = 0;
	frame_size = frs;

	radar_images = new float*[1000];

 	radar_images_triangles = new vector<unsigned int>*[1000];
// 	radar_images_triangles = new unsigned int**[1000];

	all_scaled = false;

}

RadarImages::~RadarImages()
{
	for (int i = 0; i < num_images; i++)
	{
		delete[] radar_images[i];
		delete[] radar_images_triangles[i];
	}


	if (all_scaled)
	{
		for (int i = 0; i < num_scaled_images; i++)
		{
			delete[] scaled_radar_images[i];
		}
		delete[] scaled_radar_images;
	}

	delete[] scaled_image;
	delete[] radar_images;
	delete[] radar_images_triangles;
}

void RadarImages::scaleAllImages(int x, int y)
{

	// jeśli już były skalowane to trzeba usunąć wszystko i na nowo działać
	if (all_scaled)
	{
		for (int i = 0; i < num_images; i++)
			delete[] scaled_radar_images[i];
		delete[] scaled_image;
	}

	num_scaled_images = 0;
	scaled_radar_images = new float*[num_images];

	for (int i = 0; i < num_images; i++)
	{
		scaled_radar_images[i] = new float[x * y];
		scaleImage(x, y, i);
		for (int j = 0; j < x * y; j++)
			scaled_radar_images[i][j] = scaled_image[j];

	}

	all_scaled = true;
}

void RadarImages::scaleImage(int x, int y, int image_id)
{
	// po tej opracji przeskalowany obrazek siedzi w scaled_image.
	// nie jest dodawane nic do tabeli scaled_radar_images
	// (jedynie przy skalowaniu wszystkich na raz)

	delete[] scaled_image;
	delete[] scaled_image_triangles;


	double start = 0, stop = 0, scale, partial_sum;
	float x_scaled_image[x * frame_size], scaled_image_RGB[x * y * 3];
	vector<unsigned int> x_scaled_image_triangles[x * frame_size];
	int k = 0;

	scaled_image = new float[x * y];
	scaled_image_triangles = new vector<unsigned int>[x * y];


	//skalowanie w Xach

	scale = frame_size / double(x);
	for (int j = 0; j < frame_size; j++)	// jedzie po Yach
   	{
		start = 0;
		stop = scale;
		k = 0;
		while (stop <= frame_size)			// jedzie po Xach
	   	{
			//			printf("%f   %f", start, stop);
			partial_sum = 0;
			// to co pomiedzy
			for (int i = ceil(start); i < floor(stop); i++) {
				partial_sum
				   	+= radar_images[image_id][i + (j * frame_size)];

				// przekladanie indeksów trójkątów
				x_scaled_image_triangles[k + j * x].insert(
					x_scaled_image_triangles[k + j * x].end(),
					radar_images_triangles[image_id][i + (j*frame_size)].begin(),
					radar_images_triangles[image_id][i + (j*frame_size)].end()
					);


			}
			// to co z przodu i z tylu
			if ((start - floor(start)) != 0)
			{
				partial_sum += (ceil(start) - start)
				   	* radar_images[image_id][int(floor(start) + j *frame_size)];

				x_scaled_image_triangles[k + j * x].insert(
					x_scaled_image_triangles[k + j * x].end(),
					radar_images_triangles[image_id][int(floor(start)+ j*frame_size)].begin(),
					radar_images_triangles[image_id][int(floor(start)+ j*frame_size)].end()
					);
			}
			if (stop - floor(stop) != 0)
			{
				partial_sum += (stop - floor(stop))
					* radar_images[image_id][int(floor(stop) + j * frame_size)];

				x_scaled_image_triangles[k + j * x].insert(
					x_scaled_image_triangles[k + j * x].end(),
					radar_images_triangles[image_id][int(floor(stop)+ j*frame_size)].begin(),
					radar_images_triangles[image_id][int(floor(stop)+ j*frame_size)].end()
					);
			}

			x_scaled_image[k + j * x] = partial_sum;
			k++;
			start += scale;
			stop += scale;

		}
	}

// 	for (int l=0; l< x*frame_size; l++)
// 		if (x_scaled_image[l] <0 || x_scaled_image[l] > 8000)
// 		   	printf("%.2f\t",x_scaled_image[l]);
// 	puts("");

	//skalowanie w Ykach

	scale = frame_size / double(y);
	for (int j = 0; j < x; j++)
   	{
		start = 0;
		stop = scale;
		k = 0;
		while (stop <= frame_size)
	   	{
			//			printf("%f   %f", start, stop);
			partial_sum = 0;
			// to co pomiedzy
			for (int i = ceil(start); i < floor(stop); i++) {
				partial_sum += x_scaled_image[i * x + j];

				scaled_image_triangles[k * x + j].insert(
					scaled_image_triangles[k * x + j].end(),
					x_scaled_image_triangles[i * x + j].begin(),
					x_scaled_image_triangles[i * x + j].end()
					);
			}
			// to co z przodu i z tylu
			if ((start - floor(start)) !=0)
			{
				partial_sum += (ceil(start) - start)
					* x_scaled_image[int(floor(start) * x + j)];

				scaled_image_triangles[k * x + j].insert(
					scaled_image_triangles[k * x + j].end(),
					x_scaled_image_triangles[int(floor(start) * x + j)].begin(),
					x_scaled_image_triangles[int(floor(start) * x + j)].end()
					);
			}
			if (stop - floor(stop)  !=0)
			{
				partial_sum += (stop - floor(stop))
					* x_scaled_image[int(floor(stop) * x + j)];

				scaled_image_triangles[k * x + j].insert(
					scaled_image_triangles[k * x + j].end(),
					x_scaled_image_triangles[int(floor(stop) * x + j)].begin(),
					x_scaled_image_triangles[int(floor(stop) * x + j)].end()
					);
			}

			scaled_image[k * x + j] = partial_sum;
			k++;
			start += scale;
			stop += scale;
		}
	}


// 	if (print_to_file) {
		FILE *f;
		f = fopen("scaled.dat", "w");
// 		for (int j = y - 1; j >= 0; j--) {
		for (int j = 0; j < y; j++) {
			for (int i = 0; i < x ; i++) {
//				fprintf(f, "%d\t", int(scaled_image[j * x + i]));
				fprintf(f, "%.0f\t", (scaled_image[j * x + i]));
				//				fprintf(f, "%f\t", float(scaled_image[j * x + i]));// zliczenia, tylko int
			}
			fprintf(f, "\n");
		}
		fclose(f);
// 	}
/*
	if (render_onto_screen) {
		for (int i = 0; i < x * y; i++) {
			if (scaled_image[i] != 0) {
				scaled_image_RGB[i * 3] = scaled_image[i];
				scaled_image_RGB[i * 3 + 1] = 0;
				scaled_image_RGB[i * 3 + 2] = 0;
			} else {
				scaled_image_RGB[i * 3] = 1;
				scaled_image_RGB[i * 3 + 1] = 1;
				scaled_image_RGB[i * 3 + 2] = 1;
			}
		}
	}
*/
}

void RadarImages::createRadarImage(float *pixel_buffer_red, float *pixel_buffer_green,
		float *pixel_buffer_blue, float *pixel_buffer,
		unsigned int *pixel_buffer_uint)
{
	/**
	 * funkcja dodaje do tabeli **radar_images nowy obraz radarowy
	 * stworzony z podanyc tablic z danymi
	 */


	num_images++;
	radar_images[num_images - 1] = new float[frame_size * frame_size];

	// tworzy tablice wielkości frs*frs vektorów uns. int
	radar_images_triangles[num_images-1] =
		new vector<unsigned int>[frame_size*frame_size];

	int i, j;
	float ni, d, max;

	int radar_frame_size = frame_size;

	for (i = 0; i < radar_frame_size * radar_frame_size; i++)
		radar_images[num_images - 1][i] = 0;

	// UWAGA!! jest przestawione (red jest z lewej, blue z prawej,
	// nie ma odbicia lustrzanego z obrazem z gory)


	for (i = 0; i < radar_frame_size * radar_frame_size; i++)
   	{
		ni = pixel_buffer_red[i];
		d = pixel_buffer[i];

		int x = int((1 - ni) * radar_frame_size / 2);
		int y = int(d * radar_frame_size);

		if (d != 0 && ni != 0)
		{
			radar_images[num_images - 1][y*radar_frame_size + x]
			   	+= pixel_buffer_green[i];

			radar_images_triangles[num_images-1][y*radar_frame_size + x]
											.push_back( pixel_buffer_uint[i] );
		}
	}

	for (i = 0; i < radar_frame_size * radar_frame_size; i++) {
		ni = pixel_buffer_blue[i];
		d = pixel_buffer[i];

		int x = int(ni * radar_frame_size / 2 + radar_frame_size / 2);
		int y = int(d * radar_frame_size);

		if (d != 0 && ni != 0)
		{
			radar_images[num_images - 1][y*radar_frame_size + x]
				+= pixel_buffer_green[i];

			radar_images_triangles[num_images -1][y*radar_frame_size + x]
											.push_back( pixel_buffer_uint[i] );
		}
	}


	max = -1000;
	for (i = 1; i < radar_frame_size * radar_frame_size; i++) {
		if (radar_images[num_images - 1][i] > max)
			max = radar_images[num_images - 1][i];
	}
	printf("max: %f\n",max);
//	max = 10;
//	for (i = 0; i < radar_frame_size * radar_frame_size; i++) {
//		radar_image[i] /= max;
//	}

	// do wyświetlania
	//float radar_image_RGB[ (radar_frame_size*radar_frame_size*3)];
// 	if (render_onto_screen) {
// 		for (i = 0; i < (radar_frame_size * radar_frame_size); i++) {
// 			if (radar_image[i] != 0) {
// 				radar_image_RGB[i * 3] = radar_image[i];
// 				radar_image_RGB[i * 3 + 1] = 0;
// 				radar_image_RGB[i * 3 + 2] = 0;
// 			} else {
// 				radar_image_RGB[i * 3] = 1;
// 				radar_image_RGB[i * 3 + 1] = 1;
// 				radar_image_RGB[i * 3 + 2] = 1;
// 			}
// 		}
// 	}
// 	glUseProgram(NULL);
// 	if (render_onto_screen)
// 		glDrawPixels(radar_frame_size, radar_frame_size, GL_RGB, GL_FLOAT, radar_image_RGB);
//	scaled_image_size_x = 20;
//	scaled_image_size_y = 20;
//	scaleImage(scaled_image_size_x, scaled_image_size_y);


//	for (int i = 180141; i < 180160; i++)
//			printf("%.0f\t", radar_image[i]);
//	puts("po druku radar_image");

// 	number_of_radar_images+=1;
// 	radar_image_table[number_of_radar_images -1] = new float[frame_size * frame_size];
// 	for (int i = 0; i< frame_size*frame_size; i++) {
// 		radar_image_table[number_of_radar_images -1][i] = radar_image[i];
// 	}
//	printf("po kopiowaniu, %d\n", number_of_radar_images);

// 	/*
	FILE *f = fopen("out.dat", "w");
	for (int j = 0; j < frame_size; j++)
	{
		for(i = 0; i < frame_size; i++)
		{
			// 		if (radar_images[num_images - 1][i] != 0)
			fprintf(f,"%.0f ", radar_images[num_images - 1][j*frame_size + i]);
		}
		fprintf(f, "\n");
	}

	fclose(f);
// 	*/
}
