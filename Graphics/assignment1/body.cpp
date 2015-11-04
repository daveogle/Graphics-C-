#include "body.h"


body::body(GLfloat shininess, GLfloat ambient)
{
	this->light = new lighting(shininess, ambient);
	this->transform = new transformation();
	defineVeritices();
}


body::~body()
{
}

void body::defineVeritices()
{
	int numberOfVerticies = 20;

	GLfloat width = 1.0;
	GLfloat depth = 2.5;
	GLfloat hight = 1.0;

	glm::vec3 normals[20];
	GLfloat x = width;
	GLfloat y = hight / 5;
	GLfloat z = depth / 2;
	GLfloat inset = width / 2;
	GLfloat incline = width / 3;
	int numberOfSquares = 5;

	// Define vertices as glm:vec3
	glm::vec3 vertices[] = {

		//bottom
		glm::vec3(x, y, -z), glm::vec3(x, y, z), glm::vec3(x, y - 0.1, -z), glm::vec3(x, y - 0.1, z),
		glm::vec3(x, y - 0.1, -z), glm::vec3(x, y - 0.1, z), glm::vec3(x - incline, -y, -z + inset), glm::vec3(x - incline, -y, z - inset),
		glm::vec3(x - incline, -y, -z + inset), glm::vec3(x - incline, -y, z - inset), glm::vec3(-x + incline, -y, -z + inset), glm::vec3(-x + incline, -y, z - inset),
		glm::vec3(-x + incline, -y, -z + inset), glm::vec3(-x + incline, -y, z - inset), glm::vec3(-x, y - 0.1, -z), glm::vec3(-x, y - 0.1, z),
		glm::vec3(-x, y - 0.1, -z), glm::vec3(-x, y - 0.1, z), glm::vec3(-x, y, -z), glm::vec3(-x, y, z),
	};

	glGenBuffers(1, &this->bodyBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, bodyBufferObject);
	glBufferData(GL_ARRAY_BUFFER, numberOfVerticies * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//define normals
	int step = 0;

	for (int j = 0; j < numberOfSquares; j++)
	{
		glm::vec3 V = glm::vec3(vertices[step + 1] - vertices[step]);
		glm::vec3 W = glm::vec3(vertices[step + 2] - vertices[step]);
		glm::vec3 xProduct = glm::normalize(glm::cross(V, W));

		for (int i = step; i < step + 4; i++)
		{
			normals[i] = xProduct;
		}
		step += 4;
	}


	glGenBuffers(1, &this->normalsBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, normalsBufferObject);
	glBufferData(GL_ARRAY_BUFFER, numberOfVerticies * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	GLuint pindices[] = { 0, 1, 2,
						  2, 3, 1,
						  4, 5, 6,
						  6, 7, 5,
						  8, 9, 10,
						  10, 11, 9,
						  12, 13, 14,
						  14, 15, 13,
						  16, 17, 18,
						  18, 19, 17, };

	glGenBuffers(1, &elementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 30 * sizeof(GLuint), pindices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void body::drawBody()
{
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, bodyBufferObject);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	/* Bind the normals */
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, normalsBufferObject);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawElements(GL_TRIANGLE_STRIP, 30, GL_UNSIGNED_INT, (GLvoid*)0);
}