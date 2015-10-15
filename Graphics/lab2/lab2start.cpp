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

/* GLM core */
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
//#include <glm/gtc/type_ptr.hpp>

GLuint positionBufferObject, colourObject;
GLuint program;
GLuint vao;

/* Position and view globals */
GLfloat angle_x, angle_x_inc, angle_y, angle_y_inc;

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

	angle_x = 0;
	angle_x_inc = 0;
	angle_y = 0;
	angle_y_inc = 0;

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
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, -angle_y, glm::vec3(0, 1, 0));
	model = glm::rotate(model, -angle_x, glm::vec3(1, 0, 0)); //rotating in clockwise direction around x-axis

	// Send our transformations to the currently bound shader,
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &model[0][0]);

	glDrawArrays(GL_TRIANGLES, 0, 36);

	glDisableVertexAttribArray(0);
	glUseProgram(0);

	/* Modify our animation variables */
	angle_x += angle_x_inc;
	angle_y += angle_y_inc;
}


/* Called whenever the window is resized. The new window size is given, in pixels. */
static void reshape(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

/* change view angle, exit upon ESC */
static void keyCallback(GLFWwindow* window, int k, int s, int action, int mods)
{
	if (action != GLFW_PRESS) return;
	if (k == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (k == GLFW_KEY_DOWN && action == GLFW_PRESS) angle_x_inc += 0.1f;
	if (k == GLFW_KEY_UP) angle_x_inc -= 0.1f;
	if (k == GLFW_KEY_RIGHT) angle_y_inc += 0.1f;
	if (k == GLFW_KEY_LEFT) angle_y_inc -= 0.1f;
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



