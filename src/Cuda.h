#ifndef CUDA_H_
#define CUDA_H_
#define CUDA_VERSION 8000
// #define GLM_COMPILER 0

#include <iostream>
#include <fstream>
#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <cmath>

#include <cuda.h>
// #include <cuda_runtime.h>
#include <cuda_gl_interop.h>


class Cuda {
public:
	Cuda(GLuint &tex, int w, int h);
	~Cuda();

	void copyResult();
	float calculateMagnitudes();

	float *dev_result, result;
	struct cudaGraphicsResource *vbo_res;
	cudaChannelFormatDesc channelDesc;
	cudaArray *array_img;
	float *dev_blocks_partials;
	int threads_per_block;
	int num_blocks, initial_num_blocks;
	int width, height;
	GLuint texture;
};
















#endif /* CUDA_H_ */
