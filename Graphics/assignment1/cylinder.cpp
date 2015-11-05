#include "cylinder.h"
#define PI  3.14159265358979323846264338327950288   /* pi */
#define ARRAY_SIZE(array) (sizeof((array))/sizeof((array[0])))
#include <iostream>

cylinder::cylinder(GLfloat radius, GLfloat length, GLfloat ambient, GLfloat shininess)
{
	this->colour = glm::vec3(0.33, 0.368, 0.192);
	this->radius = radius;
	this->length = length;
	this->definition = 100; //number of verticies around the circle
	this->light = new lighting(shininess, ambient);
	this->transform = new transformation();
	makeCylinderVBO();
}

cylinder::~cylinder()
{
}

void cylinder::makeCylinderVBO()
{
	defineVertices();

	GLuint pindices[204];
	for (int i = 0; i < 101; i++)
	{
		pindices[i] = i;
	}
	pindices[101] = 1;
	for (int i = 102; i < 203; i++)
	{
		pindices[i] = i -1;
	}
	pindices[103] = 101;
	this->isize = (sizeof(pindices) / sizeof(*pindices));
	glGenBuffers(1, &this->cylinderElementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->cylinderElementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, isize * sizeof(GLuint), pindices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
	//based on
	//https://www.opengl.org/discussion_boards/showthread.php/167115-Creating-cylinder
	void cylinder::defineVertices()
	{
		GLfloat DEG_TO_RADIAN = PI / 180.0;


		glm::vec3 vertices[202];
		glm::vec3 normals[202];
		glm::vec3 colour[202];

		//number of pVertieces is total points * 3;
		GLfloat halfLength = this->length / 2;
		auto step = 3;

		//define vertex at the center/top of the cylider
		vertices[0] = glm::vec3(0, halfLength, 0);

		//for every point around the circle
		for (auto i = 1; i < this->definition +1; i++)
		{
			GLfloat theta = float(i) * (2.0*PI);
			//GLfloat nextTheta = (float(i) + 1)* 2.0 * PI;

			GLfloat x = radius * glm::cos(theta * DEG_TO_RADIAN);
			GLfloat y = halfLength;
			GLfloat z = radius*glm::sin(theta * DEG_TO_RADIAN);

			vertices[i] = glm::vec3(x, y, z);
			normals[i] = glm::vec3(0.0, 1.0, 0.0);
			colour[i] = glm::vec3(0.1, 0.0, 0.0);
		}
		vertices[101] = glm::vec3(0, -halfLength, 0);

		//for every point around the circle
		for (auto i = 101; i < (this->definition*2) + 2; i++)
		{
			GLfloat theta = float(i) * (2.0*PI);
			//GLfloat nextTheta = (float(i) + 1)* 2.0 * PI;

			GLfloat x = radius * glm::cos(theta * DEG_TO_RADIAN);
			GLfloat y = -halfLength;
			GLfloat z = radius*glm::sin(theta * DEG_TO_RADIAN);

			vertices[i] = glm::vec3(x, y, z);
			normals[i] = glm::vec3(0.0, 1.0, 0.0);
			colour[i] = glm::vec3(-0.1, 0.0, 0.0);
		}
		/* Create the vertex buffer for the cylinder */
		glGenBuffers(1, &this->cylinderBufferObject);
		glBindBuffer(GL_ARRAY_BUFFER, this->cylinderBufferObject);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * (this->definition*2) + 2 * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glGenBuffers(1, &this->cylinderNormals);
		glBindBuffer(GL_ARRAY_BUFFER, cylinderNormals);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * (this->definition*2) + 2 * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		/* Store the colours in a buffer object */
		glGenBuffers(1, &this->cylinderColours);
		glBindBuffer(GL_ARRAY_BUFFER, cylinderColours);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)* (this->definition*2) +2, &colour[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void cylinder::drawCyclinder()
	{
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, cylinderBufferObject);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		/* Bind the normals */
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, cylinderNormals);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, this->cylinderColours);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->cylinderElementbuffer);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawElements(GL_TRIANGLE_FAN, 101, GL_UNSIGNED_INT, (GLvoid*)0);
		glDrawElements(GL_TRIANGLE_FAN, 6, GL_UNSIGNED_INT, (GLvoid*)(101));
	}