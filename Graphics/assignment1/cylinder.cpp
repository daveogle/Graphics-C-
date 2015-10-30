#include "cylinder.h"
#define PI  3.14159265358979323846264338327950288   /* pi */
#define ARRAY_SIZE(array) (sizeof((array))/sizeof((array[0])))
#include <iostream>

cylinder::cylinder(GLfloat radius, GLfloat length, int definition, GLfloat ambient, GLfloat shininess)
{
	this->radius = radius;
	this->length = length;
	this->definition = definition; //number of verticies around the circle
	this->light = new lighting(shininess, ambient);
	this->transform = new transformation();
	makeCylinderVBO();
}

cylinder::~cylinder()
{
}

void cylinder::makeCylinderVBO()
{
	/* Calculate the number of vertices required in cylinder, add one for the central point*/
	GLuint numvertices = this->definition + 1; //* 2;

	//pVertices is total vertices * 3 - one for each x, y, z
	GLfloat* pVertices = new GLfloat[numvertices * 3];

	//pNormals is the same as pVertices - one normal for each vertice
	GLfloat* pNormals = new GLfloat[numvertices * 3];
	//GLfloat* pColours = new GLfloat[numvertices * 4];
	defineVertices(pVertices, pNormals);

	/* Create the vertex buffer for the cylinder */
	glGenBuffers(1, &this->cylinderBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, this->cylinderBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * ARRAY_SIZE(pVertices), pVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/* Store the normals in a buffer object */
	glGenBuffers(1, &this->cylinderNormals);
	glBindBuffer(GL_ARRAY_BUFFER, this->cylinderNormals);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * ARRAY_SIZE(pNormals), pNormals, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//number of indices is 3 points for each point around the edge (one triangle for each point)
	GLuint numindices = (this->definition * 3);
	GLuint* pindices = new GLuint[numindices];

	// fill "indices" to define triangle strips
	GLuint index = 0;		// Current index
	// Define indices for each triangle 
	for (int i = 0; i < this->definition; i++)
	{
		//first point always center of circle
		int first = 0;
		//second point
		int second = i+1;
		//third point
		int third = i + 2;

		pindices[index * 3] = first;
		pindices[index * 3 + 1] = second;
		pindices[index * 3 + 2] = third;
		index++;
	}

	glGenBuffers(1, &this->cylinderElementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->cylinderElementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numindices * sizeof(GLuint), pindices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	delete pindices;
	delete pVertices;
}
	//based on
	//https://www.opengl.org/discussion_boards/showthread.php/167115-Creating-cylinder
	void cylinder::defineVertices(GLfloat *pVertices, GLfloat *pNormals)
	{
		GLfloat DEG_TO_RADIAN = PI / 180.0;

		//number of pVertieces is total points * 3;
		this->num_pvertices = (this->definition + 1) * 3;
		GLfloat halfLength = this->length / 2;
		auto step = 3;

		//define vertex at the center/top of the cylider
		pVertices[0] = 0.0;
		pVertices[1] = halfLength;
		pVertices[2] = 0.0;

		//define normals for vertex at top of cylinder pointing up
		pNormals[0] = 0.0;
		pNormals[1] = 1.0;
		pNormals[2] = 0.0;

		//for every point around the circle
		for (auto i = 1; i < this->definition +1; i++)
		{
			GLfloat theta = float(i)*2.0*PI;
			GLfloat nextTheta = (float(i) + 1)* 2.0 * PI;

			GLfloat x = radius * glm::cos(theta * DEG_TO_RADIAN);
			GLfloat y = halfLength;
			GLfloat z = radius*glm::sin(theta * DEG_TO_RADIAN);
			std::cout <<i << ") " << x << ":" << y << ":" << z << std::endl;

			//define the point
			pVertices[step] = x;
			pVertices[step + 1] = y;
			pVertices[step + 2] = z;


			//define the normal
			pNormals[step] = 0.0;
			pNormals[step + 1] = 1.0;
			pNormals[step + 2] = 0.0;

			step = step + 3;
		}
	}

	void cylinder::drawCyclinder()
	{
		glBindBuffer(GL_ARRAY_BUFFER, this->cylinderBufferObject);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		/* Bind the sphere normals */
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, this->cylinderNormals);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		/* Bind the sphere colours */
		//glBindBuffer(GL_ARRAY_BUFFER, this->sphereColours);
		//glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
		//glEnableVertexAttribArray(1);

		//glPointSize(3.f);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		/* Bind the indexed vertex buffer */
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->cylinderElementbuffer);

		/* Draw triangle fan*/
		glDrawElements(GL_TRIANGLE_FAN, this->definition, GL_UNSIGNED_INT, (GLvoid*)(0));
	}