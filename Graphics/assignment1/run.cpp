/* Link to static libraries*/
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glloadD.lib")
#pragma comment(lib, "opengl32.lib")

/* Include the header to the GLFW wrapper class which
also includes the OpenGL extension initialisation*/
#include "wrapper_glfw.h"
#include <iostream>
/* Include GLM core and matrix extensions*/
#include "cylinder.h"
#include "cuboid.h"
#include "sphere.h"
#include "track.h"
#include "body.h"

glm::mat4 projection;
GLuint light_mode;
GLuint program, vao;			/*shader & vertex array object*/
GLfloat speed_r, speed_l;
GLfloat aspect_ratio;			/* Aspect ratio of the window defined in the reshape callback*/
GLfloat width, height;			/*window width & height*/
GLfloat view_x, view_y, view_z;
GLuint projectionID, modelViewID, normal_matrixID, shininessID, ambientID, specularID, diffuseID, light_posID, emisiveID, global_ambientID, lightModeID;
glm::vec3 lightPosition, global_ambient;

track* trackOne;
track* trackTwo;
sphere* lightOne;
sphere* lightTwo;
body* tankBody;
transformation* globalTransform;
cylinder* testC;


void init(wrapper_glfw *glw)
{
	aspect_ratio = width / height;
	lightPosition = glm::vec3(20.0, 0.0, 100.0);
	global_ambient = glm::vec3(0.05);
	view_x = 0;
	view_y = 0;
	view_z = 5;
	speed_r = 1;
	speed_l = 1;
	light_mode = 0;
	globalTransform = new transformation();
	fprintf(stderr, "VENDOR: %s\n", (char *)glGetString(GL_VENDOR));
	fprintf(stderr, "VERSION: %s\n", (char *)glGetString(GL_VERSION));
	fprintf(stderr, "RENDERER: %s\n", (char *)glGetString(GL_RENDERER));
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	std::cout << "WINDOW SIZE = " << width << ':' << height << '\n';
	std::cout << "Aspect Ratio  = " << aspect_ratio << '\n';
	std::cout << '\n';
	std::cout << "Key 1 = move lamp left" << '\n';
	std::cout << "Key 2 = move lamp right" << '\n';
	std::cout << "Key 3 = move lamp down" << '\n';
	std::cout << "Key 4 = move lamp up" << '\n';
	std::cout << "Key 5 = move lamp forward" << '\n';
	std::cout << "Key 6 = move lamp back" << '\n';
	std::cout << "Key 9 = make lamp brighter" << '\n';
	std::cout << "Key 0 = make lamp dimmer" << '\n';
	std::cout << "Key up = move view up" << '\n';
	std::cout << "Key down = move view down" << '\n';
	std::cout << "Key left = move view left" << '\n';
	std::cout << "Key right = move view right" << '\n';
	std::cout << "Key . = move right tracks faster" << '\n';
	std::cout << "Key , = move right tracks slower" << '\n';
	std::cout << "Key X = move left tracks faster" << '\n';
	std::cout << "Key Z = move left tracks slower" << '\n';
	std::cout << "Key m = change light mode" << '\n';

	try
	{
		program = glw->LoadShader("Shaders/myfile.vert", "Shaders/myfile.frag");
	}
	catch (std::exception &e)
	{
		std::cout << "Caught exception: " << e.what() << std::endl;
		std::cin.ignore();
		exit(0);
	}

	// Generate index (name) for one vertex array object
	glGenVertexArrays(1, &vao);

	// Create the vertex array object and make it current
	glBindVertexArray(vao);

	lightOne = new sphere(200, 200, 0.5, 10.0);
	lightOne->light->setDiffuse(1.0, 1.0, 1.0);
	lightOne->light->emitLight(true);
	lightOne->transform->scaleUniform(-0.9);
	lightOne->transform->translate(1.0, 'y');

	lightTwo = new sphere(200, 200, 0.5, 10.0);
	lightTwo->light->setDiffuse(1.0, 1.0, 1.0);
	lightTwo->light->emitLight(true);
	lightTwo->transform->scaleUniform(-0.9);

	tankBody = new body(90.0, 0.1);
	tankBody->light->setDiffuse(1.0, 0.0, 0.0);
	tankBody->transform->rotate(180.0, 'y');

	testC = new cylinder(0.2, 50.0);
	//testC->light->setDiffuse(1.0, 0.0, 0.0);

	trackOne = new track();
	trackTwo = new track();
	trackOne->getTrack()->transform->translate(-0.9, 'z');
	trackTwo->getTrack()->transform->translate(0.9, 'z');

	//Uniform locations
	projectionID = glGetUniformLocation(program, "projection");
	modelViewID = glGetUniformLocation(program, "model_view");
	shininessID = glGetUniformLocation(program, "shininess");
	ambientID = glGetUniformLocation(program, "ambient");
	specularID = glGetUniformLocation(program, "specular_colour");
	diffuseID = glGetUniformLocation(program, "diffuse_colour");
	light_posID = glGetUniformLocation(program, "light_pos");
	normal_matrixID = glGetUniformLocation(program, "normal_matrix");
	emisiveID = glGetUniformLocation(program, "emissive");
	global_ambientID = glGetUniformLocation(program, "global_ambient");
	lightModeID = glGetUniformLocation(program, "light_mode");
}

void setUniforms(glm::mat4 view, glm::mat4 model, lighting* light)
{
	glm::mat4 model_view = view * model;
	glm::mat3 normal_matrix = glm::transpose(glm::inverse(glm::mat3(model_view)));
	glUniformMatrix4fv(modelViewID, 1, GL_FALSE, &model_view[0][0]);
	glUniformMatrix3fv(normal_matrixID, 1, GL_FALSE, &normal_matrix[0][0]);

	glUniform1f(ambientID, light->getAmbient());
	glUniform1f(shininessID, light->getShininess());
	glUniform3fv(specularID, 1, &light->getSpecular()[0]);
	glUniform3fv(emisiveID, 1, &light->getEmisive()[0]);
}

void display()
{
	/* Define the background colour */
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	/* Clear the colour and frame buffers */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* Enable depth test  */
	glEnable(GL_DEPTH_TEST);

	/* Make the compiled shader program current */
	glUseProgram(program);
	glUniform3fv(global_ambientID, 1, &global_ambient[0]);

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	projection = glm::perspective(30.0f, aspect_ratio, 0.1f, 100.0f);
	glUniformMatrix4fv(projectionID, 1, GL_FALSE, &projection[0][0]);

	glm::mat4 view = glm::lookAt(
		glm::vec3(0, 0, 5), // Camera is at (0,0,4), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
		);

	glm::vec3 lightPosition_p;
	glm::vec4 lightPosition_h = view * glm::vec4(lightOne->transform->getCoords(), 1.0);

	glm::mat4 model = glm::mat4(1.0f);

	if (light_mode)
	{
		lightPosition_p = glm::vec3(lightPosition_h.x, lightPosition_h.y, lightPosition_h.z);
		glUniform3fv(light_posID, 1, &lightPosition_p[0]);

		model = model * lightOne->transform->getModel();
		setUniforms(view, model, lightOne->light);
		lightOne->drawSphere();
	}
	else
	{
		glUniform3fv(light_posID, 1, &lightPosition[0]);
	}
	glUniform1ui(lightModeID, light_mode);

	model = globalTransform->getModel();
	
	model = model * testC->transform->getModel();
	setUniforms(view, model, testC->light);
	testC->drawCyclinder();

	//track* tracks[2] = { trackOne, trackTwo };
	//model = model * tankBody->transform->getModel();
	//setUniforms(view, model, tankBody->light);
	//tankBody->drawBody();
	//for (int j = 0; j < 2; j++)
	//{
	//	for (int i = 0; i < tracks[j]->getTracks().size(); i++)
	//	{
	//		model = globalTransform->getModel();
	//		model = model * tracks[j]->getTrack()->transform->getModel();
	//		model = model * tracks[j]->getTracks()[i]->getModel();
	//		setUniforms(view, model, tracks[j]->getTrack()->light);
	//		tracks[j]->getTrack()->drawTrack();
	//	}
	//}

	//trackOne->moveForward(speed_r);
	//trackTwo->moveForward(speed_l);
	glDisableVertexAttribArray(0);
	glUseProgram(0);
}

/* Called whenever the window is resized. The new window size is given, in pixels. */
static void reshape(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	aspect_ratio = float(w) / float(h);
}

/* change view angle, exit upon ESC */
static void keyCallback(GLFWwindow* window, int key, int s, int action, int mods)
{
	/* Enable this call if you want to disable key responses to a held down key*/
	//if (action != GLFW_PRESS) return;

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == '1') lightOne->transform->translate(-0.05, 'x');
	if (key == '2') lightOne->transform->translate(0.05, 'x');
	if (key == '3') lightOne->transform->translate(-0.05, 'y');
	if (key == '4') lightOne->transform->translate(0.05, 'y');
	if (key == '5') lightOne->transform->translate(-0.05, 'z');
	if (key == '6') lightOne->transform->translate(0.05, 'z');

	if (key == GLFW_KEY_UP) globalTransform->rotate(1.0, 'x');
	if (key == GLFW_KEY_DOWN) globalTransform->rotate(-1.0, 'x');
	if (key == GLFW_KEY_LEFT) globalTransform->rotate(1.0, 'y');
	if (key == GLFW_KEY_RIGHT) globalTransform->rotate(-1.0, 'y');
	if (key == GLFW_KEY_KP_ADD) globalTransform->rotate(1.0, 'z');
	if (key == GLFW_KEY_KP_SUBTRACT) globalTransform->rotate(-1.0, 'z');


	if (action == GLFW_PRESS)
	{
		if (key == ',') speed_r -= 1;
		if (key == '.') speed_r += 1;
		if (key == 'X') speed_l += 1;
		if (key == 'Z') speed_l -= 1;
		if (key == 'M')
		{
			light_mode = !light_mode;
			std::cout << "lightmode changed: " << light_mode << std::endl;
		}
	}
}


/**
* taken from http://stackoverflow.com/questions/8690619/how-to-get-screen-resolution-in-c
*/
void getDesktopResolution(GLfloat& horizontal, GLfloat& vertical)
{
	RECT desktop;
	// Get a handle to the desktop window
	const HWND hDesktop = GetDesktopWindow();
	// Get the size of screen to the variable desktop
	GetWindowRect(hDesktop, &desktop);
	// The top left corner will have coordinates (0,0)
	// and the bottom right corner will have coordinates
	// (horizontal, vertical)
	horizontal = desktop.right - 30;
	vertical = desktop.bottom - 50;
}

/* Entry point of program */
int main(int argc, char* argv[])
{
	getDesktopResolution(width, height);
	char* message = "Dave Ogle - Assignment 1";
	wrapper_glfw *glw = new wrapper_glfw(width, height, message);

	if (!ogl_LoadFunctions())
	{
		fprintf(stderr, "ogl_LoadFunctions() failed. Exiting\n");
		return 0;
	}

	glw->setRenderer(display);
	glw->setKeyCallback(keyCallback);
	glw->setReshapeCallback(reshape);

	init(glw);

	glw->eventLoop();

	delete(glw);
	return 0;
}