#pragma once
#include <glload/gl_4_0.h>
#include <glload/gl_load.h>
#include <vector>
#include "transformation.h"

class cuboid
{
	float height;
	float width;
	float depth;
	GLuint cuboidBufferObject, normalsBufferObject;
	GLfloat vertexPositions[144];
	GLfloat normals[108];
	GLfloat shininess;
	glm::vec3 specular_value, ambient_value, diffuse_value;

public:
	transformation* transform;
	cuboid(float height, float width, float depth, float ambient, float shininess);
	~cuboid();
	void defineVertices();
	int setNormal(GLfloat* normalArray, std::vector<float> bottomLeft, std::vector<float> topLeft, std::vector<float> bottomRight, int index);
	int cuboid::addTriangle(GLfloat* pVertices, std::vector<float> point1, std::vector<float> point2, std::vector<float> point3, int startIndex);
	void drawCuboid();
	glm::mat4 getModel();
	glm::vec3 getAmbient();
	GLfloat getShininess();
	glm::vec3 getSpecular();
	glm::vec3 getDiffuse();
	void setSpecular(GLfloat colour_one, GLfloat colour_two, GLfloat colour_three);
	void setDiffuse(GLfloat colour_one, GLfloat colour_two, GLfloat colour_three);
};

