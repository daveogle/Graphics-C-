#pragma once
/* object_ldr.h
   Example class to show the start of an .obj mesh obkect file
   loader
   Iain Martin November 2014
*/

#pragma once

#include "wrapper_glfw.h"
#include <vector>
#include <glm/glm.hpp>

class object_ldr_dave
{
public:
	object_ldr_dave();
	~object_ldr_dave();

	void load_obj(const char* filename);
	void drawObject();
	void createObject();
	void smoothNormals();

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> verticeNormals;
	std::vector<GLuint> elements;

	GLuint vbo_mesh_vertices;
	GLuint vbo_mesh_normals;
	GLuint ibo_mesh_elements;
	GLuint attribute_v_coord;
	GLuint attribute_v_normal;
};

