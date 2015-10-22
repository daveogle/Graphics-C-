/*
Lab3start
Creates a cube and a sphere
Includes rotation, scaling, translation, view and perspective transformations,
and is setup for you to add lighting because the objects have normals and colours defined
for all vertices.
Feel free to either use this example as a start to lab3 or extract bits and add to
an example of your own to practice implementing per-vertex lighting.
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
#include "sphere.h"
/* Include GLM core and matrix extensions*/
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>

/* Define buffer object indices */
GLuint positionBufferObject, colourObject, normalsBufferObject;
GLuint program;		/* Identifier for the shader program */
GLuint vao;			/* Vertex array (Container) object. This is the index of the VAO that will be the container for our buffer objects*/
GLuint colourmode;	/* Index of a uniform to switch the colour mode in the vertex shader
					I've included this to show you how to pass in an unsigned integer into
					your vertex shader. */

/* Position and view globals */
GLfloat angle_x, angle_inc_x, x, scale, z, y, ambient_value;
GLfloat angle_y, angle_inc_y, angle_z, angle_inc_z;
/* Uniforms*/
GLuint projectionID, modelViewID, normal_matrixID;
GLuint colourmodeID, ambientID, light_dirID;
glm::vec3 lightDirection;
GLfloat aspect_ratio;		/* Aspect ratio of the window defined in the reshape callback*/
sphere* new_sphere;
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

	std::cout << "Key 'Q' = spin negative X" << std::endl;
	std::cout << "Key 'W' = spin positive X" << std::endl;
	std::cout << "Key 'E' = spin negative Y" << std::endl;
	std::cout << "Key 'R' = spin positive Y" << std::endl;
	std::cout << "Key 'T' = spin negative Z" << std::endl;
	std::cout << "Key 'Y' = spin positive Z" << std::endl;
	std::cout << "Key 'A' = make smaller" << std::endl;
	std::cout << "Key 'S' = make bigger" << std::endl;
	std::cout << "Key 'Z' = move together" << std::endl;
	std::cout << "Key 'X' = move apart" << std::endl;
	std::cout << "Key 'C' = move cube down" << std::endl;
	std::cout << "Key 'V' = move cube up" << std::endl;
	std::cout << "Key 'B' = move cube back" << std::endl;
	std::cout << "Key 'N' = move cube forward" << std::endl;
	std::cout << "Key '1' = increase ambient value" << std::endl;
	std::cout << "Key '2' = decrease ambient value" << std::endl;
	std::cout << "Key 'M' = change colour mode" << std::endl;
	std::cout << "Key 'F' = change draw mode" << std::endl;

	/* Set the object transformation controls to their initial values */
	x = 0.05f;
	y = 0;
	z = 0;
	angle_x = angle_y = angle_z = 0;
	angle_inc_x = angle_inc_y = angle_inc_z = 0;
	scale = 1.f;
	aspect_ratio = 1024.f / 768.f;	// Initial aspect ratio from window size (variables would be better!)
	colourmode = 0;
	ambient_value = 0.2;
	lightDirection = glm::vec3(100.0, 100.0, 100.0);
	// Generate index (name) for one vertex array object
	glGenVertexArrays(1, &vao);

	// Create the vertex array object and make it current
	glBindVertexArray(vao);

	/* Define vertices for a cube in 12 triangles */
	GLfloat vertexPositions[] =
	{
		-0.25f, 0.25f, -0.25f,
		-0.25f, -0.25f, -0.25f,
		0.25f, -0.25f, -0.25f,

		0.25f, -0.25f, -0.25f,
		0.25f, 0.25f, -0.25f,
		-0.25f, 0.25f, -0.25f,

		0.25f, -0.25f, -0.25f,
		0.25f, -0.25f, 0.25f,
		0.25f, 0.25f, -0.25f,

		0.25f, -0.25f, 0.25f,
		0.25f, 0.25f, 0.25f,
		0.25f, 0.25f, -0.25f,

		0.25f, -0.25f, 0.25f,
		-0.25f, -0.25f, 0.25f,
		0.25f, 0.25f, 0.25f,

		-0.25f, -0.25f, 0.25f,
		-0.25f, 0.25f, 0.25f,
		0.25f, 0.25f, 0.25f,

		-0.25f, -0.25f, 0.25f,
		-0.25f, -0.25f, -0.25f,
		-0.25f, 0.25f, 0.25f,

		-0.25f, -0.25f, -0.25f,
		-0.25f, 0.25f, -0.25f,
		-0.25f, 0.25f, 0.25f,

		-0.25f, -0.25f, 0.25f,
		0.25f, -0.25f, 0.25f,
		0.25f, -0.25f, -0.25f,

		0.25f, -0.25f, -0.25f,
		-0.25f, -0.25f, -0.25f,
		-0.25f, -0.25f, 0.25f,

		-0.25f, 0.25f, -0.25f,
		0.25f, 0.25f, -0.25f,
		0.25f, 0.25f, 0.25f,

		0.25f, 0.25f, 0.25f,
		-0.25f, 0.25f, 0.25f,
		-0.25f, 0.25f, -0.25f,
	};

	/* Manually specified colours for our cube */
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

	/* Manually specified normals for our cube */
	GLfloat normals[] =
	{
		0, 0, -1.f, 0, 0, -1.f, 0, 0, -1.f,
		0, 0, -1.f, 0, 0, -1.f, 0, 0, -1.f,
		1.f, 0, 0, 1.f, 0, 0, 1.f, 0, 0,
		1.f, 0, 0, 1.f, 0, 0, 1.f, 0, 0,
		0, 0, 1.f, 0, 0, 1.f, 0, 0, 1.f,
		0, 0, 1.f, 0, 0, 1.f, 0, 0, 1.f,
		-1.f, 0, 0, -1.f, 0, 0, -1.f, 0, 0,
		-1.f, 0, 0, -1.f, 0, 0, -1.f, 0, 0,
		0, -1.f, 0, 0, -1.f, 0, 0, -1.f, 0,
		0, -1.f, 0, 0, -1.f, 0, 0, -1.f, 0,
		0, 1.f, 0, 0, 1.f, 0, 0, 1.f, 0,
		0, 1.f, 0, 0, 1.f, 0, 0, 1.f, 0,
	};

	/* Create the vertex buffer for the cube */
	glGenBuffers(1, &positionBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/* Create the colours buffer for the cube */
	glGenBuffers(1, &colourObject);
	glBindBuffer(GL_ARRAY_BUFFER, colourObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexColours), vertexColours, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/* Create the normals  buffer for the cube */
	glGenBuffers(1, &normalsBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, normalsBufferObject);
	glBufferData(GL_ARRAY_BUFFER, 36 * sizeof(glm::vec3), normals, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/* create the sphere object */
	new_sphere = new sphere();
	/* Load and build the vertex and fragment shaders */
	try
	{
		program = glw->LoadShader("lab3start.vert", "lab3start.frag");
	}
	catch (std::exception &e)
	{
		std::cout << "Caught exception: " << e.what() << std::endl;
		std::cin.ignore();
		exit(0);
	}

	/* Define uniforms to send to vertex shader */
	colourmodeID = glGetUniformLocation(program, "colourmode");
	projectionID = glGetUniformLocation(program, "projection");
	modelViewID = glGetUniformLocation(program, "model_view");
	ambientID = glGetUniformLocation(program, "ambient");
	light_dirID = glGetUniformLocation(program, "light_dir");
	normal_matrixID = glGetUniformLocation(program, "normal_matrix");
}

/* Called to update the display. Note that this function is called in the event loop in the wrapper
class because we registered display as a callback function */
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

	/* Bind cube vertices. Note that this is in attribute index 0 */
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	/* Bind cube colours. Note that this is in attribute index 1 */
	glBindBuffer(GL_ARRAY_BUFFER, colourObject);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);

	/* Bind cube normals. Note that this is in attribute index 2 */
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, normalsBufferObject);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// Define the model transformations for the cube
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(x + 0.5, y, z));
	model = glm::scale(model, glm::vec3(scale, scale, scale));//scale equally in all axis
	model = glm::rotate(model, -angle_x, glm::vec3(1, 0, 0)); //rotating in clockwise direction around x-axis
	model = glm::rotate(model, -angle_y, glm::vec3(0, 1, 0)); //rotating in clockwise direction around y-axis
	model = glm::rotate(model, -angle_z, glm::vec3(0, 0, 1)); //rotating in clockwise direction around z-axis

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(30.0f, aspect_ratio, 0.1f, 100.0f);

	// Camera matrix
	glm::mat4 view = glm::lookAt(
		glm::vec3(0, 0, 4), // Camera is at (0,0,4), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
		);

	//2. define model_view matrix
	glm::mat4 model_view = view * model;
	//4. define mat3 normal model_view trasformation matrix
	glm::mat3 normal_matrix = glm::transpose(glm::inverse(glm::mat3(model_view)));
	//normalize the light direction
	lightDirection = glm::normalize(lightDirection);
	// Send our uniforms variables to the currently bound shader,
	glUniform1ui(colourmodeID, colourmode);
	glUniformMatrix4fv(modelViewID, 1, GL_FALSE, &model_view[0][0]);
	glUniformMatrix4fv(projectionID, 1, GL_FALSE, &Projection[0][0]);
	glUniformMatrix3fv(normal_matrixID, 1, GL_FALSE, &normal_matrix[0][0]);
	glUniform1f(ambientID, ambient_value);
	glUniform3f(light_dirID, lightDirection[0], lightDirection[1], lightDirection[2]);
	/* Draw our cube*/
	glDrawArrays(GL_TRIANGLES, 0, 36);

	/* Define the model transformations for our sphere */
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-x - 0.5, 0, 0));
	model = glm::scale(model, glm::vec3(scale / 3.f, scale / 3.f, scale / 3.f));//scale equally in all axis
	model = glm::rotate(model, -angle_x, glm::vec3(1, 0, 0)); //rotating in clockwise direction around x-axis
	model = glm::rotate(model, -angle_y, glm::vec3(0, 1, 0)); //rotating in clockwise direction around y-axis
	model = glm::rotate(model, -angle_z, glm::vec3(0, 0, 1)); //rotating in clockwise direction around z-axis
	//create a model_view matrix
	model_view = view * model;
	//define the normal model-view matrix
	normal_matrix = glm::transpose(glm::inverse(glm::mat3(model_view)));
	/* Draw our sphere */
	glUniformMatrix4fv(modelViewID, 1, GL_FALSE, &model_view[0][0]);
	glUniformMatrix4fv(normal_matrixID, 1, GL_FALSE, &normal_matrix[0][0]);

	new_sphere -> drawSphere();

	glDisableVertexAttribArray(0);
	glUseProgram(0);

	/* Modify our animation variables */
	angle_x += angle_inc_x;
	angle_y += angle_inc_y;
	angle_z += angle_inc_z;
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

	if (key == 'Q') angle_inc_x -= 0.05f;
	if (key == 'W') angle_inc_x += 0.05f;
	if (key == 'E') angle_inc_y -= 0.05f;
	if (key == 'R') angle_inc_y += 0.05f;
	if (key == 'T') angle_inc_z -= 0.05f;
	if (key == 'Y') angle_inc_z += 0.05f;
	if (key == 'A') scale -= 0.02f;
	if (key == 'S') scale += 0.02f;
	if (key == 'Z') x -= 0.05f;
	if (key == 'X') x += 0.05f;
	if (key == 'C') y -= 0.05f;
	if (key == 'V') y += 0.05f;
	if (key == 'B') z -= 0.05f;
	if (key == 'N') z += 0.05f;
	if (key == GLFW_KEY_2) ambient_value += 0.02;
	if (key == GLFW_KEY_1) ambient_value -= 0.02;

	if (key == 'M' && action != GLFW_PRESS)
	{
		colourmode = !colourmode;
		std::cout << "colourmode= " << colourmode << std::endl;
	}

	/* Cycle between drawing vertices, mesh and filled polygons */
	if (key == 'F' && action != GLFW_PRESS)
	{
		if (new_sphere->incrementDrawmode(1) > 2) new_sphere->setDrawMode(0);
	}
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

	glw->setRenderer(display);
	glw->setKeyCallback(keyCallback);
	glw->setReshapeCallback(reshape);

	init(glw);

	glw->eventLoop();

	delete(glw);
	return 0;
}

