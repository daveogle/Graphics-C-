#ifndef SPHERE_H
#define SPHERE_H

#pragma once

#include <glload/gl_4_0.h>
#include "wrapper_glfw.h"
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glloadD.lib")
#pragma comment(lib, "opengl32.lib")
#include "transformation.h"
#include "lighting.h"

class sphere
{

	GLuint sphereBufferObject, sphereNormals, sphereColours, elementbuffer;
	GLuint numspherevertices;			//drawmode defines drawing mode of sphere as points, lines or filled polygons
	GLuint numlats, numlongs;					//Define the resolution of the sphere object

public:
	transformation* transform;
	lighting* light;

	explicit sphere(GLuint numlats, GLuint numlongs, GLfloat ambient, GLfloat shininess);
	~sphere();
	void makeSphereVBO();
	void makeUnitSphere(GLfloat* p_vertices, GLuint numlats, GLuint numlongs);
	void drawSphere();
};

#endif