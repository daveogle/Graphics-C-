#ifndef SPHERE_H
#define SPHERE_H

#pragma once

#include <glload/gl_4_0.h>
#include <glload/gl_load.h>
#include <GLFW/glfw3.h>
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glloadD.lib")
#pragma comment(lib, "opengl32.lib")
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"

class sphere
{

	GLuint sphereBufferObject, sphereNormals, sphereColours, elementbuffer;
	GLuint drawmode, numspherevertices;			//drawmode defines drawing mode of sphere as points, lines or filled polygons
	GLuint numlats, numlongs;	//Define the resolution of the sphere object

public:
	explicit sphere();
	~sphere();
	void makeSphereVBO();
	void makeUnitSphere(GLfloat* p_vertices, GLuint numlats, GLuint numlongs);
	void drawSphere();
	GLuint incrementDrawmode(GLuint value);
	void setDrawMode(GLuint value);
};

#endif

