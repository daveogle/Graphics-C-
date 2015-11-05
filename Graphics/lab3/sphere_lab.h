#ifndef SPHERE_LAB_H
#define SPHERE_LAB_H

#pragma once

#include <glload/gl_4_0.h>
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glloadD.lib")
#pragma comment(lib, "opengl32.lib")

class sphere_lab
{

	GLuint sphereBufferObject, sphereNormals, sphereColours, elementbuffer;
	GLuint drawmode, numspherevertices;			//drawmode defines drawing mode of sphere as points, lines or filled polygons
	GLuint numlats, numlongs;	//Define the resolution of the sphere object

public:
	explicit sphere_lab(GLuint numlats, GLuint numlongs);
	~sphere_lab();
	void makeSphereVBO();
	void makeUnitSphere(GLfloat* p_vertices, GLuint numlats, GLuint numlongs);
	void drawSphere();
	GLuint incrementDrawmode(GLuint value);
	void setDrawMode(GLuint value);
};

#endif

