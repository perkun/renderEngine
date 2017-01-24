/*
 * Shader.h
 *
 *  Created on: 15 wrz 2015
 *      Author: grzeslaff
 */

#ifndef SHADER_H_
#define SHADER_H_

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "Transform.h"
#include "Camera.h"
//#include "transform.h"

#define GLM_FORCE_RADIANS

class Shader {
public:
	GLuint program;

	Shader();
	Shader(const Shader& other) {}
	Shader(const std::string& filename);
	virtual ~Shader();

	void bind();
	void update(Transform& transform, const Camera& camera, const Camera& light);
	void update(Transform& transform, const Camera& camera, const Camera& light,
			int model_id);
	void update(const Camera& camera, const Camera& light);
// 	void sendUniformArrays(int array_size);
	void updateRadar(Transform& transform, const Camera& camera, const Camera& light);
// 	void update(int model_index, int camera_index, int light_index);
	void updateModelMatrix(Transform& transform);

//private:
	std::string loadShader(const std::string& filename);
	void checkShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);
	GLuint createShader(const std::string& text, unsigned int type);

	// można tu dadać jakąś tablice, w której można przesyłać jakieś parametry,
	// podobnie jak np RGB_VALUE_U
	enum {
		MODEL_U,
		VIEW_U,
		PROJECTION_U,
		LIGHT_VIEW_U,
		LIGHT_PROJECTION_U,
		LIGHT_POSITION_U,
		RGBA_VALUE_U,
		NO_TRNSLATION_MODEL_U,
		NO_TRNSLATION_VIEW_U,
		MODEL_ID,

		NUM_UNIFORMS
	};

	static const unsigned int NUM_SHADERS = 2;
	glm::vec4 RGBA_value;

	GLuint shaders[NUM_SHADERS];
	GLuint uniforms[NUM_UNIFORMS];


};

#endif /* SHADER_H_ */
