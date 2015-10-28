#include "sphere.h"


sphere::sphere(GLuint numlats, GLuint numlongs, GLfloat ambient, GLfloat shininess)
{
	this->numlats = numlats;		// Number of latitudes in our sphere
	this->numlongs = numlongs;		// Number of longitudes in our sphere
	this->light = new lighting(shininess, ambient);
	this->transform = new transformation();
	makeSphereVBO();
}

sphere::~sphere()
{
}

void sphere::makeSphereVBO()
{
	GLuint i, j;
	/* Calculate the number of vertices required in sphere */
	GLuint numvertices = 2 + ((numlats - 1) * numlongs);
	GLfloat* pVertices = new GLfloat[numvertices * 3];
	//GLfloat* pColours = new GLfloat[numvertices * 4];
	makeUnitSphere(pVertices, numlats, numlongs);

	/* Define colours as the x,y,z components of the sphere vertices */
	//for (i = 0; i < numvertices; i++)
	//{
	//	pColours[i * 4] = pVertices[i * 3];
	//	pColours[i * 4 + 1] = pVertices[i * 3 + 1];
	//	pColours[i * 4 + 2] = pVertices[i * 3 + 2];
	//	pColours[i * 4 + 3] = 1.f;
	//}

	/* Generate the vertex buffer object */
	glGenBuffers(1, &sphereBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, sphereBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * numvertices * 3, pVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/* Store the normals in a buffer object */
	glGenBuffers(1, &sphereNormals);
	glBindBuffer(GL_ARRAY_BUFFER, sphereNormals);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)* numvertices * 3, pVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/* Store the colours in a buffer object */
	//glGenBuffers(1, &sphereColours);
	//glBindBuffer(GL_ARRAY_BUFFER, sphereColours);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)* numvertices * 4, pColours, GL_STATIC_DRAW);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);

	/* Calculate the number of indices in our index array and allocate memory for it */
	GLuint numindices = ((numlongs * 2) + 2) * (numlats - 1) + ((numlongs + 2) * 2);
	GLuint* pindices = new GLuint[numindices];

	// fill "indices" to define triangle strips
	GLuint index = 0;		// Current index

	// Define indices for the first triangle fan for one pole
	for (i = 0; i < numlongs + 1; i++)
	{
		pindices[index++] = i;
	}
	pindices[index++] = 1;	// Join last triangle in the triangle fan

	GLuint start = 1;		// Start index for each latitude row
	for (j = 0; j < numlats - 2; j++)
	{
		for (i = 0; i < numlongs; i++)
		{
			pindices[index++] = start + i;
			pindices[index++] = start + i + numlongs;
		}
		pindices[index++] = start; // close the triangle strip loop by going back to the first vertex in the loop
		pindices[index++] = start + numlongs; // close the triangle strip loop by going back to the first vertex in the loop

		start += numlongs;
	}

	// Define indices for the last triangle fan for the south pole region
	for (i = numvertices - 1; i > (numvertices - numlongs - 2); i--)
	{
		pindices[index++] = i;
	}
	pindices[index] = numvertices - 2;	// Tie up last triangle in fan

	// Generate a buffer for the indices
	glGenBuffers(1, &elementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numindices * sizeof(GLuint), pindices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	delete pindices;
	//delete pColours;
	delete pVertices;
	this->numspherevertices = numvertices;
}

/* Define the vertex positions for a sphere. The array of vertices must have previosuly
been created.
*/
void sphere::makeUnitSphere(GLfloat *pVertices, GLuint numlats, GLuint numlongs)
{
	GLfloat DEG_TO_RADIANS = 3.141592f / 180.f;
	GLuint vnum = 0;
	GLfloat x, y, z, lat_radians, lon_radians;

	/* Define north pole */
	pVertices[0] = 0; pVertices[1] = 0; pVertices[2] = 1.f;
	vnum++;

	GLfloat latstep = 180.f / numlats;
	GLfloat longstep = 360.f / numlongs;

	/* Define vertices along latitude lines */
	for (GLfloat lat = 90.f - latstep; lat > -90.f; lat -= latstep)
	{
		lat_radians = lat * DEG_TO_RADIANS;
		for (GLfloat lon = -180.f; lon < 180.f; lon += longstep)
		{
			lon_radians = lon * DEG_TO_RADIANS;

			x = glm::cos(lat_radians) * glm::cos(lon_radians);
			y = glm::cos(lat_radians) * glm::sin(lon_radians);
			z = glm::sin(lat_radians);

			/* Define the vertex */
			pVertices[vnum * 3] = x; pVertices[vnum * 3 + 1] = y; pVertices[vnum * 3 + 2] = z;
			vnum++;
		}
	}
	/* Define south pole */
	pVertices[vnum * 3] = 0; pVertices[vnum * 3 + 1] = 0; pVertices[vnum * 3 + 2] = -1.f;
}

/* Draws the sphere form the previously defined vertex and index buffers */
void sphere::drawSphere()
{
	GLuint i;

	/* Draw the vertices as GL_POINTS */
	glBindBuffer(GL_ARRAY_BUFFER, this->sphereBufferObject);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	/* Bind the sphere normals */
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, this->sphereNormals);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	/* Bind the sphere colours */
	//glBindBuffer(GL_ARRAY_BUFFER, this->sphereColours);
	//glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
	//glEnableVertexAttribArray(1);

	glPointSize(3.f);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	/* Bind the indexed vertex buffer */
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

	/* Draw the north pole regions as a triangle  */
	glDrawElements(GL_TRIANGLE_FAN, numlongs + 2, GL_UNSIGNED_INT, (GLvoid*)(0));

	/* Calculate offsets into the indexed array. Note that we multiply offsets by 4
	because it is a memory offset the indices are type GLuint which is 4-bytes */
	GLuint lat_offset_jump = (numlongs * 2) + 2;
	GLuint lat_offset_start = numlongs + 2;
	GLuint lat_offset_current = lat_offset_start * 4;

	/* Draw the triangle strips of latitudes */
	for (i = 0; i < numlats - 2; i++)
	{
		glDrawElements(GL_TRIANGLE_STRIP, numlongs * 2 + 2, GL_UNSIGNED_INT, (GLvoid*)(lat_offset_current));
		lat_offset_current += (lat_offset_jump * 4);
	}
		/* Draw the south pole as a triangle fan */
		glDrawElements(GL_TRIANGLE_FAN, numlongs + 2, GL_UNSIGNED_INT, (GLvoid*)(lat_offset_current));
}