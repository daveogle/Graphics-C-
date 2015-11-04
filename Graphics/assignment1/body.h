#pragma once
#include <glload/gl_4_0.h>
#include <glload/gl_load.h>
#include <vector>
#include "transformation.h"
#include "lighting.h"

class body
{
public:
	transformation* transform;
	lighting* light;
	GLuint bodyBufferObject, normalsBufferObject, elementbuffer;

	body(GLfloat shininess, GLfloat ambient);
	~body();
	void defineVeritices();
	void drawBody();
};

