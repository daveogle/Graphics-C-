#include "cuboid.h"
#include <iostream>


cuboid::cuboid(float height, float width, float depth, float ambient, float shininess)
{
	this->height = height;
	this->width = width;
	this->depth = depth;
	this->shininess = shininess;
	this->ambient_value = glm::vec3(ambient);
	this->specular_value = glm::vec3(1.0);
	this->diffuse_value = glm::vec3(0.1, 0.1, 0.1);
	defineVertices();
	this->transform = new transformation();

	/* Create a vertex buffer object to store vertices */
	glGenBuffers(1, &this->cuboidBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, this->cuboidBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(this->vertexPositions), this->vertexPositions, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/* Create the normals  buffer for the cube */
	glGenBuffers(1, &this->normalsBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, this->normalsBufferObject);
	glBufferData(GL_ARRAY_BUFFER, 36 * sizeof(glm::vec3), this->normals, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

cuboid::~cuboid()
{
}

void cuboid::defineVertices()
{
	GLfloat* pVertices = new GLfloat[(12 * 3) * 4];
	GLfloat* normalArray = new GLfloat[(12 * 3) * 3];
	float y = this->height / 2;
	float x = this->width / 2;
	float z = this->depth / 2;

	//back top left
	std::vector<float> left_top_back(4, 1.0);
	left_top_back[0] = -x;
	left_top_back[1] = y;
	left_top_back[2] = - z;

	//back bottom left
	std::vector<float> left_bottom_back(4, 1.0);
	left_bottom_back[0] =-x;
	left_bottom_back[1] =- y;
	left_bottom_back[2] =- z;

	//back bottom right
	std::vector<float> right_bottom_back(4, 1.0);
	right_bottom_back[0] = x;
	right_bottom_back[1] = -y;
	right_bottom_back[2] = -z;

	//back top right
	std::vector<float> right_top_back(4, 1.0);
	right_top_back[0] = x;
	right_top_back[1] = y;
	right_top_back[2] = -z;


	//front top left
	std::vector<float> left_top_front(4, 1.0);
	left_top_front[0] = -x;
	left_top_front[1] = y;
	left_top_front[2] = z;

	//front bottom left
	std::vector<float> left_bottom_front(4, 1.0);
	left_bottom_front[0] = -x;
	left_bottom_front[1] = -y;
	left_bottom_front[2] = z;

	//front bottom right
	std::vector<float> right_bottom_front(4, 1.0);
	right_bottom_front[0] = x;
	right_bottom_front[1] = -y;
	right_bottom_front[2] = z;

	//front top right
	std::vector<float> right_top_front(4, 1.0);
	right_top_front[0] = x;
	right_top_front[1] = y;
	right_top_front[2] = z;


	int startIndex = addTriangle(pVertices, left_top_back, left_bottom_back, right_bottom_back, 0);
	int normalIndex = setNormal(normalArray, left_bottom_back, left_top_back, right_bottom_back, 0);
	startIndex = addTriangle(pVertices, right_bottom_back, right_top_back, left_top_back, startIndex);

	startIndex = addTriangle(pVertices, right_bottom_back, right_bottom_front, right_top_back, startIndex);
	normalIndex = setNormal(normalArray, right_bottom_back, right_top_back, right_bottom_front, normalIndex);
	startIndex = addTriangle(pVertices, right_bottom_front, right_top_front, right_top_back, startIndex);


	startIndex = addTriangle(pVertices, right_bottom_front, left_bottom_front, right_top_front, startIndex);
	normalIndex = setNormal(normalArray, right_bottom_front, right_top_front, left_bottom_front, normalIndex);
	startIndex = addTriangle(pVertices, left_bottom_front, left_top_front, right_top_front, startIndex);


	startIndex = addTriangle(pVertices, left_bottom_front, left_bottom_back, left_top_front, startIndex);
	normalIndex = setNormal(normalArray, left_bottom_front, left_top_front, left_bottom_back, normalIndex);
	startIndex = addTriangle(pVertices, left_bottom_back, left_top_back, left_top_front, startIndex);


	startIndex = addTriangle(pVertices, left_bottom_front, right_bottom_front, right_bottom_back, startIndex);
	normalIndex = setNormal(normalArray, right_bottom_front, left_bottom_front, right_bottom_back, normalIndex);
	startIndex = addTriangle(pVertices, right_bottom_back, left_bottom_back, left_bottom_front, startIndex);


	startIndex = addTriangle(pVertices, left_top_back, right_top_back, right_top_front, startIndex);
	normalIndex = setNormal(normalArray, right_top_back, left_top_back, right_top_front, normalIndex);
	startIndex = addTriangle(pVertices, right_top_front, left_top_front, left_top_back, startIndex);


	for (int i = 0; i < normalIndex; i++)
	{
		this->normals[i] = normalArray[i];
	}
	for (int i = 0; i < startIndex; i++)
	{
		this->vertexPositions[i] = pVertices[i];
	}
}

//Define normal of a triangle with normal pointing out of screen
int cuboid::setNormal(GLfloat* normalArray, std::vector<float> origin, std::vector<float> first_point, std::vector<float> second_point, int index)
{
	glm::vec3 V = glm::vec3((first_point[0] - origin[0]), (first_point[1] - origin[1]), (first_point[2] - origin[2]));
	glm::vec3 W = glm::vec3((second_point[0] - origin[0]), (second_point[1] - origin[1]), (second_point[2] - origin[2]));
	glm::vec3 xProduct = glm::normalize(glm::cross(V, W));
	for (int i = 0; i < 6; i++)
	{
		normalArray[index] = xProduct.x;
		normalArray[index + 1] = xProduct.y;
		normalArray[index + 2] = xProduct.z;
		index = index + 3;
	}
	return index;
}

int cuboid::addTriangle(GLfloat* pVertices, std::vector<float> point1, std::vector<float> point2, std::vector<float> point3, int startIndex)
{
	for (auto i = 0; i < point1.size(); i++)
	{
		pVertices[startIndex] = point1[i];
		startIndex++;
	}

	for (auto i = 0; i < point2.size(); i++)
	{
		pVertices[startIndex] = point2[i];
		startIndex++;
	}

	for (auto i = 0; i < point3.size(); i++)
	{
		pVertices[startIndex] = point3[i];
		startIndex++;
	}

	return startIndex;
}

void cuboid::drawCuboid()
{
	glBindBuffer(GL_ARRAY_BUFFER, cuboidBufferObject);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, normalsBufferObject);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);


	glDrawArrays(GL_TRIANGLES, 0, 36);
}

glm::mat4 cuboid::getModel()
{
	return transform->getModel();
}

glm::vec3 cuboid::getAmbient()
{
	return this->ambient_value;
}

GLfloat cuboid::getShininess()
{
	return this->shininess;
}

glm::vec3 cuboid::getSpecular()
{
	return this->specular_value;
}

glm::vec3 cuboid::getDiffuse()
{
	return diffuse_value;
}

void cuboid::setSpecular(GLfloat colour_one, GLfloat colour_two, GLfloat colour_three)
{
	this->specular_value = glm::vec3(colour_one, colour_two, colour_three);
}

void cuboid::setDiffuse(GLfloat colour_one, GLfloat colour_two, GLfloat colour_three)
{
	this->diffuse_value = glm::vec3(colour_one, colour_two, colour_three);
}