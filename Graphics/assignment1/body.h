#pragma once
#include <glload/gl_4_0.h>
#include <glload/gl_load.h>
#include <vector>
#include "transformation.h"
#include "lighting.h"

class body
{
	int numberOfVerticies = 23;
	int isize;
	glm::vec3 colour;

public:
	transformation* transform;
	lighting* light;
	GLuint bodyBufferObject, normalsBufferObject, elementbuffer, colourBuffer;

	body(GLfloat shininess, GLfloat ambient);
	~body();
	void defineVeritices();
	void drawBody();
};

