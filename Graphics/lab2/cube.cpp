#include "cube.h"
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glloadD.lib")
#pragma comment(lib, "opengl32.lib")

cube::cube(GLfloat vertexPositions[144])
{
	this->angle_x = 0;
	this->angle_x_inc = 0;
	this->angle_y = 0;
	this->angle_y_inc = 0;
	this->scale_value = 1;
	this->x_tanslate = 0;
	this->y_tanslate = 0;
	this->z_tanslate = 0;

	#define ARRAY_SIZE(array) (sizeof((array))/sizeof((array[0])))

	for (auto i = 0; i < ARRAY_SIZE(vertexPositions); i++)
	{
		this->vertexPositions[i] = vertexPositions[i];
	}
}

cube::~cube()
{
}

GLfloat cube::get_angle_x()
{
	return this->angle_x;
}

GLfloat cube::get_angle_x_inc()
{
	return this->angle_x_inc;
}

GLfloat cube::get_angle_y()
{
	return this->angle_y;
}

GLfloat cube::get_angle_y_inc()
{
	return this->angle_y_inc;
}

GLfloat cube::get_scale_value()
{
	return this->scale_value;
}

GLfloat cube::get_x_translate()
{
	return this->x_tanslate;
}

GLfloat cube::get_y_translate()
{
	return this->y_tanslate;
}

GLfloat cube::get_z_translate()
{
	return this->z_tanslate;
}

glm::mat4 cube::get_model()
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(x_tanslate, y_tanslate, z_tanslate));
	model = glm::scale(model, glm::vec3(scale_value, scale_value, scale_value));
	model = glm::rotate(model, -angle_y, glm::vec3(0, 1, 0));
	model = glm::rotate(model, -angle_x, glm::vec3(1, 0, 0)); //rotating in clockwise direction around x-axis
	return model;
}

void cube::set_angle_x(GLfloat x)
{
	this->angle_x = x;
}

void cube::set_angle_x_inc(GLfloat x_inc)
{
	this->angle_x_inc = x_inc;
}

void cube::set_angle_y(GLfloat y)
{
	this->angle_y = y;
}

void cube::set_angle_y_inc(GLfloat y_inc)
{
	this->angle_y_inc = y_inc;
}

void cube::set_scale_value(GLfloat scale)
{
	this->scale_value = scale;
}

void cube::set_x_translate(GLfloat x)
{
	this->x_tanslate = x;
}

void cube::set_y_translate(GLfloat y)
{
	this->y_tanslate = y;
}

void cube::set_z_translate(GLfloat z)
{
	this->z_tanslate = z;
}