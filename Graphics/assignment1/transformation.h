#pragma once
#include <glload/_int_gl_type.h>
#include "glm/gtc/matrix_transform.hpp"
#include <stack>

class transformation
{
	GLfloat angle_x;
	GLfloat angle_x_inc;
	GLfloat angle_y;
	GLfloat angle_y_inc;
	GLfloat angle_z;
	GLfloat angle_z_inc;
	GLfloat scale_x_value;
	GLfloat scale_y_value;
	GLfloat scale_z_value;
	GLfloat x_tanslate;
	GLfloat y_tanslate;
	GLfloat z_tanslate;
	//std::stack<glm::m> transformation_stack;

public:
	transformation();
	transformation(GLfloat angle_x, GLfloat angle_x_inc, GLfloat angle_y, GLfloat angle_y_inc,
		GLfloat angle_z, GLfloat angle_z_inc, GLfloat scale_x_value, GLfloat scale_y_value, GLfloat scale_z_value,
		GLfloat x_tanslate, GLfloat y_tanslate, GLfloat z_tanslate);
	~transformation();
	glm::mat4 getModel();
	glm::vec3 getCoords();
	void rotate(GLfloat amount, char plane);
	void scale(GLfloat amount, char plane);
	void scaleUniform(GLfloat amount);
	void translate(GLfloat amount, char plane);
	void spin(GLfloat increment, char plane);
};

