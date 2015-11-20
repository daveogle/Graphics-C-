#pragma once
#include <glload/gl_load.h>
#include <glload/gl_4_0.h>
#include <glm/detail/type_vec3.hpp>

class lighting
{

	GLfloat shininess, ambient_value;
	glm::vec3 specular_value, diffuse_value, emisive_value;
	GLboolean emit;

public:
	lighting(GLfloat shininess, GLfloat ambient);
	~lighting();
	GLfloat getAmbient();
	GLfloat getShininess();
	glm::vec3 getSpecular();
	glm::vec3 getDiffuse();
	glm::vec3 getEmisive();
	void emitLight(GLboolean emit);
	void setAmbient(GLfloat ambient);
	void setShininess(GLfloat shininess);
	void setSpecular(GLfloat colour_one, GLfloat colour_two, GLfloat colour_three);
	void setDiffuse(GLfloat colour_one, GLfloat colour_two, GLfloat colour_three);
	void setEmisive(GLfloat colour_one, GLfloat colour_two, GLfloat colour_three);
};

