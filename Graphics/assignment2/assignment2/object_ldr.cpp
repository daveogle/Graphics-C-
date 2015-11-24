/* object_ldr.cpp
Example class to show the start of an .obj mesh obkect file
loader
Iain Martin November 2014

This is a wrapper class based around the code taken from :
http://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Load_OBJ
I made a few changes to fit in with my vertex shaders and added the code to
bind the vertex buffers.
Uses std::vector class as the containor for the array of glm::vec3 types
*/

#include "object_ldr.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

/* Define the vertex attributes for vertex positions and normals. 
   Make these match your application and vertex shader
   You might also want to add colours and texture coordinates */
object_ldr::object_ldr()
{
	texture = false;
	attribute_v_coord = 0;
	attribute_v_normal = 1;
	attribute_v_texture = 2;
	textureId = 0;
}


object_ldr::~object_ldr()
{
}

/* Load the object, parsing the file. 
For every line begining with 'v', it adds an extra glm::vec3 to the std::vector containor 
For every line beginning with 'f', it adds the "face" indices to the array of indices
Then it calculates flat shaded normals, i.e calculates the face normal and applies that to 
all vertices in the face.

This function could be improved by extending the parsing to cope with face definitions with
normals defined.
*/
void object_ldr::load_obj(const char* filename) {
	ifstream in(filename, ios::in);
	if (!in) { cerr << "Cannot open " << filename << endl; exit(1); }
	string line;
	while (getline(in, line))
	{
		if (line.substr(0, 2) == "v ") //if vertacie
		{
			istringstream s(line.substr(2));
			glm::vec3 v; s >> v.x; s >> v.y; s >> v.z;
			vertices.push_back(glm::vec4(v, 1.0f));
		}
		else if (line.substr(0, 2) == "vn")//if normals
		{
			istringstream s(line.substr(2));
			glm::vec3 n; s >> n.x; s >> n.y; s >> n.z;
			n = glm::normalize(n);
			normals.push_back(n);
		}
		else if (line.substr(0, 2) == "vt")//if texture
		{
			texture = true;
			istringstream s(line.substr(2));
			glm::vec2 v; s >> v.x; s >> v.y;
			textures.push_back(glm::vec3(v, 0.0f));
		}
		else if (line.substr(0, 2) == "f ")
		{
			if (verticeNormals.size() < vertices.size())
				verticeNormals.resize(vertices.size(), glm::vec3(1.0f));
			if (textureCoords.size() < vertices.size())
				textureCoords.resize(vertices.size(), glm::vec3(0.0f));
			vector<string> face;
			std::stringstream s(line.substr(2));
			std::string newFace;
			char delim = ' ';
			while (getline(s, newFace, delim)) //get the face values
			{
				face.push_back(newFace);
			}
			for (string vnPair : face)
			{
				size_t firstSlash = vnPair.find('/');
				string element = vnPair.substr(0, firstSlash);
				vnPair = vnPair.substr(firstSlash + 1);
				size_t secondSlash = vnPair.find('/');
				string tex = vnPair.substr(0, secondSlash);
				string normal = vnPair.substr(secondSlash + 1);
				GLuint u_element = atoi(element.c_str());
				elements.push_back(u_element - 1);
				GLuint normalIndex = atoi(normal.c_str());
				verticeNormals.at(u_element - 1) = normals.at(normalIndex - 1);
				if (tex != "")
				{
					GLuint u_texture = atoi(tex.c_str());
					textureCoords.at(u_element - 1) = textures.at(u_texture - 1);
				}
			}
		}
	}
	std::cout << "File " << filename << "loaded\n" << endl;
}

/* Copy the vertices, normals and element indices into vertex buffers */
void object_ldr::createObject()
{
	/* Generate the vertex buffer object */
	glGenBuffers(1, &vbo_mesh_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_mesh_vertices);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec4), &(vertices[0]), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/* Store the normals in a buffer object */
	glGenBuffers(1, &vbo_mesh_normals);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_mesh_normals);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &(normals[0]), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &vbo_mesh_textures);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_mesh_textures);
	glBufferData(GL_ARRAY_BUFFER, textures.size() * sizeof(glm::vec3), &(textures[0]), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Generate a buffer for the indices
	glGenBuffers(1, &ibo_mesh_elements);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_mesh_elements);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements.size() * sizeof(GLuint), &(elements[0]), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

/* Enable vertex attributes and draw object
Could improve efficiency by moving the vertex attribute pointer functions to the
create object but this method is more general 
This code is almost untouched fomr the tutorial code except that I changed the
number of elements per vertex from 4 to 3*/
void object_ldr::drawObject()
{
	int size;	// Used to get the byte size of the element (vertex index) array

	// Describe our vertices array to OpenGL (it can't guess its format automatically)
	glEnableVertexAttribArray(attribute_v_coord);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_mesh_vertices);
	glVertexAttribPointer(
		attribute_v_coord,  // attribute index
		4,                  // number of elements per vertex, here (x,y,z)
		GL_FLOAT,           // the type of each element
		GL_FALSE,           // take our values as-is
		0,                  // no extra data between each position
		0                   // offset of first element
		);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_mesh_normals);
	glEnableVertexAttribArray(attribute_v_normal);
	glVertexAttribPointer(
		attribute_v_normal, // attribute
		3,                  // number of elements per vertex, here (x,y,z)
		GL_FLOAT,           // the type of each element
		GL_FALSE,           // take our values as-is
		0,                  // no extra data between each position
		(void*)0            // offset of first element
		);

	if (texture)
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo_mesh_textures);
		glEnableVertexAttribArray(attribute_v_texture);
		glVertexAttribPointer(
			attribute_v_texture, // attribute
			3,                  // number of elements per vertex, here (x,y,z)
			GL_FLOAT,           // the type of each element
			GL_FALSE,           // take our values as-is
			0,                  // no extra data between each position
			(void*)0            // offset of first element
			);
		glBindTexture(GL_TEXTURE_2D, textureId);
	}
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_mesh_elements); 
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	glDrawElements(GL_TRIANGLES, size / sizeof(GLuint), GL_UNSIGNED_INT, 0);
}

/* This is the smooth normals function given in the tutorial code */
void object_ldr::smoothNormals()
{
	std::vector<GLuint> nb_seen;
	normals.resize(vertices.size(), glm::vec3(0.0, 0.0, 0.0));
	nb_seen.resize(vertices.size(), 0);
	for (int i = 0; i < elements.size(); i += 3) {
		GLushort ia = elements[i];
		GLushort ib = elements[i + 1];
		GLushort ic = elements[i + 2];
		glm::vec3 normal = glm::normalize(glm::cross(
			glm::vec3(vertices[ib]) - glm::vec3(vertices[ia]),
			glm::vec3(vertices[ic]) - glm::vec3(vertices[ia])));

		int v[3];  v[0] = ia;  v[1] = ib;  v[2] = ic;
		for (int j = 0; j < 3; j++) {
			GLushort cur_v = v[j];
			nb_seen[cur_v]++;
			if (nb_seen[cur_v] == 1) {
				normals[cur_v] = normal;
			}
			else {
				// average
				normals[cur_v].x = normals[cur_v].x * (1.0 - 1.0 / nb_seen[cur_v]) + normal.x * 1.0 / nb_seen[cur_v];
				normals[cur_v].y = normals[cur_v].y * (1.0 - 1.0 / nb_seen[cur_v]) + normal.y * 1.0 / nb_seen[cur_v];
				normals[cur_v].z = normals[cur_v].z * (1.0 - 1.0 / nb_seen[cur_v]) + normal.z * 1.0 / nb_seen[cur_v];
				normals[cur_v] = glm::normalize(normals[cur_v]);
			}
		}
	}

}

bool object_ldr::setTexture(const char* textureFile, int textureNumber)
{
	textureId = texture_loader::loadTexture(textureFile, textureNumber);
	if (textureId != 0)
		return texture = true;
	return texture = false;
}