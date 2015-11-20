#pragma once
/* terrain_object.h
   Example class to show how to create and render a height map
   Iain Martin November 2014
*/

#pragma once

#include "wrapper_glfw.h"
#include <vector>
#include <glm/glm.hpp>

class terrain_object
{
private:
	void calculateNoise();
	void calculateNormals();
	void stretchToRange(GLfloat min, GLfloat max);
	void defineSea(GLfloat sealevel);

public:
	terrain_object(int octaves, GLfloat freq, GLfloat scale);
	~terrain_object();

	void createTerrain(GLuint xp, GLuint yp, GLfloat xs, GLfloat ys);
	void createObject();
	void drawObject(int drawmode);

	GLboolean texture;

	glm::vec4 *vertices;
	glm::vec3 *normals;
	glm::vec3 *textures;
	std::vector<GLuint> elements;
	GLfloat* noise;

	GLuint vbo_mesh_vertices;
	GLuint vbo_mesh_normals;
	GLuint ibo_mesh_elements;
	GLuint vbo_mesh_texture;
	GLuint attribute_v_coord;
	GLuint attribute_v_normal;
	GLuint attribute_v_texture;
	unsigned int texID;

	GLuint xsize;
	GLuint zsize;
	GLfloat width;
	GLfloat height;
	GLuint perlin_octaves;
	GLfloat perlin_freq;
	GLfloat perlin_scale;
	GLfloat height_scale;
};

