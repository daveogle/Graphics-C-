#include "cylinder.h"
#define PI  3.14159265358979323846264338327950288   /* pi */

	cylinder::cylinder(float radius, float length, int definition)
	{
		this->radius = radius;
		this->length = length;
		this->definition = definition;
		this->num_vertices = definition * 36;
		GLfloat* pVertices = new GLfloat[this->num_vertices];
		defineVertices(pVertices);

		/* Create the vertex buffer for the cylinder */
		glGenBuffers(1, &cylinderBufferObject);
		glBindBuffer(GL_ARRAY_BUFFER, cylinderBufferObject);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * this->num_vertices, pVertices, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	cylinder::~cylinder()
	{
	}

	//https://www.opengl.org/discussion_boards/showthread.php/167115-Creating-cylinder
	void cylinder::defineVertices(GLfloat *pVertices)
	{
		GLfloat halfLength = this->length / 2;
		auto step = 0;
		for (auto i = 0; i < this->definition; i++)
		{
			GLfloat theta = float(i)*2.0*PI;
			GLfloat nextTheta = (float(i) + 1)* 2.0 * PI;

			//vertex at the center/top of the cylider
			pVertices[step + 0] = 0.0;
			pVertices[step + 1] = halfLength;
			pVertices[step + 2] = 0.0;
			std::cout << step << '\n' << std::endl;

			//1st vertext at the edge of the top circle
			pVertices[step + 3] = radius*glm::cos(theta);
			pVertices[step + 4] = halfLength;
			pVertices[step + 5] = radius*sin(theta);

			//2nd vertext at the edge of the top circle
			pVertices[step + 6] = radius*cos(nextTheta);
			pVertices[step + 7] = halfLength;
			pVertices[step + 8] = radius*sin(nextTheta);

			//1st triangle side of cylinder
			pVertices[step + 9] = radius*glm::cos(theta);
			pVertices[step + 10] = halfLength;
			pVertices[step + 11] = radius*sin(theta);
			pVertices[step + 12] = radius*glm::cos(theta);
			pVertices[step + 13] = -halfLength;
			pVertices[step + 14] = radius*sin(theta);
			pVertices[step + 15] = radius*cos(nextTheta);
			pVertices[step + 16] = -halfLength;
			pVertices[step + 17] = radius*sin(nextTheta);

			//2nd triangle side of cylinder
			pVertices[step + 18] = radius*cos(nextTheta);
			pVertices[step + 19] = -halfLength;
			pVertices[step + 20] = radius*sin(nextTheta);
			pVertices[step + 21] = radius*cos(nextTheta);
			pVertices[step + 22] = halfLength;
			pVertices[step + 23] = radius*sin(nextTheta);
			pVertices[step + 24] = radius*glm::cos(theta);
			pVertices[step + 25] = halfLength;
			pVertices[step + 26] = radius*sin(theta);

			//2nd vertext at the edge of the bottom circle
			pVertices[step + 27] = radius*cos(nextTheta);
			pVertices[step + 28] = -halfLength;
			pVertices[step + 29] = radius*sin(nextTheta);

			//1st vertext at the edge of the bottom circle
			pVertices[step + 30] = radius*glm::cos(theta);
			pVertices[step + 31] = -halfLength;
			pVertices[step + 32] = radius*sin(theta);

			//vertex at the center/bottom of the cylider
			pVertices[step + 33] = 0.0;
			pVertices[step + 34] = -halfLength;
			pVertices[step + 35] = 0.0;
			step = step + 36;
		}
	}

	void cylinder::drawCyclinder()
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glBindBuffer(GL_ARRAY_BUFFER, cylinderBufferObject);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, this->num_vertices);
	}