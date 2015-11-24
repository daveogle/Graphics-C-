#pragma once
/* object_ldr.h
   Example class to show the start of an .obj mesh obkect file
   loader
   Iain Martin November 2014
*/
#include "wrapper_glfw.h"
#include <vector>
#include <glm/glm.hpp>
#include "texture_loader.h"

class object_ldr
{
public:
	object_ldr();
	~object_ldr();

	void load_obj(const char* filename);
	void drawObject();
	void createObject();
	void smoothNormals();
	bool setTexture(const char* textureFile, int textureNumber);

	std::vector<glm::vec4> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> verticeNormals;
	std::vector<glm::vec3> textureCoords;
	std::vector<glm::vec3> textures;
	std::vector<GLuint> elements;

	GLuint vbo_mesh_vertices;
	GLuint vbo_mesh_normals;
	GLuint ibo_mesh_elements;
	GLuint vbo_mesh_textures;

	GLuint attribute_v_coord;
	GLuint attribute_v_normal;
	GLuint attribute_v_texture;
	GLuint textureId;

	GLboolean texture;
};

