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
	target = GL_TEXTURE_2D;

	color_texture_width = w;
	color_texture_height = h;
	depth_texture_width = w;
	depth_texture_height = h;

	createColorTexture();
	createDepthTexture();

	attachToCurrentFramebuffer();
}

Texture::Texture(int w, int h, GLenum tar, GLvoid *data,  GLint internal_format,
		GLenum format, GLenum type)
{
	target = tar;

	color_texture_width = w;
	color_texture_height = h;
	depth_texture_width = w;
	depth_texture_height = h;

	createColorTexture(target, data, internal_format, format, type);
 	createDepthTexture();

	attachToCurrentFramebuffer();
}

Texture::Texture(const std::string& filename) {
// 	shadow_size ;
	target = GL_TEXTURE_2D;
	createColorTexture(filename);
	createDepthTexture();

	attachToCurrentFramebuffer();
}


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

	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, color_texture_width,
			color_texture_height);
// 	 	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, color_texture_width,
// 	 	color_texture_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
}

void Texture::createColorTexture(GLenum target, GLvoid *data,
		GLint internal_format, GLenum format, GLenum type)
{

	/** można tu wpierdolić tekstórę jakiegokolwiek typu,
	 * manipuować można formatem wew., formatem i typem
	 * 	  np dla tekstury rgba z unsigned int
	 * 		internal format = GL_RGBA32UI
	 *		format			= GL_RGBA_INTEGER
	 *		type			= GL_UNSIGNED_INT
	 *
	 *	nadal są takie parametry jak GL_REPEAT, WRAP czy access GL_READ_WRITE
	 *
	 *	tutaj w odróżniniu od createColorTexture(nic) jest z teksturą zbindowany
	 *	image (glBindImageTexture(...)), a nie Storage.
	 *	Tak samo jak tu jest w createDepthTExture() akczyli
	 *
	 *	a tu jest przykladzik uzycia unsigned char do kodowani binarnego kolorow
	 *
	 *	GLuint* data2 = new GLuint[256*256]; //Use OpenGL's types
	 *		for (int y = 0; y < 256; ++y)
	 *			for (int x = 0; x < 256; ++x)
	 * 				if ((x - 100)*(x - 100) + (y - 156)*(y - 156) < 75*75)
	 * 					data2[256*y + x]
	 * 				   	= ((0x9C << 24) | (0xFF << 16) | (0x9C << 8) | (0xC8 << 0));
	 * 				else
	 * 					data2[256*y + x] = 0; // I'd expect this to be transparent
	 * 										  // and the above to be slightly
	 * 										  // transparent and green, but it's red
	 * 										  // somehow.
	 * 			// albo
 	 *			// data2[(256*y + x)*4+0] = 255;
	 *
	*/


	glGenTextures(1, &color_texture);

	glActiveTexture(GL_TEXTURE0 + num_textures);			// uwaga na numerki!
	color_texture_unit = num_textures;
	num_textures++;

// 	glBindTexture(GL_TEXTURE_2D, color_texture);
	glBindTexture(target, color_texture);

	glBindImageTexture(
			color_texture_unit,				// unit
		   	color_texture,					// texture
		   	0,								// level
		   	GL_FALSE,						// layered
		   	0,								// layer
			GL_READ_WRITE,					// access
		   	internal_format);				// format vel internal format
   											// (np GL_RGBA32UI)
	if (target == GL_TEXTURE_2D)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
// 	if (target == GL_TEXTURE_1D)
// 		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);


 		glTexParameterf(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
 		glTexParameterf(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

// 	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, color_texture_width,
// 			color_texture_height);
	if (target == GL_TEXTURE_1D)
		glTexImage1D(
				GL_TEXTURE_1D,				// target
				0,							// level
				internal_format,			// internal format(np GL_RGBA32UI)
				color_texture_width,		// witdh
				0,							// border
				format,						// format (np GL_RGBA_INTEGER)
				type,						// type (np GL_UNSIGNED_INT)
				(GLvoid*) data);			// data


	if (target == GL_TEXTURE_2D)
		glTexImage2D(
				GL_TEXTURE_2D,				// target
				0,							// level
				internal_format,			// internal format(np GL_RGBA32UI)
				color_texture_width,		// witdh
				color_texture_height,		// height
				0,							// border
				format,						// format (np GL_RGBA_INTEGER)
				type,						// type (np GL_UNSIGNED_INT)
				(GLvoid*) data);			// data
	// 	delete[] data;
}

void Texture::createColorTexture(const std::string& filename)
{
	int width, height, num_components;

	unsigned char* image_data = stbi_load(filename.c_str(), &width, &height,
			&num_components, 4);

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

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
			GL_UNSIGNED_BYTE, image_data);



	stbi_image_free(image_data);
}


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
// 	if (target == GL_TEXTURE_2D)
// 	{
		glActiveTexture(GL_TEXTURE0 + depth_texture_unit);
		glBindTexture(GL_TEXTURE_2D, depth_texture);
// 	}
		glActiveTexture(GL_TEXTURE0 + color_texture_unit);
		glBindTexture(target, color_texture);

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

// 	if (target == GL_TEXTURE_2D)
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
				GL_TEXTURE_2D, depth_texture, 0);



	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void Texture::setUniform(Shader &shader, const char* color_name,
		const char* depth_name)
{

	shader.bind();

	color_texture_location = glGetUniformLocation(shader.program, color_name);
	glUniform1i(color_texture_location, color_texture_unit);

// 	if (target == GL_TEXTURE_2D)
// 	{
		depth_texture_location = glGetUniformLocation(shader.program, depth_name);
		glUniform1i(depth_texture_location, depth_texture_unit);
// 	}

}


Texture::~Texture() {
	glDeleteTextures(1, &color_texture);
// 	if (target == GL_TEXTURE_2D)
		glDeleteTextures(1, &depth_texture);
}



