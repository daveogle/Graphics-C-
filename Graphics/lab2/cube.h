#ifndef CUBE_H
#define CUBE_H

#pragma once

#include <glload/gl_4_0.h>
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/mat4x2.hpp>

class cube
{
	/* Position and view globals */
	GLfloat angle_x, angle_x_inc, angle_y, angle_y_inc, scale_value, x_tanslate, y_tanslate, z_tanslate;
	GLfloat vertexPositions[144];
	GLuint positionBufferObject;

public:
	explicit cube();
	~cube();
	GLfloat get_angle_x();
	GLfloat get_angle_x_inc();
	GLfloat get_angle_y();
	GLfloat get_angle_y_inc();
	GLfloat get_scale_value();
	GLfloat get_x_translate();
	GLfloat get_y_translate();
	GLfloat get_z_translate();
	glm::mat4 get_model();
	GLfloat* get_vertexPositions();
	GLuint get_positionBufferObject();
	void set_angle_x(GLfloat x);
	void set_angle_x_inc(GLfloat x_inc);
	void set_angle_y(GLfloat y);
	void set_angle_y_inc(GLfloat y_inc);
	void set_scale_value(GLfloat scale);
	void set_x_translate(GLfloat x);
	void set_y_translate(GLfloat y);
	void set_z_translate(GLfloat z);
	};

#endif

