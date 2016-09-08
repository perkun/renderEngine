/*
 * Mesh.cpp
 *
 *  Created on: 15 wrz 2015
 *      Author: grzeslaff
 */

#include "Mesh.h"

#define GLM_FORCE_RADIANS
// #include "obj_loader.h"


Mesh::Mesh() {
    // blabla
}



Mesh::Mesh(const Mesh& other)
{
//     puts("sdsdsds");


    vertex_array_object = other.vertex_array_object;
    vertex_array_buffer = other.vertex_array_buffer;
    for (int i = 0; i < NUM_BUFFERS; i++)
        vertex_array_buffers[i] = other.vertex_array_buffers[i];
    draw_count = other.draw_count;
}



void Mesh::InitMesh(const IndexedModel &model)
{
// 	printf("%d\n",  model.indices.size());
    draw_count = model.indices.size();

    glGenVertexArrays(1, &vertex_array_object);
    glBindVertexArray(vertex_array_object);

    glGenBuffers(NUM_BUFFERS, vertex_array_buffers);

    // positions (vertices)
    glBindBuffer(GL_ARRAY_BUFFER, vertex_array_buffers[POSITION_VB]);
    glBufferData(GL_ARRAY_BUFFER, model.positions.size() * sizeof(model.positions[0]), &model.positions[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // koordynaty tekstury
    glBindBuffer(GL_ARRAY_BUFFER, vertex_array_buffers[TEXCOORD_VB]);
    glBufferData(GL_ARRAY_BUFFER, model.positions.size() * sizeof(model.texCoords[0]), &model.texCoords[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);


    // indices - definicje trójkątów
    // GL_ELEMENT_ARRAY_BUFFER to array który odnosi sie do innego arraya
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertex_array_buffers[INDEX_VB]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.indices.size() * sizeof(model.indices[0]), &model.indices[0], GL_STATIC_DRAW);

    // normalne
    glBindBuffer(GL_ARRAY_BUFFER, vertex_array_buffers[NORMAL_VB]);
    glBufferData(GL_ARRAY_BUFFER, model.normals.size()
		   	* sizeof(model.normals[0]), &model.normals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);


    glBindVertexArray(0);

}


Mesh::Mesh(const std::string &file_name) {

	std::cout << "indexing...\n";
    model = OBJModel(file_name).ToIndexedModel();
	std::cout << "model indexed\n";

// 	for (int i = 0; i < 100; i++)
// 		printf("%f \t %f \t %f\n", model.positions[i].x, model.positions[i].y,
// 		model.positions[i].z);
// 	puts("xxxxxxxxxxxxxxx");
    InitMesh(model);

}



Mesh::Mesh(Vertex* vertices, unsigned int number_of_vertices,
		unsigned int *indices, unsigned int num_indices)
{

//     IndexedModel model;

// 	m_draw_count = number_of_vertices;
    draw_count = num_indices;
//
// 	std::vector<glm::vec3> positions;
// 	std::vector<glm::vec2> tex_coords;
//
// 	positions.reserve(number_of_vertices);
// 	tex_coords.reserve(number_of_vertices);
//

    for(unsigned int i = 0; i < number_of_vertices; i++) {
        model.positions.push_back(vertices[i].pos);
        model.texCoords.push_back(vertices[i].tex_coord);
        model.normals.push_back(vertices[i].normal);
    }
    model.CalcNormals();


    for (unsigned int i = 0; i < num_indices; i++) {
        model.indices.push_back(indices[i]);
    }

    InitMesh(model);

}


Mesh::Mesh(float vertices[][3], int num_pkt, int indices[][3], int num_tr )
{
	// dodać licznie wsp. tekstry

// 	IndexedModel model;

// 	glm::vec3 normals[num_pkt];

    // normalizacja modelu do 1
    if (normalize)
   	{
        max=-1.;
        for (int i = 0; i < num_pkt; i++)
	   	{
			float l = glm::length(glm::vec3(vertices[i][0], vertices[i][1],
						vertices[i][2]) );
            if ( l  > max )
                max = l;
        }
//         transform.scale = glm::vec3(1./max, 1./max, 1./max);
		for (int i = 0; i < num_pkt; i++)
	   	{
			vertices[i][0] /= max;
			vertices[i][1] /= max;
			vertices[i][2] /= max;
		}
    }

    // brak interpolacji normalnych
    if (!normal_interpolation)
   	{
        int k = 0;
        for (int i = 0; i < num_tr; i++)
	   	{
            int i0 = indices[i][0] -1;
            int i1 = indices[i][1] -1;
            int i2 = indices[i][2] -1;

			model.positions.push_back( glm::vec3( vertices[i0][0],
						vertices[i0][1], vertices[i0][2]  ) );
			// finkcja licząca texCoord f(vertex) = vec2(u,v)
//             model.texCoords.push_back(glm::vec2(0.,0.) );
            model.texCoords.push_back(calculateTextureCoords(vertices[i0]));
            model.normals.push_back( glm::vec3(0.0,0.0,0.0));

			model.positions.push_back( glm::vec3( vertices[i1][0],
						vertices[i1][1], vertices[i1][2]  ) );
//             model.texCoords.push_back(glm::vec2(0.,0.) );
            model.texCoords.push_back(calculateTextureCoords(vertices[i1]));
            model.normals.push_back( glm::vec3(0.0,0.0,0.0));

			model.positions.push_back( glm::vec3( vertices[i2][0],
						vertices[i2][1], vertices[i2][2]  ) );
//             model.texCoords.push_back(glm::vec2(0.,0.) );
            model.texCoords.push_back(calculateTextureCoords(vertices[i2]));
            model.normals.push_back( glm::vec3(0.0,0.0,0.0));


            model.indices.push_back( k    );
            model.indices.push_back( k +1 );
            model.indices.push_back( k +2 );

            k += 3;
        }
    }

    // 	interpolacja normalnych na powierzchni
    if (normal_interpolation)
   	{
        for (int i = 0; i < num_tr; i++) {
            for (int j = 0; j < 3; j++)
				// przesuniecie (bo w pliku jest numeracja trojkatów od 1)
				model.indices.push_back(indices[i][j]-1);
        }
        for ( int i = 0; i < num_pkt; i++) {
			model.positions.push_back( glm::vec3( vertices[i][0],
						vertices[i][1], vertices[i][2]  ) );

//             model.texCoords.push_back(glm::vec2(0.,0.) );
            model.texCoords.push_back(calculateTextureCoords(vertices[i]));

            model.normals.push_back( glm::vec3(0.0,0.0,0.0));
        }
    }


    // oblicz normalne do pow
    /*	for ( int i = 0; i < model.indices.size(); i += 3) {
    		int i0 = model.indices[i];
    		int i1 = model.indices[i + 1];
    		int i2 = model.indices[i + 2];

    		glm::vec3 v1 = model.positions[i0] - model.positions[i1];
    		glm::vec3 v2 = model.positions[i0] - model.positions[i2];

    		glm::vec3 normal = glm::normalize(glm::cross(v1, v2));

    		model.normals[i0] += normal;
    		model.normals[i1] += normal;
    		model.normals[i2] += normal;
    	}
    	for ( int i = 0; i < model.positions.size(); i++)
    		model.normals[i] = glm::normalize(model.normals[i]);
    */

    model.CalcNormals();

    InitMesh(model);

	////////////////////////////////////////////////////////////
	// teraz tracony jest model i vertices i indices, wiec tutaj trzeba obliczyc
	// teksture (tablice 2D) z indeksami trojkatów
	// jakas flaga? bool? opcj, czy nowy konstrukor?
	// - zapisany IndexedModel model jako public var
	////////////////////////////////////////////////////////////


}

Mesh::Mesh(float vertices[][3], int num_pkt, int indices[][3], int num_tr,
		float texture_coords[][2] )
{

// 	IndexedModel model;


	// 	glm::vec3 normals[num_pkt];

	// normalizacja modelu do 1
	if (normalize) {
		max=-1.;
		for (int i = 0; i < num_pkt; i++) {
			float l = glm::length(glm::vec3(vertices[i][0], vertices[i][1], vertices[i][2]) );
			if ( l  > max )
				max = l;
		}
		//         transform.scale = glm::vec3(1./max, 1./max, 1./max);
		for (int i = 0; i < num_pkt; i++) {
			vertices[i][0] /= max;
			vertices[i][1] /= max;
			vertices[i][2] /= max;
		}
	}

	// brak interpolacji normalnych
	if (!normal_interpolation) {
		int k = 0;
		for (int i = 0; i < num_tr; i++) {
			int i0 = indices[i][0] -1;
			int i1 = indices[i][1] -1;
			int i2 = indices[i][2] -1;

			model.positions.push_back( glm::vec3( vertices[i0][0],
						vertices[i0][1], vertices[i0][2]  ) );
// 			model.texCoords.push_back(glm::vec2(0.,0.) );
			model.texCoords.push_back(glm::vec2(texture_coords[i0][0],
						texture_coords[i0][1]) );
			model.normals.push_back( glm::vec3(0.0,0.0,0.0));

			model.positions.push_back( glm::vec3( vertices[i1][0],
						vertices[i1][1], vertices[i1][2]  ) );
// 			model.texCoords.push_back(glm::vec2(0.,0.) );
			model.texCoords.push_back(glm::vec2(texture_coords[i1][0],
						texture_coords[i1][1]) );
			model.normals.push_back( glm::vec3(0.0,0.0,0.0));

			model.positions.push_back( glm::vec3( vertices[i2][0],
						vertices[i2][1], vertices[i2][2]  ) );
// 			model.texCoords.push_back(glm::vec2(0.,0.) );
			model.texCoords.push_back(glm::vec2(texture_coords[i2][0],
						texture_coords[i2][1]) );
			model.normals.push_back( glm::vec3(0.0,0.0,0.0));


			model.indices.push_back( k  );
			model.indices.push_back( k +1 );
			model.indices.push_back( k  +2);

			k += 3;
		}
	}

	// 	interpolacja normalnych na powierzchni
	if (normal_interpolation)
   	{
		for (int i = 0; i < num_tr; i++)
	   	{
			for (int j = 0; j < 3; j++)
				// przesuniecie (bo w pliku jest numeracja trojkatów od 1)
				model.indices.push_back(indices[i][j]-1);
		}

		for ( int i = 0; i < num_pkt; i++)
	   	{
			model.positions.push_back( glm::vec3( vertices[i][0],
						vertices[i][1], vertices[i][2]  ) );
// 			model.texCoords.push_back(glm::vec2(0.,0.) );
			model.texCoords.push_back(glm::vec2(texture_coords[i][0],
						texture_coords[i][1]) );
			model.normals.push_back( glm::vec3(0.0,0.0,0.0));
		}
	}

	model.CalcNormals();

	InitMesh(model);
}



Mesh::~Mesh() {
    glDeleteVertexArrays(1, &vertex_array_object);
}

void Mesh::draw() {

    glBindVertexArray(vertex_array_object);

    glDrawElements(draw_mode, draw_count , GL_UNSIGNED_INT, 0);

// 	glDrawArrays(GL_TRIANGLES, 0, draw_count );
//	glDrawElementsBaseVertex(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, 0, 0);

// puts("in mesh.draw... ");
    glBindVertexArray(0);
}

glm::vec2 Mesh::calculateTextureCoords(float *vec)
{
	glm::vec3 vertex = glm::vec3(vec[0], vec[1], vec[2]);
	// do sferycznych:
	float r = length(vertex);
	float alpha = atan2(vertex.y, vertex.x);
	float beta = acos(vertex.z / r);

	float u = 0.5 + alpha/2./M_PI;
	float v = 1. - beta/M_PI;

	return glm::vec2(u, v);
}


