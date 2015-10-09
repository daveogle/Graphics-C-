/* Basic GLFW3 Example and GLFW example
   using a modern OpenGL context.
   
   To build this source file in Visual Studio
	- Create lib and include directories
	- Copy the GLFW and GLLoad libs into the lib directory
	- Copy the GLLoad and GLFW headers into the include directory
	- Add the lib path to the VC++ lib directory (right click on project, then properties)
	- Add the include path to the VC++ include directory (right click on project, then properties)
	- Note that you may have to rebuild the libs from source if they don't work on
	  your system
   */

/* Link to static libraries, could define these as linker inputs in the project settings instead
if you prefer */
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glloadD.lib")
#pragma comment(lib, "opengl32.lib")

/* Include standard libraries and the GLFW3 header*/
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

/* Inlcude GL_Load and GLFW */
#include <glload/gl_4_0.h>
#include <glload/gl_load.h>
#include <GLFW/glfw3.h>

/* Define some global objects that we'll use to render */
GLuint positionBufferObject;
GLuint program;
GLuint vao;
GLfloat x;
GLfloat y;
GLfloat inc;

/* Array of vertex positions */
GLfloat vertexPositions[] = {
	0.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 1.0f,
	0.5f, 1.0f, 0.0f, 1.0f,
	-1.0f, 0.0f, 0.0f, 1.0f,
	0.0f, -0.0f, 0.0f, 1.0f,
	-0.5f, -1.0f, 0.0f, 1.0f,
};

/* Build shaders from strings containing shader source code */
GLuint BuildShader(GLenum eShaderType, const std::string &shaderText)
{
	GLuint shader = glCreateShader(eShaderType);
	const char *strFileData = shaderText.c_str();
	glShaderSource(shader, 1, &strFileData, NULL);

	glCompileShader(shader);

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		// Output the compile errors

		GLint infoLogLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);

		const char *strShaderType = NULL;
		switch (eShaderType)
		{
		case GL_VERTEX_SHADER: strShaderType = "vertex"; break;
		case GL_GEOMETRY_SHADER: strShaderType = "geometry"; break;
		case GL_FRAGMENT_SHADER: strShaderType = "fragment"; break;
		}

		std::cerr << "Compile error in " << strShaderType << "\n\t" << strInfoLog << std::endl;
		delete[] strInfoLog;

		throw std::exception("Shader compile exception");
	}

	return shader;
}

/* Error callback, outputs error to stl error stream */
static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}

/* Key response callback */
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_A && action == GLFW_PRESS)
		if (y > 0.15)
		{
			y -= 0.05;
			std::cout << y << std::endl;
		}

	if (key == GLFW_KEY_D && action == GLFW_PRESS)
		if (y <= 1)
		{
			y += 0.05;
			std::cout << y << std::endl;
		}
}

/* Window reshape callback
   Called whenever the window is resized. The new window size is given, in pixels. */
static void reshape(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

/* Our own initialisation function */
void init()
{
	/* Define animation variables */
	x = 0;
	y = vertexPositions[4];
	inc = 0.001;

	/* Create a vertex buffer object to store our array of vertices */
	/* A vertext buffer is a memory object that is created and owned by 
	   the OpenGL context */
	/* Generate buffer names (unique index identifiers) */
	glGenBuffers(1, &positionBufferObject);

	/* Specify the current active buffer object by identifer */
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);

	/* Allocates OpenGL memory for storing data or indices, any data 
	   previously defined is deleted*/
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_DYNAMIC_DRAW);

	/* Stop using buffer object for target (GL_ARRAY_BUFFER) because buffer name = 0*/
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/* Define the vertex shader code as a string */
	const std::string vertexShader(
		"#version 330\n"
		"layout(location = 0) in vec4 position;\n"
		"void main()\n"
		"{\n"
		"gl_PointSize = 10.0;\n"
		"   gl_Position = position;\n"
		"}\n"
		);

	/* Define the fragment shader as a string */
	const std::string fragmentShader(
		"#version 330\n"
		"out vec4 outputColor;\n"
		"void main()\n"
		"{\n"
		"   outputColor = vec4(1.0f, 0.0f, 1.0f, 1.0f);\n"
		"}\n"
		);

	/* Build both shaders */
	GLuint vertShader = BuildShader(GL_VERTEX_SHADER, vertexShader);
	GLuint fragShader = BuildShader(GL_FRAGMENT_SHADER, fragmentShader);

	/* Create a shader program object and link the vertex and fragment shaders
	into a single shader program */
	program = glCreateProgram();
	glAttachShader(program, vertShader);
	glAttachShader(program, fragShader);
	glLinkProgram(program);

	/* Output and shader compilation errors */
	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		if (!glext_ARB_debug_output)
		{
			GLint infoLogLength;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

			GLchar *strInfoLog = new GLchar[infoLogLength + 1];
			glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
			fprintf(stderr, "Linker failure: %s\n", strInfoLog);
			delete[] strInfoLog;
		}
		throw std::runtime_error("Shader could not be linked.");
	}
}


/* Rendering function */
void display()
{
	vertexPositions[0] = x;
	vertexPositions[4] = y;

	/* Update the vertext buffer object with the modified array of vertices */
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_DYNAMIC_DRAW);

	/* Define the background colour*/
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	/* Set the current shader program to be used */
	glUseProgram(program);

	/* Set the current active buffer object */
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);

	/* Specifies where the data values accociated with index can accessed in the vertex shader */
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

	/* Enable  the vertex array associated with the index*/
	glEnableVertexAttribArray(0);

	/* Constructs a sequence of geometric primitives using the elements from the currently
	   bound matrix */
	glDrawArrays(GL_TRIANGLES, 0, 6);

	/* Disable vertex array and shader program */
	glDisableVertexAttribArray(0);
	glUseProgram(0);
}

/* Standard main progrm */
int main(void)
{
    GLFWwindow* window;

	/* Register the error callback first to enable any GLFW errors to be processed*/
    glfwSetErrorCallback(error_callback);

	/* Init GLFW */
    if (!glfwInit())
        exit(EXIT_FAILURE);

	/* Create a GLFW window, bail out if it doesn't work */
    window = glfwCreateWindow(1200, 600, "Dave's Custom Title", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

	/* Associate an OpenGL context with the recently created GLFW window */
    glfwMakeContextCurrent(window);

	/* Initialise GLLoad library. You must have obtained a current OpenGL context or this will fail */
	/* We need this to link to modern OpenGL function*/
	if (!ogl_LoadFunctions())
	{
		fputs("oglLoadFunctions() failed. Exiting", stderr);
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	/* Register callbacks for keyboard and window resize */
    glfwSetKeyCallback(window, key_callback);
	glfwSetFramebufferSizeCallback(window, reshape);

	/* Call our own function to perform any setup work*/
	init();

	/* The event loop */
    while (!glfwWindowShouldClose(window))
    {
		/* Call our own drawing function */
		display();

		/* Swap buffers: GLFW is double buffered as standard */
        glfwSwapBuffers(window);

		/* Processes registered events, causes the callbacks to be called.*/
        glfwPollEvents();

		/* Modify our animatino variables */
		x += inc;
		if (x >= 1 || x<= 0) inc = -inc;
    }

	/* Clean up */
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
