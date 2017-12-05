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



// void Mesh::InitMesh(const IndexedModel &model)
void Mesh::InitMesh()
{
// 	printf("%d\n",  model.indices.size());
    draw_count = indices.size();

    glGenVertexArrays(1, &vertex_array_object);
    glBindVertexArray(vertex_array_object);

    glGenBuffers(NUM_BUFFERS, vertex_array_buffers);

    // positions (vertices)
    glBindBuffer(GL_ARRAY_BUFFER, vertex_array_buffers[POSITION_VB]);
    glBufferData(GL_ARRAY_BUFFER, positions.size()
		   	* sizeof(positions[0]), &positions[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // koordynaty tekstury
    glBindBuffer(GL_ARRAY_BUFFER, vertex_array_buffers[TEXCOORD_VB]);
    glBufferData(GL_ARRAY_BUFFER, positions.size()
		   	* sizeof(texCoords[0]), &texCoords[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);


    // indices - definicje trójkątów
    // GL_ELEMENT_ARRAY_BUFFER to array który odnosi sie do innego arraya
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertex_array_buffers[INDEX_VB]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()
		   	* sizeof(indices[0]), &indices[0], GL_STATIC_DRAW);

    // normalne
    glBindBuffer(GL_ARRAY_BUFFER, vertex_array_buffers[NORMAL_VB]);
    glBufferData(GL_ARRAY_BUFFER, normals.size()
		   	* sizeof(normals[0]), &normals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);


    glBindVertexArray(0);

}

Mesh::Mesh(float vertices[][3], int num_pkt, int indices[][3], int num_tr )
{
	// indices indeksowane od 0 !!!!

    // normalizacja modelu do 1
    if (normalize)
   	{
        max=-1.0e9;
        for (int i = 0; i < num_pkt; i++)
	   	{
			float l = glm::length(glm::vec3(vertices[i][0], vertices[i][1],
						vertices[i][2]) );
            if ( l  > max )
                max = l;
        }

		for (int i = 0; i < num_pkt; i++)
	   	{
			vertices[i][0] /= max;
			vertices[i][1] /= max;
			vertices[i][2] /= max;
		}
    }

	for (int i = 0; i < num_pkt; i++)
	{
		positions.push_back(
				vec3( vertices[i][0], vertices[i][1], vertices[i][2])
		);
	}

	calculateNormals();
	calculateTextureCoords();


    InitMesh();


}

Mesh::Mesh(const std::string &file_name) {

//     model = OBJModel(file_name).ToIndexedModel();

	indexObj(file_name);

    InitMesh();

}

void Mesh::indexObj(const string &file_name)
{
	ifstream f(file_name.c_str() );
	string buf;
	char flag[100];

	if (!f.is_open())
		cout << "error opening file "<< file_name << "\n";

	while ( getline(f, buf) )
	{
		if (buf[0] == 'v')
		{
			if (buf[1] == 't')
			{
				//TODO
			}
			else if (buf[1] == 'n')
			{
				//TODO
			}
			else if (buf[1] == ' ' || buf[1] == '\t')
			{
				// samo v
				double x, y, z;
				if (sscanf(buf.c_str(), "%s %lf %lf %lf", flag, &x, &y, &z)!=4)
					cout << "error reading v line in OBJ file" << "\n";
				else
					positions.push_back(vec3((float)x,(float)y,(float)z));

			}
		}
		else if (buf[0] == 'f')
		{
// 			cout << buf << "\n";
			int a,b,c;
			sscanf(buf.c_str(), "%s %d %d %d", flag, &a, &b, &c);

			indices.push_back(a - 1);
			indices.push_back(b - 1);
			indices.push_back(c - 1);
		}

	}

	f.close();

	calculateTextureCoordinates();
	calculateNormals();


}

void Mesh::calculateTextureCoordinates()
{
	texCoords.clear();
	for (vec3 v: positions)
		texCoords.push_back( textureCoordinates(v)  );
}

void Mesh::calculateNormals()
{
	normals.clear();
	for (auto v: positions)
		normals.push_back( vec3(0., 0., 0.) );


	for(unsigned int i = 0; i < indices.size()-3; i += 3)
	{
		int i0 = indices[i];
		int i1 = indices[i + 1];
		int i2 = indices[i + 2];

		if (i0 >= positions.size() || i0 < 0 ||
				i1 >= positions.size() || i1 < 0 ||
				i2 >= positions.size() || i2 < 0 )
		{
			cout << "index out of range: " << "\t";
			cout << i0<<"\t"<<i1<<"\t"<<i2 << "\n";
		}
//
		glm::vec3 v1 = positions[i0] - positions[i1];
		glm::vec3 v2 = positions[i0] - positions[i2];
//
		glm::vec3 normal = glm::normalize(glm::cross(v1, v2));
//
		normals[i0] += normal;
		normals[i1] += normal;
		normals[i2] += normal;
	}

	for(unsigned int i = 0; i < normals.size(); i++)
		normals[i] = glm::normalize(normals[i]);


}

/*
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
//     model.CalcNormals();


    for (unsigned int i = 0; i < num_indices; i++) {
        model.indices.push_back(indices[i]);
    }

    InitMesh(model);

}
*/

/*

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

*/


Mesh::~Mesh() {
    glDeleteVertexArrays(1, &vertex_array_object);

    glDeleteBuffers(NUM_BUFFERS, vertex_array_buffers);
}

void Mesh::draw()
{

    glBindVertexArray(vertex_array_object);

    glDrawElements(draw_mode, draw_count , GL_UNSIGNED_INT, 0);

// 	glDrawArrays(GL_TRIANGLES, 0, draw_count );
//	glDrawElementsBaseVertex(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, 0, 0);

// puts("in mesh.draw... ");
    glBindVertexArray(0);
}

// glm::vec2 Mesh::calculateTextureCoords(float *vec)
glm::vec2 Mesh::textureCoordinates(vec3 vertex)
{
// 	glm::vec3 vertex = glm::vec3(vec[0], vec[1], vec[2]);
	// do sferycznych:
	float r = length(vertex);
	float alpha = atan2(vertex.y, vertex.x);
	float beta = acos(vertex.z / r);

	float u = 0.5 + alpha/2./M_PI;
	float v = 1. - beta/M_PI;

	return glm::vec2(u, v);
}


