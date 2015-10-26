#pragma once
#include <glload/_int_gl_type.h>
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"

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

public:
	transformation();
	transformation(GLfloat angle_x, GLfloat angle_x_inc, GLfloat angle_y, GLfloat angle_y_inc,
		GLfloat angle_z, GLfloat angle_z_inc, GLfloat scale_x_value, GLfloat scale_y_value, GLfloat scale_z_value,
		GLfloat x_tanslate, GLfloat y_tanslate, GLfloat z_tanslate);
	~transformation();
	glm::mat4 getModel();
	void rotate(GLfloat amount, char plane);
	void scale(GLfloat amount, char plane);
	void translate(GLfloat amount, char plane);
	void spin(GLfloat increment, char plane);
};

