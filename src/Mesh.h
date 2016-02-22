/*
 * Mesh.h
 *
 *  Created on: 15 wrz 2015
 *      Author: grzeslaff
 */

#ifndef MESH_H_
#define MESH_H_

#define GLM_FORCE_RADIANS

#include <stdio.h>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <iostream>
#include <vector>
#include "obj_loader.h"
#include "Transform.h"

class Vertex {
public:

	glm::vec3 pos;
	glm::vec2 tex_coord;
	glm::vec3 normal;

	Vertex(const glm::vec3& pos, const glm::vec2& tex_coord, const glm::vec3 &normal = glm::vec3(0,0,0) ) {
		this -> pos = pos;
		this -> tex_coord = tex_coord;
		this -> normal = normal;
	}

//	glm::vec3* GetPos() { return &pos; }
protected:
private:

};




class Mesh {
public:
	static bool normalize, normal_interpolation;
	GLenum draw_mode = GL_TRIANGLES;

	bool visible = true;
	bool casting_shadow = true;

	float max;

	Transform transform;

	void draw();
	Mesh();

	Mesh(const Mesh& other);

	Mesh(const std::string &file_name);
	Mesh(Vertex *vertices, unsigned int number_of_vertices, unsigned int *indices, unsigned int num_indices);

	Mesh(float vertices[][3], int num_pkt, int indices[][3], int num_tr);
	virtual ~Mesh();

private:

	enum {
		POSITION_VB,
		TEXCOORD_VB,
		NORMAL_VB,
		INDEX_VB,
		NUM_BUFFERS
	};

	GLuint vertex_array_object;
	GLuint vertex_array_buffers[NUM_BUFFERS];
	GLuint vertex_array_buffer;

	unsigned int draw_count;

	void InitMesh(const IndexedModel &model);



};

#endif /* MESH_H_ */
