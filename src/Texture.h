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
	static int num_textures;
	int color_texture_unit, depth_texture_unit;
	GLuint color_texture, depth_texture;


	Texture(int, int);
	Texture(int, int, GLuint*);
	Texture(const std::string& filename);

	Texture();
	Texture(const Texture& other) {}

	void bind();


// 	Texture(int mode);
	void createDepthTexture();
	void createColorTexture(const std::string& filename);
	void createColorTexture();
	void createColorTexture(GLuint*);
	void setUniform(Shader& shader, const char* color_name, const char* depth_name);

	virtual ~Texture();

private:
	int depth_texture_height, depth_texture_width, color_texture_width, color_texture_height;
	GLuint color_texture_location, depth_texture_location;


	void attachToCurrentFramebuffer();



};







#endif /* TEXTURE_H_ */
