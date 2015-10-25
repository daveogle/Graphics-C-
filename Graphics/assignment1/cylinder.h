#pragma once
#include "wrapper_glfw.h"
#include <glm/glm.hpp>
#include <iostream>
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glloadD.lib")
#pragma comment(lib, "opengl32.lib")

class cylinder
{

	float radius, length;
	int definition;
	GLuint cylinderBufferObject;
	GLuint num_vertices;

public:
	cylinder(float radius, float length, int definition);
	~cylinder();
	void defineVertices(GLfloat *pVertices);
	void drawCyclinder();
};

