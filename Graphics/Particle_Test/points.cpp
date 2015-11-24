#include "points.h"
#include "glm/gtc/random.hpp"
#include <iostream>

/* Constructor, set initial parameters*/
points::points(GLuint number, GLfloat floor, GLfloat sp)
{
	numpoints = number;
	this->floor = floor;
	speed = sp;
}


points::~points()
{
	delete [] colours;
	delete[] vertices;
}

void points::updateParams(GLfloat floor, GLfloat sp)
{
	floor = floor;
	speed = sp;
}


void  points::create()
{
	vertices = new glm::vec4[numpoints];
	colours = new glm::vec3[numpoints];
	velocity = new glm::vec3[numpoints];

	/* Define random position and velocity */
	for (int i = 0; i < numpoints; i++)
	{
		int size = (rand() % 16) + 2;         // snowflake size from 2 to 15
		vertices[i] = glm::vec4(glm::ballRand(1.f), size);//random 3d vector within given radius
		colours[i] = glm::vec3(1.0, 1.0, 1.0);
		velocity[i] = glm::vec3(glm::ballRand(glm::linearRand(0.0, (double)speed)));
		velocity[i].y = glm::linearRand(0.0, (double)speed);
	}

	/* Create the vertex buffer object */
	/* and the vertex buffer positions */
	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, numpoints * sizeof(glm::vec4), vertices, GL_DYNAMIC_DRAW);

	glGenBuffers(1, &colour_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, colour_buffer);
	glBufferData(GL_ARRAY_BUFFER, numpoints * sizeof(glm::vec3), colours, GL_STATIC_DRAW);
}


void points::draw()
{
	/* Bind  vertices. Note that this is in attribute index 0 */
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

	/* Bind cube colours. Note that this is in attribute index 1 */
	glBindBuffer(GL_ARRAY_BUFFER, colour_buffer);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	/* Draw our points*/
	glDrawArrays(GL_POINTS, 0, numpoints);
}


void points::animate()
{
	for (int i = 0; i < numpoints; i++)
	{
		//strip snowflake size out of vertice
		glm::vec3 vertacie = glm::vec3(vertices[i].x, vertices[i].y, vertices[i].z);
		GLfloat size = vertices[i].w;

		// Add velocity to the vertices 
		vertacie -= velocity[i];

		// Calculate distance to the floor
		GLfloat dist = vertacie.y;//glm::length(vertacie);

		// If at the floor, then move back to top
		if (dist < floor)
		{
			vertacie.y = 1.0;
		}

		//add snowflake size back in
		vertices[i] = glm::vec4(vertacie, size);
	}

	// Update the vertex buffer data
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, numpoints * sizeof(glm::vec4), vertices, GL_DYNAMIC_DRAW);
}



