#include "transformation.h"
#include <iostream>


transformation::transformation()
{
	this->angle_x = 0.0;
	this->angle_x_inc = 0.0;
	this->angle_y = 0.0;
	this->angle_y_inc = 0.0;
	this->angle_z = 0.0;
	this->angle_z_inc = 0.0;
	this->scale_x_value = 1;
	this->scale_y_value = 1;
	this->scale_z_value = 1;
	this->x_tanslate = 0.0;
	this->y_tanslate = 0.0;
	this->z_tanslate = 0.0;
}

transformation::~transformation()
{
}

glm::mat4 transformation::getModel()
{
	this->angle_x += this->angle_x_inc;
	this->angle_y += this->angle_y_inc;
	this->angle_z += this->angle_z_inc;
	// Define the model transformations for the cube
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(x_tanslate, y_tanslate, z_tanslate));
	model = glm::rotate(model, -angle_x, glm::vec3(1, 0, 0)); //rotating in clockwise direction around x-axis
	model = glm::rotate(model, -angle_y, glm::vec3(0, 1, 0)); //rotating in clockwise direction around y-axis
	model = glm::rotate(model, -angle_z, glm::vec3(0, 0, 1)); //rotating in clockwise direction around z-axis
	model = glm::scale(model, glm::vec3(scale_x_value, scale_y_value, scale_z_value));
	return model;
}

glm::mat4 transformation::getModel(glm::mat4 currentModel)
{
	this->angle_x += this->angle_x_inc;
	this->angle_y += this->angle_y_inc;
	this->angle_z += this->angle_z_inc;
	// Define the model transformations for the cube
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(currentModel, glm::vec3(x_tanslate, y_tanslate, z_tanslate));
	model = glm::scale(model, glm::vec3(scale_x_value, scale_y_value, scale_z_value));
	model = glm::rotate(model, -angle_x, glm::vec3(1, 0, 0)); //rotating in clockwise direction around x-axis
	model = glm::rotate(model, -angle_y, glm::vec3(0, 1, 0)); //rotating in clockwise direction around y-axis
	model = glm::rotate(model, -angle_z, glm::vec3(0, 0, 1)); //rotating in clockwise direction around z-axis
	return model;
}

glm::vec3 transformation::getCoords()
{
	return glm::vec3(this->x_tanslate, this->y_tanslate, this->z_tanslate);
}

void transformation::rotate(GLfloat amount, char plane)
{
	switch (plane)
	{
	case 'x':
		this->angle_x = this->angle_x + amount;
		break;
	case 'y':
		this->angle_y = this->angle_y + amount;
		break;
	case 'z':
		this->angle_z = this->angle_z + amount;
		break;
	default:
		std::cout << "Error no plane " << plane << " was found" << std::endl;
	}
}

void transformation::scale(GLfloat amount, char plane)
{
	switch (plane)
	{
	case 'x':
		this->scale_x_value = this->scale_x_value + amount;
		break;
	case 'y':
		this->scale_y_value = this->scale_y_value + amount;
		break;
	case 'z':
		this->scale_z_value = this->scale_z_value + amount;
		break;
	default:
		std::cout << "Error no plane " << plane << " was found" << std::endl;
	}
}

void transformation::scaleUniform(GLfloat amount)
{
	this->scale_x_value = this->scale_x_value + amount;
	this->scale_y_value = this->scale_y_value + amount;
	this->scale_z_value = this->scale_z_value + amount;
}

void transformation::translate(GLfloat amount, char plane)
{
	switch (plane)
	{
	case 'x':
		this->x_tanslate = this->x_tanslate + amount;
		break;
	case 'y':
		this->y_tanslate = this->y_tanslate + amount;
		break;
	case 'z':
		this->z_tanslate = this->z_tanslate + amount;
		break;
	default:
		std::cout << "Error no plane " << plane << " was found" << std::endl;
	}
}

void transformation::spin(GLfloat increment, char plane)
{
	switch (plane)
	{
	case 'x':
		this->angle_x_inc = this->angle_x_inc + increment;
		break;
	case 'y':
		this->angle_y_inc = this->angle_y_inc + increment;
		break;
	case 'z':
		this->angle_z_inc = this->angle_z_inc + increment;
		break;
	default:
		std::cout << "Error no plane " << plane << " was found" << std::endl;
	}
}