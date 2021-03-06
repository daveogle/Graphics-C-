#include "body.h"


body::body(GLfloat shininess, GLfloat ambient)
{
	this-> colour = glm::vec3(0.33, 0.368, 0.192);
	this->numberOfVerticies = 40;
	this->light = new lighting(shininess, ambient);
	this->transform = new transformation();
	this->base_cylider = new cylinder(0.2, 70.0, this->colour);
	this->gun_transformation = new transformation();
	defineVeritices();
}


body::~body()
{
}

void body::defineVeritices()
{
	GLfloat width = 1.5;
	GLfloat depth = 1.5;
	GLfloat hight = 1.0;

	glm::vec3 normals[40];
	glm::vec3 pColours[40];
	GLfloat x = width;
	GLfloat y = hight / 5;
	GLfloat z = depth / 2;
	GLfloat inset = depth / 6;
	GLfloat incline = width / 3;
	int numberOfSquares = 10;

	// Define vertices as glm:vec3
	glm::vec3 vertices[] = {

		//bottom
/*0*/	glm::vec3(x, y, -z), glm::vec3(x, y, z), glm::vec3(x, y - 0.1, -z), glm::vec3(x, y - 0.1, z),
/*4*/	glm::vec3(x, y - 0.1, -z), glm::vec3(x, y - 0.1, z), glm::vec3(x - incline, -y, -z + inset), glm::vec3(x - incline, -y, z - inset),
/*8*/	glm::vec3(x - incline, -y, -z + inset), glm::vec3(x - incline, -y, z - inset), glm::vec3(-x + incline, -y, -z + inset), glm::vec3(-x + incline, -y, z - inset),
/*12*/	glm::vec3(-x + incline, -y, -z + inset), glm::vec3(-x + incline, -y, z - inset), glm::vec3(-x, y - 0.1, -z), glm::vec3(-x, y - 0.1, z),
/*16*/	glm::vec3(-x, y - 0.1, -z), glm::vec3(-x, y - 0.1, z), glm::vec3(-x, y, -z), glm::vec3(-x, y, z),

		//side long strips
/*20*/	glm::vec3(-x, y, -z), glm::vec3(x, y, -z), glm::vec3(-x, y - 0.1, -z), glm::vec3(x, y - 0.1, -z),
/*24*/  glm::vec3(x, y, z), glm::vec3(-x, y, z), glm::vec3(x, y - 0.1, z), glm::vec3(-x, y - 0.1, z),

		//side squares
/*28*/  glm::vec3(-x + incline, y - 0.1, -z), glm::vec3(x - incline, y - 0.1, -z), glm::vec3(-x + incline, -y, -z + inset), glm::vec3(x - incline, -y, -z + inset),
/*32*/  glm::vec3(x - incline, y - 0.1, z), glm::vec3(-x + incline, y - 0.1, z), glm::vec3(x - incline, -y, z - inset), glm::vec3(-x + incline, -y, z - inset),

/*36*/  glm::vec3(-x, y, -z), glm::vec3(-x, y, z), glm::vec3(x, y, -z), glm::vec3(x, y, z),

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
						  18, 19, 17,
						  20, 21, 22,
						  22, 23, 21,
						  24, 25, 26,
						  26, 27, 25,
						  28, 29, 30,
						  30, 31, 29,
						  32, 33, 34, 
						  34, 35, 33,
						  36, 37, 38,
						  38, 39, 37,
						  22, 28, 30,
						  26, 32, 34,
	                      29, 23, 31,
						  33, 27, 35};	
	this->isize = (sizeof(pindices) / sizeof(*pindices));
	glGenBuffers(1, &elementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, isize * sizeof(GLuint), pindices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/* Define colours as the x,y,z components of the sphere vertices */
	for (int i = 0; i < this->numberOfVerticies; i++)
	{
		pColours[i] = this->colour;
	}

	/* Store the colours in a buffer object */
	glGenBuffers(1, &this->colourBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colourBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)* this->numberOfVerticies, pColours, GL_STATIC_DRAW);
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

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, this->colourBuffer);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawElements(GL_TRIANGLES, this->isize, GL_UNSIGNED_INT, (GLvoid*)0);
}

glm::vec3 body::getColour()
{
	return this->colour;
}

std::vector<transformation*> body::getGunTransformations()
{
	std::vector<transformation*> gun_transformation(3);
	
	transformation* turret = new transformation();
	transformation* barrel = new transformation();
	transformation* muzzel = new transformation();

	turret->scale(-0.7, 'y');
	turret->scale(-0.35, 'x');
	turret->scale(-0.35, 'z');
	turret->translate(0.3, 'y');
	gun_transformation[0] = turret;

	barrel->scaleUniform(-0.95);
	barrel->scale(1.5, 'x');
	barrel->rotate(-90, 'z');
	barrel->translate(-1.3, 'x');
	barrel->translate(0.34, 'y');
	gun_transformation[1] = barrel;

	muzzel->scaleUniform(-0.92);
	muzzel->scale(0.1, 'x');
	muzzel->rotate(-90, 'z');
	muzzel->translate(-2.1, 'x');
	muzzel->translate(0.34, 'y');
	gun_transformation[2] = muzzel;
	return gun_transformation;
}

cylinder* body::getBaseCylider()
{
	return this->base_cylider;
}

glm::mat4 body::spinTurret(float amount)
{
	glm::mat4 turretModel = glm::mat4(1.0f);
	turretModel = glm::rotate(turretModel, -amount, glm::vec3(0, 1, 0));
	return turretModel;
}