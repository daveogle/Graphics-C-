#include "cylinder.h"
#define PI  3.14159265358979323846264338327950288   /* pi */
#define ARRAY_SIZE(array) (sizeof((array))/sizeof((array[0])))
#include <iostream>

cylinder::cylinder(GLfloat ambient, GLfloat shininess, glm::vec3 colour)
{
	this->colour = colour;
	this->radius = 1.0f;
	this->length = 1.0f;
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

	GLuint pindices[406]; //204 //201
	for (int i = 0; i < 101; i++)
	{
		pindices[i] = i;
	}
	pindices[101] = 1;
	for (int i = 102; i < 203; i++)
	{
		pindices[i] = i -1;
	}
	pindices[203] = 102;
	//std::cout << std::endl;
	for (int i = 204; i < 404; i++)
	{
		//std::cout << i << " = " << i - 2 <<  std::endl;;
		pindices[i] = i - 2;
	}
	pindices[404] = 202;
	pindices[405] = 203;
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
		GLuint numberOfvertices = 402;
		glm::vec3 vertices[402];
		glm::vec3 normals[402];
		glm::vec3 colour[402];

		//number of pVertieces is total points * 3;
		GLfloat halfLength = this->length / 2;

		//define vertex at the center/top of the cylider
		vertices[0] = glm::vec3(0, halfLength, 0);
		normals[0] = glm::vec3(0.0, 1.0, 0.0);
		colour[0] = this->colour;

		//for every point around the circle
		for (int i = 1; i < this->definition +1; i++)
		{
			GLfloat theta = (2 * PI) / this->definition * i;

			GLfloat x = radius * cos(theta);
			GLfloat y = halfLength;
			GLfloat z = radius*sin(theta);

			vertices[i] = glm::vec3(x, y, z);
			normals[i] = glm::vec3(0.0, 1.0, 0.0);
			colour[i] = this->colour;
		}
		vertices[101] = glm::vec3(0, -halfLength, 0);
		normals[101] = glm::vec3(0.0, -1.0, 0.0);
		colour[101] = this->colour;

		//for every point around the circle
		for (int i = 102; i < (this->definition*2) + 2; i++)
		{
			GLfloat theta = (2 * PI) / this->definition * (i - 102);
			
			GLfloat x = radius * cos(theta);
			GLfloat y = -halfLength;
			GLfloat z = radius* sin(theta);

			vertices[i] = glm::vec3(x, y, z);
			normals[i] = glm::vec3(0.0, -1.0, 0.0);
			colour[i] = this->colour;
		}

		//sides				202								402
		int top = 1;
		int bottom = 102;
		for (int i = ((this->definition * 2) + 2); i < numberOfvertices; i += 2)
		{
			vertices[i] = vertices[top];
			normals[i] = glm::vec3(vertices[top].x, 0.0, vertices[top].z);
			colour[i] = this->colour;
			vertices[i + 1] = vertices[bottom];
			normals[i + 1] = glm::vec3(vertices[bottom].x, 0.0, vertices[bottom].z);
			colour[i + 1] = this->colour;
			top++;
			bottom++;
		}

		/* Create the vertex buffer for the cylinder */
		glGenBuffers(1, &this->cylinderBufferObject);
		glBindBuffer(GL_ARRAY_BUFFER, this->cylinderBufferObject);
		glBufferData(GL_ARRAY_BUFFER, (sizeof(glm::vec3) * numberOfvertices), &vertices[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glGenBuffers(1, &this->cylinderNormals);
		glBindBuffer(GL_ARRAY_BUFFER, cylinderNormals);
		glBufferData(GL_ARRAY_BUFFER, (sizeof(glm::vec3) * numberOfvertices), &normals[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		/* Store the colours in a buffer object */
		glGenBuffers(1, &this->cylinderColours);
		glBindBuffer(GL_ARRAY_BUFFER, cylinderColours);
		glBufferData(GL_ARRAY_BUFFER, (sizeof(glm::vec3)* numberOfvertices), &colour[0], GL_STATIC_DRAW);
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

		glPointSize(3.f);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->cylinderElementbuffer);

		glDrawElements(GL_TRIANGLE_FAN, 102, GL_UNSIGNED_INT, (GLvoid*)0);
		glDrawElements(GL_TRIANGLE_FAN, 102, GL_UNSIGNED_INT, (GLvoid*)(102 * sizeof(GLuint))); 
		glDrawElements(GL_TRIANGLE_STRIP, 202, GL_UNSIGNED_INT, (GLvoid*)(204 * sizeof(GLuint))); 
	}