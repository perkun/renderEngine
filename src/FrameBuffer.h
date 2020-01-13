#ifndef FRAMEBUFFER_H_
#define FRAMEBUFFER_H_

#include <GL/glew.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include<X11/X.h>
#include<X11/Xlib.h>
#include<GL/gl.h>
#include<GL/glx.h>
// #include<GL/glu.h>
#define GLM_FORCE_RADIANS
class FrameBuffer {
public:
	FrameBuffer(int, int);
	FrameBuffer();
	FrameBuffer(const FrameBuffer& other) {}
	~FrameBuffer();

	void bind();
	void clear(float, float, float, float);
private:
// 	void bindDepthTexture(unsigned int unit);
// 	void bindColorTexture(unsigned int unit);

public:
	GLuint frame_buffer;
// 	GLuint color_texture;
	GLuint depth_texture;
	GLint color_texture_location, depth_texture_location;
	int width, height; // shadow_size;
// 	GLenum draw_buffers[1];// = { GL_COLOR_ATTACHMENT0};
	GLenum draw_buffers_depth[1];// = {GL_NONE };
};

#endif /* FRAMEBUFFER_H_ */
