#pragma once
#include <glload/_int_gl_type.h>
#include <glload/_int_gl_1_5.h>
#include <glload/_int_gl_2_0.h>
#include <glload/_int_gl_1_1.h>
#include <glload/_int_gl_exts.h>
#include <vector>

class cuboid
{
	float height;
	float width;
	float depth;
	GLuint cuboidBufferObject;
	GLfloat vertexPositions[144];


public:
	cuboid(float height, float width, float depth);
	~cuboid();
	void defineVertices(GLfloat *pVertices);
	int cuboid::addTriangle(GLfloat* pVertices, std::vector<float> point1, std::vector<float> point2, std::vector<float> point3, int startIndex);
	void drawCuboid();
};

