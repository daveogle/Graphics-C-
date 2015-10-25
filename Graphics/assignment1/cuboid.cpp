#include "cuboid.h"
#include <iostream>


cuboid::cuboid(float height, float width, float depth)
{
	this->height = height;
	this->width = width;
	this->depth = depth;
	GLfloat* pVertices = new GLfloat[(12 * 3)*4];
	defineVertices(pVertices);

	/* Create a vertex buffer object to store vertices */
	glGenBuffers(1, &this->cuboidBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, this->cuboidBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(this->vertexPositions), this->vertexPositions, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


cuboid::~cuboid()
{
}

void cuboid::defineVertices(GLfloat* pVertices)
{
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
	startIndex = addTriangle(pVertices, right_bottom_back, right_top_back, left_top_back, startIndex);

	startIndex = addTriangle(pVertices, right_bottom_back, right_bottom_front, right_top_back, startIndex);
	startIndex = addTriangle(pVertices, right_bottom_front, right_top_front, right_top_back, startIndex);


	startIndex = addTriangle(pVertices, right_bottom_front, left_bottom_front, right_top_front, startIndex);
	startIndex = addTriangle(pVertices, left_bottom_front, left_top_front, right_top_front, startIndex);


	startIndex = addTriangle(pVertices, left_bottom_front, left_bottom_back, left_top_front, startIndex);
	startIndex = addTriangle(pVertices, left_bottom_back, left_top_back, left_top_front, startIndex);


	startIndex = addTriangle(pVertices, left_bottom_front, right_bottom_front, right_bottom_back, startIndex);
	startIndex = addTriangle(pVertices, right_bottom_back, left_bottom_back, left_bottom_front, startIndex);


	startIndex = addTriangle(pVertices, left_top_back, right_top_back, right_top_front, startIndex);
	startIndex = addTriangle(pVertices, right_top_front, left_top_front, left_top_back, startIndex);

	//print them all out
	int count = 1;
	for (int i = 0; i < startIndex; i++)
	{
		this->vertexPositions[i] = pVertices[i];
		//std::cout << pVertices[i] << ',';
		//count++;
		//if (count == 5)
		//{
		//	std::cout << std::endl;
		//	count = 1;
		//}
	}
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
	glDrawArrays(GL_TRIANGLES, 0, 36);
}
