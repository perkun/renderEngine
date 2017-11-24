#include "FrameBuffer.h"
#define GLM_FORCE_RADIANS
FrameBuffer::FrameBuffer() {

}

FrameBuffer::FrameBuffer(int w, int h) {
	draw_buffers[0] = GL_COLOR_ATTACHMENT0;
	draw_buffers_depth[1] = GL_NONE;

	width = w;
	height = h;
// 	shadow_size = width;

	frame_buffer = 0;
	color_texture = 0;

	glGenFramebuffers(1, &frame_buffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);


	//glDrawBuffers(1, draw_buffers);
	glDrawBuffers(1, draw_buffers_depth);

// 	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
// 		puts("cos nie tak z framebufferem");

}

void FrameBuffer::bind() {

	glViewport(0, 0, width, height);
	glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);
// 	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}

void FrameBuffer::clear(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}



void FrameBuffer::bindDepthTexture(unsigned int unit) {
	glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);
 	glActiveTexture (GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, depth_texture);
 	glDrawBuffers(1, draw_buffers_depth);

}
void FrameBuffer::bindColorTexture(unsigned int unit) {
	glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);
	glActiveTexture (GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, color_texture);
	glDrawBuffers(1, draw_buffers);
}

FrameBuffer::~FrameBuffer() {
	glDeleteTextures(1, &color_texture);
	glDeleteTextures(1, &depth_texture);
}

