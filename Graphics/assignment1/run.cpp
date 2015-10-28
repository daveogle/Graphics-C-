/* Link to static libraries*/
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glloadD.lib")
#pragma comment(lib, "opengl32.lib")

/* Include the header to the GLFW wrapper class which
also includes the OpenGL extension initialisation*/
#include "wrapper_glfw.h"
#include <iostream>
/* Include GLM core and matrix extensions*/
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include "cylinder.h"
#include "cuboid.h"
#include "sphere.h"

glm::mat4 projection;
GLuint program, vao;			/*shader & vertex array object*/
GLfloat aspect_ratio;			/* Aspect ratio of the window defined in the reshape callback*/
GLfloat width, height;			/*window width & height*/
GLuint projectionID, modelViewID, normal_matrixID, shininessID, ambientID, specularID, diffuseID, light_posID, emisiveID, global_ambientID;
glm::vec3 lightPosition, global_ambient;
//cylinder* testCylinder;
cuboid* testCube;
sphere* testSphere;
sphere* theLight;

void init(wrapper_glfw *glw)
{
	aspect_ratio = width / height;
	lightPosition = glm::vec3(100.0, 0.0, 50.0);
	global_ambient = glm::vec3(0.05);

	fprintf(stderr, "VENDOR: %s\n", (char *)glGetString(GL_VENDOR));
	fprintf(stderr, "VERSION: %s\n", (char *)glGetString(GL_VERSION));
	fprintf(stderr, "RENDERER: %s\n", (char *)glGetString(GL_RENDERER));
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	std::cout << "WINDOW SIZE = " << width << ':' << height << '\n';
	std::cout << "Aspect Ratio  = " << aspect_ratio << '\n';

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

	//testCylinder = new cylinder(1.0, 1.0, 100);

	theLight = new sphere(200, 200, 0.5, 10.0);
	theLight->light->setDiffuse(1.0, 1.0, 1.0);
	theLight->light->emitLight(true);
	theLight->transform->scaleUniform(-0.9);

	testCube = new cuboid(0.5, 0.5, 0.5, 0.05, 40.0);
	testCube->light->setDiffuse(1.0, 0.0, 0.0);
	testCube->transform->translate(1.0, 'x');
	testCube->transform->spin(0.5, 'z');
	testCube->transform->spin(0.5, 'y');

	testSphere = new sphere(200, 200, 0.05, 20.0);
	testSphere->light->setDiffuse(0.0, 1.0, 0.0);
	testSphere->transform->translate(-1.0, 'x');
	testSphere->transform->scaleUniform(-0.7);
	testSphere->transform->spin(-0.5, 'y');

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
}

template <class type> void setUniforms(glm::mat4 view, type shape)
{
	glm::mat4 model_view = view * shape->transform->getModel();
	glm::mat3 normal_matrix = glm::transpose(glm::inverse(glm::mat3(model_view)));
	glUniformMatrix4fv(modelViewID, 1, GL_FALSE, &model_view[0][0]);
	glUniformMatrix3fv(normal_matrixID, 1, GL_FALSE, &normal_matrix[0][0]);

	glUniform1f(ambientID, shape->light->getAmbient());
	glUniform1f(shininessID, shape->light->getShininess());
	glUniform3fv(specularID, 1, &shape->light->getSpecular()[0]);
	glUniform3fv(diffuseID, 1, &shape->light->getDiffuse()[0]);
	glUniform3fv(emisiveID, 1, &shape->light->getEmisive()[0]);
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

	// Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	projection = glm::perspective(30.0f, aspect_ratio, 0.1f, 100.0f);
	glUniformMatrix4fv(projectionID, 1, GL_FALSE, &projection[0][0]);

	glm::mat4 view = glm::lookAt(
		glm::vec3(0, 0, 4), // Camera is at (0,0,4), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
		);
	glm::vec4 lightPosition_h = view * glm::vec4(theLight->transform->getCoords(), 1.0);
	lightPosition = glm::vec3(lightPosition_h.x, lightPosition_h.y, lightPosition_h.z);
	glUniform3fv(light_posID, 1, &lightPosition[0]);

	setUniforms(view, theLight);
	theLight->drawSphere();

	setUniforms(view, testCube);
	testCube->drawCuboid();

	setUniforms(view, testSphere);
	testSphere->drawSphere();

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

	if (key == '1') theLight->transform->translate(-0.05, 'x');
	if (key == '2') theLight->transform->translate(0.05, 'x');
	if (key == '3') theLight->transform->translate(-0.05, 'y');
	if (key == '4') theLight->transform->translate(0.05, 'y');
	if (key == '5') theLight->transform->translate(-0.05, 'z');
	if (key == '6') theLight->transform->translate(0.05, 'z');
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