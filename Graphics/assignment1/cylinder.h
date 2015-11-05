#ifndef CYLINDER_H
#define CYLINDER_H

#include <glload/gl_4_0.h>
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glloadD.lib")
#pragma comment(lib, "opengl32.lib")
#include "transformation.h"
#include "lighting.h"
#include "math.h"

class cylinder
{
	glm::vec3 colour;
	GLfloat radius, length;
	int definition;
	GLuint cylinderBufferObject, cylinderNormals, cylinderColours, cylinderElementbuffer;
	GLuint num_pvertices;
	GLuint isize;

public:
	transformation* transform;
	lighting* light;

	cylinder(GLfloat radius, GLfloat length, GLfloat ambient, GLfloat shininess);
	~cylinder();
	void makeCylinderVBO();
	void defineVertices();
	void drawCyclinder();
};

#endif
