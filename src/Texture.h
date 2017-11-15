/*
 * Texture.h
 *
 *  Created on: 16 wrz 2015
 *      Author: grzeslaff
 */

#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <string>
#include <iostream>
#include <GL/glew.h>
#include "stb_image.h"
#include <string.h>
#include "Shader.h"


#define GLM_FORCE_RADIANS

class Texture {
public:
	Texture(int, int);
	Texture(int, int, GLenum, GLvoid*,  GLint internal_format, GLenum format,
			GLenum type);
	Texture(const std::string& filename);
	Texture();
	Texture(const Texture& other) {}
	virtual ~Texture();

	void bind();
	void createDepthTexture();
	void createColorTexture(const std::string& filename);
	void createColorTexture();
	void createColorTexture(GLenum, GLvoid*, GLint internal_format,
			GLenum format, GLenum type);
	void setUniform(Shader& shader, const char* color_name,
		   	const char* depth_name);

	static int num_textures;
	int color_texture_unit, depth_texture_unit;
	GLuint color_texture, depth_texture;
	GLenum target;

private:
	int depth_texture_height, depth_texture_width, color_texture_width,
		color_texture_height;
	GLuint color_texture_location, depth_texture_location;


	void attachToCurrentFramebuffer();



};







#endif /* TEXTURE_H_ */
