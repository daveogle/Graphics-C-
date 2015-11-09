#include "lighting.h"


lighting::lighting(GLfloat shininess, GLfloat ambient)
{
	this->specular_value = glm::vec3(1.0);
	this->diffuse_value = glm::vec3(0.1, 0.1, 0.1);
	this->shininess = shininess;
	this->ambient_value = ambient;
	this->emit = false;
	this->emisive_value = glm::vec3(1.0, 1.0, 0.8);
}


lighting::~lighting()
{
}

GLfloat lighting::getAmbient()
{
	return this->ambient_value;
}

GLfloat lighting::getShininess()
{
	return this->shininess;
}

glm::vec3 lighting::getSpecular()
{
	return this->specular_value;
}

glm::vec3 lighting::getDiffuse()
{
	return this->diffuse_value;
}

glm::vec3 lighting::getEmisive()
{
	if (this->emit)
	{
		return this->emisive_value;
	}
	return glm::vec3(0);
}

void lighting::emitLight(GLboolean emit)
{
	this->emit = emit;
}

void lighting::setAmbient(GLfloat ambient)
{
	this->ambient_value = ambient;
}

void lighting::setShininess(GLfloat shininess)
{
	this->shininess = shininess;
}

void lighting::setSpecular(GLfloat colour_one, GLfloat colour_two, GLfloat colour_three)
{
	this->specular_value = glm::vec3(colour_one, colour_two, colour_three);
}

void lighting::setDiffuse(GLfloat colour_one, GLfloat colour_two, GLfloat colour_three)
{
	this->diffuse_value = glm::vec3(colour_one, colour_two, colour_three);
}

void lighting::setEmisive(GLfloat colour_one, GLfloat colour_two, GLfloat colour_three)
{
	this->emisive_value = glm::vec3(colour_one, colour_two, colour_three);
}