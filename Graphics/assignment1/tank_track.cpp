#include "tank_track.h"


tank_track::tank_track(float height, float width, float depth, float ambient, float shininess)
{
	this->height = height;
	this->width = width;
	this->depth = depth;
	this->colour = glm::vec3(0.85, 0.85, 0.85);
	this->light = new lighting(shininess, ambient);
	defineVertices();
	this->transform = new transformation();
	glm::vec3 pColours[sizeof(this->vertexPositions)];
	for (int i = 0; i < sizeof(this->vertexPositions); i++)
	{
		pColours[i] = this->colour;
	}

	/* Store the colours in a buffer object */
	glGenBuffers(1, &this->colourBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colourBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * sizeof(this->vertexPositions), pColours, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/* Create a vertex buffer object to store vertices */
	glGenBuffers(1, &this->trackBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, this->trackBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(this->vertexPositions), this->vertexPositions, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/* Create the normals  buffer for the cube */
	glGenBuffers(1, &this->normalsBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, this->normalsBufferObject);
	glBufferData(GL_ARRAY_BUFFER, 66 * sizeof(glm::vec3), this->normals, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


tank_track::~tank_track()
{
}

void tank_track::defineVertices()
{
	GLfloat* pVertices = new GLfloat[(22 * 3) * 4];
	GLfloat* normalArray = new GLfloat[(22 * 3) * 3];
	float y = this->height / 2;
	float x = this->width / 2;
	float z = this->depth / 2;
	float m = this->width / 3;

	//back top left
	std::vector<float> left_top_back(4, 1.0);
	left_top_back[0] = -x;
	left_top_back[1] = y;
	left_top_back[2] = -z;

	//back bottom left
	std::vector<float> left_bottom_back(4, 1.0);
	left_bottom_back[0] = -x;
	left_bottom_back[1] = -y;
	left_bottom_back[2] = -z;

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

	//back middle left top
	std::vector<float> back_middleLeft_top(4, 1.0);
	back_middleLeft_top[0] = m;
	back_middleLeft_top[1] = y + m;
	back_middleLeft_top[2] = -z;

	//back middle left bottom
	std::vector<float> back_middleLeft_bottom(4, 1.0);
	back_middleLeft_bottom[0] = m;
	back_middleLeft_bottom[1] = y;
	back_middleLeft_bottom[2] = -z;

	//back middle right bottom
	std::vector<float> back_middleRight_bottom(4, 1.0);
	back_middleRight_bottom[0] = m + m;
	back_middleRight_bottom[1] = y;
	back_middleRight_bottom[2] = -z;

	//back middle right top
	std::vector<float> back_middleRight_top(4, 1.0);
	back_middleRight_top[0] = m + m;
	back_middleRight_top[1] = y + m;
	back_middleRight_top[2] = -z;

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

	//front middle left top
	std::vector<float> front_middleLeft_top(4, 1.0);
	front_middleLeft_top[0] = m;
	front_middleLeft_top[1] = y + m;
	front_middleLeft_top[2] = z;

	//front middle left bottom
	std::vector<float> front_middleLeft_bottom(4, 1.0);
	front_middleLeft_bottom[0] = m;
	front_middleLeft_bottom[1] = y;
	front_middleLeft_bottom[2] = z;

	//front middle right bottom
	std::vector<float> front_middleRight_bottom(4, 1.0);
	front_middleRight_bottom[0] = m + m;
	front_middleRight_bottom[1] = y;
	front_middleRight_bottom[2] = z;

	//front middle right top
	std::vector<float> front_middleRight_top(4, 1.0);
	front_middleRight_top[0] = m + m;
	front_middleRight_top[1] = y + m;
	front_middleRight_top[2] = z;

	//back wall
	int startIndex = addTriangle(pVertices, left_top_back, left_bottom_back, right_bottom_back, 0);
	int normalIndex = setNormal(normalArray, left_bottom_back, left_top_back, right_bottom_back, 0);
	startIndex = addTriangle(pVertices, right_bottom_back, right_top_back, left_top_back, startIndex);

	//back pillar wall
	startIndex = addTriangle(pVertices, back_middleLeft_top, back_middleLeft_bottom, back_middleRight_bottom, startIndex);
	normalIndex = setNormal(normalArray, back_middleLeft_bottom, back_middleLeft_top, back_middleRight_bottom, normalIndex);
	startIndex = addTriangle(pVertices, back_middleRight_bottom, back_middleRight_top, back_middleLeft_top, startIndex);

	//right wall
	startIndex = addTriangle(pVertices, right_bottom_back, right_bottom_front, right_top_back, startIndex);
	normalIndex = setNormal(normalArray, right_bottom_back, right_top_back, right_bottom_front, normalIndex);
	startIndex = addTriangle(pVertices, right_bottom_front, right_top_front, right_top_back, startIndex);

	//right pillar wall
	startIndex = addTriangle(pVertices, back_middleRight_bottom, front_middleRight_bottom, back_middleRight_top, startIndex);
	normalIndex = setNormal(normalArray, back_middleRight_bottom, back_middleRight_top, front_middleRight_bottom, normalIndex);
	startIndex = addTriangle(pVertices, front_middleRight_bottom, front_middleRight_top, back_middleRight_top, startIndex);

	//front wall
	startIndex = addTriangle(pVertices, right_bottom_front, left_bottom_front, right_top_front, startIndex);
	normalIndex = setNormal(normalArray, right_bottom_front, right_top_front, left_bottom_front, normalIndex);
	startIndex = addTriangle(pVertices, left_bottom_front, left_top_front, right_top_front, startIndex);

	//front pillar wall
	startIndex = addTriangle(pVertices, front_middleRight_bottom, front_middleLeft_bottom, front_middleRight_top, startIndex);
	normalIndex = setNormal(normalArray, front_middleRight_bottom, front_middleRight_top, front_middleLeft_bottom, normalIndex);
	startIndex = addTriangle(pVertices, front_middleLeft_bottom, front_middleLeft_top, front_middleRight_top, startIndex);

	//left wall
	startIndex = addTriangle(pVertices, left_bottom_front, left_bottom_back, left_top_front, startIndex);
	normalIndex = setNormal(normalArray, left_bottom_front, left_top_front, left_bottom_back, normalIndex);
	startIndex = addTriangle(pVertices, left_bottom_back, left_top_back, left_top_front, startIndex);

	//left pillar wall
	startIndex = addTriangle(pVertices, front_middleLeft_bottom, back_middleLeft_bottom, front_middleLeft_top, startIndex);
	normalIndex = setNormal(normalArray, front_middleLeft_bottom, front_middleLeft_top, back_middleLeft_bottom, normalIndex);
	startIndex = addTriangle(pVertices, back_middleLeft_bottom, back_middleLeft_top, front_middleLeft_top, startIndex);

	//bottom wall
	startIndex = addTriangle(pVertices, left_bottom_front, right_bottom_front, right_bottom_back, startIndex);
	normalIndex = setNormal(normalArray, right_bottom_front, left_bottom_front, right_bottom_back, normalIndex);
	startIndex = addTriangle(pVertices, right_bottom_back, left_bottom_back, left_bottom_front, startIndex);

	//top wall
	startIndex = addTriangle(pVertices, left_top_back, right_top_back, right_top_front, startIndex);
	normalIndex = setNormal(normalArray, right_top_back, left_top_back, right_top_front, normalIndex);
	startIndex = addTriangle(pVertices, right_top_front, left_top_front, left_top_back, startIndex);

	//top pillar wall
	startIndex = addTriangle(pVertices, back_middleLeft_top, back_middleRight_top, front_middleRight_top, startIndex);
	normalIndex = setNormal(normalArray, back_middleRight_top, back_middleLeft_top, front_middleRight_top, normalIndex);
	startIndex = addTriangle(pVertices, front_middleRight_top, front_middleLeft_top, back_middleLeft_top, startIndex);


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
	int tank_track::setNormal(GLfloat* normalArray, std::vector<float> origin, std::vector<float> first_point, std::vector<float> second_point, int index)
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

	int tank_track::addTriangle(GLfloat* pVertices, std::vector<float> point1, std::vector<float> point2, std::vector<float> point3, int startIndex)
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

	void tank_track::drawTrack()
	{
		glBindBuffer(GL_ARRAY_BUFFER, trackBufferObject);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, normalsBufferObject);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, this->colourBuffer);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glDrawArrays(GL_TRIANGLES, 0, 66);
	}
