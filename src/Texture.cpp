/*
 * Texture.cpp
 *
 *  Created on: 16 wrz 2015
 *      Author: grzeslaff
 */



#include "Texture.h"

#define GLM_FORCE_RADIANS


Texture::Texture() {
	//sdaw
}

Texture::Texture(int w, int h) {
	color_texture_width = w;
	color_texture_height = h;
	depth_texture_width = w;
	depth_texture_height = h;
	
	createColorTexture();
	createDepthTexture();
	
	attachToCurrentFramebuffer();
}
/*
Texture::Texture(const std::string& filename) {
// 	shadow_size ;
	createColorTexture(filename);
	createDepthTexture();
	
	attachToCurrentFramebuffer();
}
*/

void Texture::createColorTexture() {
	glGenTextures(1, &color_texture);
	
	glActiveTexture(GL_TEXTURE0 + num_textures);			// uwaga na numerki!
	color_texture_unit = num_textures;
	num_textures++;
	
	glBindTexture(GL_TEXTURE_2D, color_texture);
	
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, color_texture_width, color_texture_height);
// 	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, color_texture_width, color_texture_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
}
/*
void Texture::createColorTexture(const std::string& filename) {
	int width, height, num_components;

	unsigned char* image_data = stbi_load(filename.c_str(), &width, &height, &num_components, 4);

	color_texture_width = width;
	color_texture_height = height;
	depth_texture_width = width;
	depth_texture_height = height;
	
	if(image_data == NULL)
		std::cerr << "Texture loading failed" << std::endl;


	glGenTextures(1, &color_texture);
	
	glActiveTexture(GL_TEXTURE0 + num_textures);			// uwaga na numerki!
	color_texture_unit = num_textures;
	num_textures++;
	
	glBindTexture(GL_TEXTURE_2D, color_texture);

		
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);



	stbi_image_free(image_data);
}
*/

void Texture::createDepthTexture() {
	glGenTextures(1, &depth_texture);
	
	glActiveTexture(GL_TEXTURE0 + num_textures);			// uwaga na numerki!
	depth_texture_unit = num_textures;
	num_textures++;
	
	glBindTexture(GL_TEXTURE_2D, depth_texture);
	
	glTexImage2D (
		GL_TEXTURE_2D,
		0,
		GL_DEPTH_COMPONENT32,
		depth_texture_width,
		depth_texture_height,
		0,
		GL_DEPTH_COMPONENT,
		GL_UNSIGNED_BYTE,
		NULL
	);
	
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	// clamp to edge.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}


void Texture::bind() {
	glActiveTexture(GL_TEXTURE0 + depth_texture_unit);
	glBindTexture(GL_TEXTURE_2D, depth_texture);

	glActiveTexture(GL_TEXTURE0 + color_texture_unit);
	glBindTexture(GL_TEXTURE_2D, color_texture);
	
// 	GLuint clearColor = 0;
// // 	glBindTexture(GL_TEXTURE_2D);
// 	glClearTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, &clearColor);
	
	
// 	GLenum draw_buffers[1] = { GL_COLOR_ATTACHMENT0 };
// 	glDrawBuffers (1, draw_buffers);
	
	
	
	
	

// 	GLenum draw_bufs[] = { GL_NONE };
// 	glDrawBuffers (1, draw_bufs);
// 	

}

void Texture::attachToCurrentFramebuffer()
{
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, color_texture, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_texture, 0);
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void Texture::setUniform(Shader &shader, const char* color_name, const char* depth_name) {
	
	shader.bind();
	
	color_texture_location = glGetUniformLocation(shader.program, color_name);
	glUniform1i(color_texture_location, color_texture_unit);
	
	depth_texture_location = glGetUniformLocation(shader.program, depth_name);
	glUniform1i(depth_texture_location, depth_texture_unit);
	
}


Texture::~Texture() {
	glDeleteTextures(1, &color_texture);
	glDeleteTextures(1, &depth_texture);
}



