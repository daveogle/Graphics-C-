#pragma once

#include <glm/glm.hpp>
#include "wrapper_glfw.h"
#include "texture_loader.h"

class points
{
public:
	points(GLuint number, GLfloat floor, GLfloat sp);
	~points();

	void create();
	void draw();
	void animate();
	void updateParams(GLfloat dist, GLfloat sp);
	bool setTexture(const char* textureFile);

	unsigned int snowTexID;
	GLboolean texture;
	GLuint quad_vbo;
	GLfloat range;

	glm::vec4 *vertices;
	glm::vec3 *colours;
	glm::vec3 *velocity;

	GLuint numpoints;		// Number of particles
	GLuint vertex_buffer;
	GLuint colour_buffer;

	// Particle speed
	GLfloat speed;		

	// Particle max distance fomr the origin before we change direction back to the centre
	GLfloat floor;	
};

