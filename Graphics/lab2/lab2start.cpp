/*
 Lab2start.cpp
 Creates a cube and defines a uniform variable to pass a transformation
 to the vertx shader.
 Use this example as a start to lab2 or extract bits and add to
 an example of your own to practice working with 3D transformations
 and uniform variables.
 Iain Martin October 2014
*/

/* Link to static libraries, could define these as linker inputs in the project settings instead
if you prefer */
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glloadD.lib")
#pragma comment(lib, "opengl32.lib")

/* Include the header to the GLFW wrapper class which
   also includes the OpenGL extension initialisation*/
#include "wrapper_glfw.h"
#include <iostream>
#include "cube.h";

/* GLM core */
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
//#include <glm/gtc/type_ptr.hpp>

GLuint positionBufferObject, colourObject;
GLuint program;
GLuint vao;
cube* cube1;
cube* cube2;

/* Uniforms*/
GLuint modelID;

/*
This function is called before entering the main rendering loop.
Use it for all your initialisation stuff
*/
void init(GLWrapper *glw)
{

	fprintf(stderr, "VENDOR: %s\n", (char *)glGetString(GL_VENDOR));
	fprintf(stderr, "VERSION: %s\n", (char *)glGetString(GL_VERSION));
	fprintf(stderr, "RENDERER: %s\n", (char *)glGetString(GL_RENDERER));

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);

	// Generate index (name) for one vertex array object
	glGenVertexArrays(1, &vao);

	// Create the vertex array object and make it current
	glBindVertexArray(vao);

	/* Define vertices for a cube in 12 triangles */
	GLfloat vertexPositions[] =
	{
		-0.25f, 0.25f, -0.25f, 1.f,
		-0.25f, -0.25f, -0.25f, 1.f,
		0.25f, -0.25f, -0.25f, 1.f,

		0.25f, -0.25f, -0.25f, 1.f,
		0.25f, 0.25f, -0.25f, 1.f,
		-0.25f, 0.25f, -0.25f, 1.f,

		0.25f, -0.25f, -0.25f, 1.f,
		0.25f, -0.25f, 0.25f, 1.f,
		0.25f, 0.25f, -0.25f, 1.f,

		0.25f, -0.25f, 0.25f, 1.f,
		0.25f, 0.25f, 0.25f, 1.f,
		0.25f, 0.25f, -0.25f, 1.f,

		0.25f, -0.25f, 0.25f, 1.f,
		-0.25f, -0.25f, 0.25f, 1.f,
		0.25f, 0.25f, 0.25f, 1.f,

		-0.25f, -0.25f, 0.25f, 1.f,
		-0.25f, 0.25f, 0.25f, 1.f,
		0.25f, 0.25f, 0.25f, 1.f,

		-0.25f, -0.25f, 0.25f, 1.f,
		-0.25f, -0.25f, -0.25f, 1.f,
		-0.25f, 0.25f, 0.25f, 1.f,

		-0.25f, -0.25f, -0.25f, 1.f,
		-0.25f, 0.25f, -0.25f, 1.f,
		-0.25f, 0.25f, 0.25f, 1.f,

		-0.25f, -0.25f, 0.25f, 1.f,
		0.25f, -0.25f, 0.25f, 1.f,
		0.25f, -0.25f, -0.25f, 1.f,

		0.25f, -0.25f, -0.25f, 1.f,
		-0.25f, -0.25f, -0.25f, 1.f,
		-0.25f, -0.25f, 0.25f, 1.f,

		-0.25f, 0.25f, -0.25f, 1.f,
		0.25f, 0.25f, -0.25f, 1.f,
		0.25f, 0.25f, 0.25f, 1.f,

		0.25f, 0.25f, 0.25f, 1.f,
		-0.25f, 0.25f, 0.25f, 1.f,
		-0.25f, 0.25f, -0.25f, 1.f
	};

	/* Define an array of colours */
	float vertexColours[] = {
		0.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,

		1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f,

		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,

		1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 1.0f,

		0.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f, 1.0f,
	};

	//create objects;
	cube1 = new cube(vertexPositions);
	cube1->set_x_translate(-0.70);
	cube2 = new cube(vertexPositions);
	cube2->set_x_translate((0.70));

	/* Create a vertex buffer object to store vertices */
	glGenBuffers(1, &positionBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/* Create a vertex buffer object to store vertex colours */
	glGenBuffers(1, &colourObject);
	glBindBuffer(GL_ARRAY_BUFFER, colourObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexColours), vertexColours, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glEnable(GL_DEPTH_TEST);

	try
	{
		program = glw->LoadShader("lab2.vert", "lab2.frag");
	}
	catch (std::exception &e)
	{
		std::cout << "Caught exception: " << e.what() << std::endl;
		std::cin.ignore();
		exit(0);
	}

	/* Define uniforms to send to vertex shader */
	modelID = glGetUniformLocation(program, "model");
}

//Called to update the display.
//You should call glfwSwapBuffers() after all of your rendering to display what you rendered.
void display()
{
	/* Define the background colour */
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	/* Clear the colour and frame buffers */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(program);

	glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
	glEnableVertexAttribArray(0);

	/* glVertexAttribPointer(index, size, type, normalised, stride, pointer) 
	   index relates to the layout qualifier in the vertex shader and in 
	   glEnableVertexAttribArray() and glDisableVertexAttribArray() */
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, colourObject);
	glEnableVertexAttribArray(1);

	/* glVertexAttribPointer(index, size, type, normalised, stride, pointer)
	index relates to the layout qualifier in the vertex shader and in
	glEnableVertexAttribArray() and glDisableVertexAttribArray() */
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);

	// Model matrix : an identity matrix (model will be at the origin)
	glm::mat4 model = cube1->get_model();

	// Send our transformations to the currently bound shader,
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &model[0][0]);

	glDrawArrays(GL_TRIANGLES, 0, 36);

	glDisableVertexAttribArray(0);
	glUseProgram(0);

	/* Modify our animation variables */
	cube1->set_angle_x(cube1->get_angle_x() + cube1->get_angle_x_inc());
	cube1->set_angle_y(cube1->get_angle_y() + cube1->get_angle_y_inc());
}


/* Called whenever the window is resized. The new window size is given, in pixels. */
static void reshape(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

/* change view angle, exit upon ESC */
static void keyCallback(GLFWwindow* window, int k, int s, int action, int mods)
{
	const GLfloat modifer = 0.1f;

	if (action != GLFW_PRESS) return;
	if (k == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	switch (k)
	{
	case GLFW_KEY_DOWN:
		cube1->set_angle_x_inc(cube1->get_angle_x_inc() + modifer);
		break;
	case GLFW_KEY_UP:
		cube1->set_angle_x_inc(cube1->get_angle_x_inc() - modifer);
		break;
	case GLFW_KEY_RIGHT:
		cube1->set_angle_y_inc(cube1->get_angle_y_inc() + modifer);
		break;
	case GLFW_KEY_LEFT:
		cube1->set_angle_y_inc(cube1->get_angle_y_inc() - modifer);
		break;
	case GLFW_KEY_KP_ADD:
		cube1->set_scale_value(cube1->get_scale_value() + modifer);
		break;
	case GLFW_KEY_KP_SUBTRACT:
		cube1->set_scale_value(cube1->get_scale_value() - modifer);
		break;
	case GLFW_KEY_A:
		cube1->set_x_translate(cube1->get_x_translate() - modifer);
		break;
	case GLFW_KEY_D:
		cube1->set_x_translate(cube1->get_x_translate() + modifer);
		break;
	case GLFW_KEY_W:
		cube1->set_y_translate(cube1->get_y_translate() + modifer);
		break;
	case GLFW_KEY_S:
		cube1->set_y_translate(cube1->get_y_translate() - modifer);
		break;
	case GLFW_KEY_Z:
		cube1->set_z_translate(cube1->get_z_translate() + modifer);
		break;
	case GLFW_KEY_X:
		cube1->set_z_translate(cube1->get_z_translate() - modifer);
		break;
	}
}

/* An error callback function to output GLFW errors*/
static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

/* Entry point of program */
int main(int argc, char* argv[])
{
	GLWrapper *glw = new GLWrapper(1024, 768, "Lab2: Hello 3D");;

	if (!ogl_LoadFunctions())
	{
		fprintf(stderr, "ogl_LoadFunctions() failed. Exiting\n");
		return 0;
	}

	/* Note it you might want to move this call to the wrapper class */
	glw->setErrorCallback(error_callback);

	glw->setRenderer(display);
	glw->setKeyCallback(keyCallback);
	glw->setReshapeCallback(reshape);

	init(glw);

	glw->eventLoop();

	delete(glw);
	return 0;
}



