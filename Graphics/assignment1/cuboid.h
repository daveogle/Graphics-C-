#pragma once
#include <glload/gl_4_0.h>
#include <glload/gl_load.h>
#include <vector>
#include "transformation.h"
#include "lighting.h"

class cuboid
{
	float height;
	float width;
	float depth;
	GLuint cuboidBufferObject, normalsBufferObject;
	GLfloat vertexPositions[144];
	GLfloat normals[108];

public:
	transformation* transform;
	lighting* light;
	cuboid(float height, float width, float depth, float ambient, float shininess);
	~cuboid();
	void defineVertices();
	int setNormal(GLfloat* normalArray, std::vector<float> bottomLeft, std::vector<float> topLeft, std::vector<float> bottomRight, int index);
	int cuboid::addTriangle(GLfloat* pVertices, std::vector<float> point1, std::vector<float> point2, std::vector<float> point3, int startIndex);
	void drawCuboid();
};

