#include "Cuda.h"

#define GLM_FORCE_RADIANS

texture<uchar4, 2, cudaReadModeNormalizedFloat> tex_img;

__global__ void kernel(float *result, float *result2, float *result3) {
	int x, y, width = 1000, height = 1000;
	*result = -7.;
	*result2 = -8.;
	*result3 = -22.;
	printf("res3: %f\n", *result3);
	printf("res2: %f\n", *result2);
	
	*result3 = 0;
	for ( x = 0; x < width; x++)
		for ( y = 0; y < height; y++)
			*result3 += tex2D(tex_img, x + 0.5f, y + 0.5f).x;
		
		printf("res3_2: %f\n", *result3);
	
}

__global__ void sumA(float *blocks_partials, int width, int height) {
    //     int dim = blockDim.x;
    __shared__ float partials[512];
    int texture_x, texture_y;
    int global_id = blockIdx.x * blockDim.x + threadIdx.x;
    int local_id = threadIdx.x;
    int i;

    if (global_id < width * height) {

        texture_x = global_id%height;
        texture_y = (int) (global_id/height);

// 	if (local_id == 0)
// 		blocks_partials[blockIdx.x] = 0;

        // kopia textury lokalnie dla bloku (shared mem)
        partials[local_id] = tex2D(tex_img, texture_x + 0.5f, texture_y + 0.5f).x;

        for (i = 1; i <= blockDim.x/2; i *= 2)
            if (local_id%(i*2) == 0) {
                partials[local_id] += partials[local_id + i];
                __syncthreads();
            }

        if (local_id == 0)
            blocks_partials[blockIdx.x] = partials[0];
    }
}

__global__ void sumB(float *blocks_partials, float *suma, int size) {
	int global_id = blockIdx.x * blockDim.x + threadIdx.x;
	int local_id = threadIdx.x;
	int i;
	
	if (local_id < 128)	{
	
		
	__shared__ float partials[1024];
	partials[local_id] = blocks_partials[global_id];
	
	
	// 	if (id < size) {
	for (i = 1; i <= blockDim.x/2; i *= 2)
		if (local_id%(i*2) == 0) {
			partials[local_id] += partials[local_id + i];
			__syncthreads();
		}
		// 	}
		
		if (local_id == 0)
			blocks_partials[blockIdx.x] = partials[0];
		
		// 	if (global_id == 0)
		// 		*suma = blocks_partials[0];
	}
}


Cuda::Cuda(GLuint &tex, int w, int h) {
	width = w;
	height = h;
	texture = tex;
	
	cudaMalloc( (void**)&dev_result, sizeof(float) );
	
// 	glBindFramebuffer(GL_FRAMEBUFFER, 0);
// 	cudaError err = cudaGraphicsGLRegisterImage(&vbo_res, texture, GL_TEXTURE_2D, cudaGraphicsRegisterFlagsReadOnly);
// 	if ( err == cudaSuccess)
// 		printf("image successfully registered\n");
// 	else if ( err == cudaErrorInvalidResourceHandle)
// 		printf("error registering image, Invalid Handle (duplicate entry)\n");
// 	else if (err == cudaErrorUnknown)
// 		printf("error registering image (maped for access)\n");
	
	channelDesc = cudaCreateChannelDesc(8, 8, 8, 8, cudaChannelFormatKindUnsigned);
	cudaMallocArray( &array_img, &channelDesc, w, h );

	
	threads_per_block = 128;//(DISPLAY_WIDTH / 128) + 1;	// musi być 128*natural_number
	initial_num_blocks =  ceil(   (float)(w*h) / (float)threads_per_block   ); 				// musi być 2^N
// 	printf("num blocks: %d\n", init_num_blocks);
	
	cudaMalloc( (void**)&dev_blocks_partials, sizeof(float)* initial_num_blocks );
	
	
	tex_img.addressMode[0] = cudaAddressModeBorder;
	tex_img.addressMode[1] = cudaAddressModeBorder;
	tex_img.filterMode = cudaFilterModeLinear;
	tex_img.normalized = false; // don't access with normalized texture coords
	
	
// 	cudaGraphicsMapResources(1, &vbo_res, 0);
// 	cudaGraphicsSubResourceGetMappedArray(&array_img, vbo_res, 0, 0);
// 	cudaBindTextureToArray(tex_img, (cudaArray *)array_img, channelDesc);
// 	if (cudaGraphicsMapResources(1, &vbo_res, 0) != cudaSuccess) printf("error maping resources\n");
// 	err = cudaGraphicsMapResources(1, &vbo_res, 0);
// 	if ( err == cudaSuccess)
// 		printf("image successfully maped\n");
// 	else if ( err == cudaErrorInvalidResourceHandle)
// 		printf("error maping image, Invalid Handle (duplicate entry)\n");
// 	else if (err == cudaErrorUnknown)
// 		printf("error maping image (maped for access)\n");

}

float Cuda::calculateMagnitudes() {
// 	cudaMalloc( (void**)&dev_blocks_partials, sizeof(float)* initial_num_blocks );
// 	cudaMallocArray( &array_img, &channelDesc, width, height );
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	cudaError err = cudaGraphicsGLRegisterImage(&vbo_res, texture, GL_TEXTURE_2D, cudaGraphicsRegisterFlagsReadOnly);
	if ( err == cudaErrorInvalidResourceHandle)
		printf("error registering image, Invalid Handle (duplicate entry)\n");
	else if (err == cudaErrorUnknown)
		printf("error registering image (maped for access)\n");
	
// 	cudaMallocArray( &array_img, &channelDesc, width, height );
	
// 	if (cudaGraphicsMapResources(1, &vbo_res, 0) != cudaSuccess) printf("error maping resources\n");
	err = cudaGraphicsMapResources(1, &vbo_res, 0);
	if ( err == cudaErrorInvalidResourceHandle)
		printf("error maping image, Invalid Handle (duplicate entry)\n");
	else if (err == cudaErrorUnknown)
		printf("error maping image (maped for access)\n");
	
	if (cudaGraphicsSubResourceGetMappedArray(&array_img, vbo_res, 0, 0) != cudaSuccess) printf("error getting mapped array\n");
	if (cudaBindTextureToArray(tex_img, (cudaArray *)array_img, channelDesc) != cudaSuccess) printf("error binding to array\n");
	
	
	num_blocks =  initial_num_blocks; 
	sumA<<< num_blocks, threads_per_block >>>(dev_blocks_partials, width, height);
	cudaThreadSynchronize();
	while (num_blocks > 1) {
		num_blocks = ceil((float)num_blocks/(float)threads_per_block);
// 		printf("number of blocks: %d\n", num_blocks);
		sumB<<< num_blocks, threads_per_block >>>(dev_blocks_partials, dev_result, num_blocks);
		cudaThreadSynchronize();
	}
	
	
	copyResult();
	
	cudaUnbindTexture(tex_img);
	cudaGraphicsUnmapResources(1, &vbo_res, 0);
	cudaGraphicsUnregisterResource(vbo_res);
	
// 	cudaFreeArray(array_img);
// 	cudaFree(dev_blocks_partials);
	
	return -2.5*log10(result);
}


void Cuda::copyResult() {
	cudaMemcpy(&result, &dev_blocks_partials[0], sizeof(float), cudaMemcpyDeviceToHost);
// 	printf("sum of block partials (DEVICE): %f\n", result);
}


Cuda::~Cuda() {
	cudaUnbindTexture(tex_img);
	cudaFree(dev_blocks_partials);
	cudaFree(dev_result);
	//cudaFreeArray(array_img);
}