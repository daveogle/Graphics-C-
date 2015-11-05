#pragma once
#include <glload/gl_4_0.h>
#include <glload/gl_load.h>
#include <vector>
#include "transformation.h"
#include "lighting.h"

class tank_track
{

	float height;
	float width;
	float depth;
	glm::vec3 colour;
	GLuint trackBufferObject, normalsBufferObject, colourBuffer;;
	GLfloat vertexPositions[264];
	GLfloat normals[198];


public:
	tank_track(float height, float width, float depth, float ambient, float shininess);
	~tank_track();
	transformation* transform;
	lighting* light;
	void defineVertices();
	int setNormal(GLfloat* normalArray, std::vector<float> bottomLeft, std::vector<float> topLeft, std::vector<float> bottomRight, int index);
	int addTriangle(GLfloat* pVertices, std::vector<float> point1, std::vector<float> point2, std::vector<float> point3, int startIndex);
	void drawTrack();
};

