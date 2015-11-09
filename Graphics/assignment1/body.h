#pragma once
#include <glload/gl_4_0.h>
#include <glload/gl_load.h>
#include <vector>
#include "transformation.h"
#include "lighting.h"
#include "cylinder.h"

class body
{
	int numberOfVerticies = 23;
	int isize;
	glm::vec3 colour;
	cylinder* base_cylider;
	transformation* gun_transformation;

public:
	transformation* transform;
	lighting* light;
	GLuint bodyBufferObject, normalsBufferObject, elementbuffer, colourBuffer;

	body(GLfloat shininess, GLfloat ambient);
	~body();
	void defineVeritices();
	void drawBody();
	glm::vec3 getColour();
	std::vector<transformation*> getGunTransformations();
	cylinder* getBaseCylider();
	glm::mat4 spinTurret(float amount);
};

