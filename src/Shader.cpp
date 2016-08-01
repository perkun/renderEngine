/*
 * Shader.cpp
 *
 *  Created on: 15 wrz 2015
 *      Author: grzeslaff
 */

#include "Shader.h"
#define GLM_FORCE_RADIANS
Shader::Shader() {
	// asdwd
}

Shader::Shader(const std::string& filename)
{
// 	RGB_value[0] = RGB_value[1] = RGB_value[2] = 1.;
	RGB_value = glm::vec3(1., 1., 1.);


	program = glCreateProgram();
	shaders[0] = createShader(loadShader(filename + ".vs"), GL_VERTEX_SHADER);
	shaders[1] = createShader(loadShader(filename + ".fs"), GL_FRAGMENT_SHADER);

	for (unsigned int i=0; i < NUM_SHADERS; i++)
		glAttachShader(program, shaders[i]);


	glBindAttribLocation(program, 0, "position");
	glBindAttribLocation(program, 1, "texCoord");
	glBindAttribLocation(program, 2, "normal");


	glLinkProgram(program);
	checkShaderError(program, GL_LINK_STATUS, true, "Error: Shader program linking failed");

	glValidateProgram(program);
	checkShaderError(program, GL_VALIDATE_STATUS, true, "Error: Shader program is invalid");


	uniforms[MODEL_U] = glGetUniformLocation(program, "model_matrix");
	uniforms[VIEW_U] = glGetUniformLocation(program, "view_matrix");
	uniforms[PROJECTION_U] = glGetUniformLocation(program, "projection_matrix");
	uniforms[LIGHT_VIEW_U] = glGetUniformLocation(program, "light_view_matrix");
	uniforms[LIGHT_PROJECTION_U] = glGetUniformLocation(program, "light_projection_matrix");
	uniforms[LIGHT_POSITION_U] = glGetUniformLocation(program, "light_position");

	uniforms[RGB_VALUE_U] = glGetUniformLocation(program, "RGB_value");

	// dodać uniformy dla angles, view angles,
	// oraz maximum length? (czy normalizować model zawsze?)
	uniforms[NO_TRNSLATION_MODEL_U] = glGetUniformLocation(program,
			"no_translation_model_matrix");
	uniforms[NO_TRNSLATION_VIEW_U] = glGetUniformLocation(program,
			"no_translation_view_matrix");
}

Shader::~Shader()
{
	for (unsigned int i=0; i < NUM_SHADERS; i++) {
		glDetachShader(program, shaders[i]);
		glDeleteShader(shaders[i]);
	}

	glDeleteProgram(program);
}

void Shader::bind()
{
	glUseProgram(program);
}

void Shader::update(Transform& transform, const Camera& camera, const Camera& light)
{
 	glm::mat4 model_matrix = transform.getModelMatrix();
	glm::mat4 view_matrix = camera.getViewMatrix();
	glm::mat4 projection_matrix = camera.getProjectionMatrix();
	glm::mat4 light_view_matrix = light.getViewMatrix();
	glm::mat4 light_projection_matrix = light.getProjectionMatrix();

	float lp[3];
	lp[0] = light.position.x; lp[1] = light.position.y; lp[2] = light.position.z;

	glUniformMatrix4fv(uniforms[MODEL_U], 1, GL_FALSE, &model_matrix[0][0]);
	glUniformMatrix4fv(uniforms[VIEW_U], 1, GL_FALSE, &view_matrix[0][0]);
	glUniformMatrix4fv(uniforms[PROJECTION_U], 1, GL_FALSE, &projection_matrix[0][0]);
	glUniformMatrix4fv(uniforms[LIGHT_VIEW_U], 1, GL_FALSE, &light_view_matrix[0][0]);
	glUniformMatrix4fv(uniforms[LIGHT_PROJECTION_U], 1, GL_FALSE, &light_projection_matrix[0][0]);
	glUniform3fv(uniforms[LIGHT_POSITION_U], 1, lp);

// 	float dupa[3] = {0.,0., 1.};
// 	glUniform3fv(uniforms[RGB_VALUE_U], 1, dupa);
// 	glm::vec3 dupa = glm::vec3(0.,0.,1.);
	glUniform3fv(uniforms[RGB_VALUE_U], 1, (float*) &RGB_value);
}

void Shader::updateRadar(Transform& transform, const Camera& camera,
	   	const Camera& light)
{
 	glm::mat4 model_matrix = transform.getModelMatrix();
	glm::mat4 view_matrix = camera.getViewMatrix();
	glm::mat4 projection_matrix = camera.getProjectionMatrix();
	glm::mat4 light_view_matrix = light.getViewMatrix();
	glm::mat4 light_projection_matrix = light.getProjectionMatrix();

	glm::mat4 no_translation_model_matrix =
		transform.getNoTranslationModelMatrix();
	glm::mat4 no_translation_view_matrix =
		camera.getNoTranslationViewMatrix();


	float lp[3];
	lp[0] = light.position.x; lp[1] = light.position.y; lp[2] = light.position.z;

	glUniformMatrix4fv(uniforms[MODEL_U], 1, GL_FALSE, &model_matrix[0][0]);
	glUniformMatrix4fv(uniforms[VIEW_U], 1, GL_FALSE, &view_matrix[0][0]);
	glUniformMatrix4fv(uniforms[PROJECTION_U], 1, GL_FALSE, &projection_matrix[0][0]);
	glUniformMatrix4fv(uniforms[LIGHT_VIEW_U], 1, GL_FALSE, &light_view_matrix[0][0]);
	glUniformMatrix4fv(uniforms[LIGHT_PROJECTION_U], 1, GL_FALSE, &light_projection_matrix[0][0]);
	glUniform3fv(uniforms[LIGHT_POSITION_U], 1, lp);

	glUniformMatrix4fv(uniforms[NO_TRNSLATION_MODEL_U], 1, GL_FALSE,
			&no_translation_model_matrix[0][0]);
	glUniformMatrix4fv(uniforms[NO_TRNSLATION_VIEW_U], 1, GL_FALSE,
			&no_translation_view_matrix[0][0]);
}


void Shader::updateModelMatrix(Transform& transform)
{
	glm::mat4 model_matrix = transform.getModelMatrix();
	glUniformMatrix4fv(uniforms[MODEL_U], 1, GL_FALSE, &model_matrix[0][0]);
}

std::string Shader::loadShader(const std::string& filename)
{
//	const std::string& filename2 = "basicShader.vs";

	std::ifstream file;
	file.open((filename).c_str());

	std::string output;
	std::string line;

	if (file.is_open()) {
		while (file.good()) {
			getline(file, line);
			output.append(line + "\n");
		}
	} else {
		std::cerr << "Unable to load shader: " << filename << std::endl;
	}

	return output;
}

void Shader::checkShaderError(GLuint shader, GLuint flag, bool isProgram,
		const std::string& errorMessage)
{
	GLint success = 0;
	GLchar error[1024] = { 0 };

	if (isProgram)
		glGetProgramiv(shader, flag, &success);
	else
		glGetShaderiv(shader, flag, &success);

	if (success == GL_FALSE) {
		if (isProgram)
			glGetProgramInfoLog(shader, sizeof(error), NULL, error);
		else
			glGetShaderInfoLog(shader, sizeof(error), NULL, error);

		std::cerr << errorMessage << ": '" << error << "'" << std::endl;
	}
}

GLuint Shader::createShader(const std::string& text, unsigned int type)
{
	GLuint shader = glCreateShader(type);

	if (shader == 0)
		std::cerr << "Error compiling shader type " << type << std::endl;

	const GLchar* p[1];
	p[0] = text.c_str();
	GLint lengths[1];
	lengths[0] = text.length();

	glShaderSource(shader, 1, p, lengths);
	glCompileShader(shader);

	checkShaderError(shader, GL_COMPILE_STATUS, false,
			"Error compiling shader!");

	return shader;
}
