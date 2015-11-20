/*Modified from example code on blackboard*/
/* Link to static libraries*/
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glloadD.lib")
#pragma comment(lib, "opengl32.lib")

/* Include the header to the GLFW wrapper class which
also includes the OpenGL extension initialisation*/
#include "wrapper_glfw.h"
#include "transformation.h"
#include "lighting.h"
#include "object_ldr.h"
#include "cuboid.h"
#include "terrain_object.h"
#include <iostream>

/* Include GLM core and matrix extensions*/

glm::mat4 projection;
GLuint light_mode, numberOfLights, texture_mode;
GLuint program, vao;			/*shader & vertex array object*/
GLfloat aspect_ratio;			/* Aspect ratio of the window defined in the reshape callback*/
GLfloat width, height;			/*window width & height*/
GLfloat fixedLight_x, fixedLight_y, fixedLight_z;
GLuint projectionID, modelViewID, normal_matrixID, shininessID, ambientID, specularID, diffuseID, light_posID, emisiveID, 
	   global_ambientID, lightModeID, numberOfLightsID, textureModeID;
glm::vec3 lightPosition, global_ambient;

transformation *globalTransform;

//Terrain globals
terrain_object *heightfield;
int octaves;
GLfloat perlin_scale, perlin_frequency;
GLfloat land_size;
lighting *terrainLight;

object_ldr cube;
lighting *cubeLight;
transformation *cubeTransform;

object_ldr lightBulb;
lighting *bulbLight;
transformation *bulbOne;
transformation *bulbTwo;

void init(wrapper_glfw *glw)
{
	/* Create the heightfield object */
	octaves = 1;
	perlin_scale = 2.f;
	perlin_frequency = 1.f;
	land_size = 50.f;
	heightfield = new terrain_object(octaves, perlin_frequency, perlin_scale);
	heightfield->createTerrain(1000, 1000, land_size, land_size);
	heightfield->createObject();
	terrainLight = new lighting(2.0f, 0.2f);
	terrainLight->setDiffuse(0.0, 1.0, 0.0);

	texture_mode = 1;
	cubeLight = new lighting(50.0f, 0.2f);
	cubeLight->setDiffuse(1.0, 0.0, 0.0);
	cubeLight->setSpecular(1.0, 0.0, 0.0);
	cube.load_obj("../Objects/cube.obj");
	cube.smoothNormals();
	cube.createObject();

	//Load a texture for the object
	texture_mode = cube.loadTexture("../Textures/img.png");
	int loc = glGetUniformLocation(program, "tex1");
	if (loc >= 0) glUniform1i(loc, 0);

	cubeTransform = new transformation();
	cubeTransform->scaleUniform(-0.7);
	
	lightBulb.load_obj("../Objects/sphere.obj");
	lightBulb.createObject();
	
	bulbOne = new transformation();
	bulbTwo = new transformation();
	bulbLight = new lighting(90.0f, 0.2f);
	bulbLight->setDiffuse(1.0, 1.0, 0.0);
	bulbLight->emitLight(true);
	bulbLight->setEmisive(1.0, 1.0, 0.0);

	bulbOne->scaleUniform(-0.9);
	bulbOne->translate(1.0, 'y');
	bulbOne->translate(-1.0, 'x');

	bulbTwo->scaleUniform(-0.9);
	bulbTwo->translate(1.0, 'y');
	bulbTwo->translate(1.0, 'x');

	aspect_ratio = width / height;
	fixedLight_x = 0.0f;
	fixedLight_y = 100.0;
	fixedLight_z = 100.0;
	global_ambient = glm::vec3(0.05);
	numberOfLights = 1;
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

	std::cout << "Key 1 = move lamp1 left" << '\n';
	std::cout << "Key 2 = move lamp1 right" << '\n';
	std::cout << "Key 3 = move lamp1 down" << '\n';
	std::cout << "Key 4 = move lamp1 up" << '\n';
	std::cout << "Key 5 = move lamp1 forward" << '\n';
	std::cout << "Key 6 = move lamp1 back" << '\n';
	std::cout << "Key A = move lamp2 left" << '\n';
	std::cout << "Key D = move lamp2 right" << '\n';
	std::cout << "Key S = move lamp2 down" << '\n';
	std::cout << "Key W = move lamp2 up" << '\n';
	std::cout << "Key E = move lamp2 forward" << '\n';
	std::cout << "Key R = move lamp2 back" << '\n';
	std::cout << "Key up = move view up" << '\n';
	std::cout << "Key down = move view down" << '\n';
	std::cout << "Key left = move view left" << '\n';
	std::cout << "Key right = move view right" << '\n';
	std::cout << "Key M = change light mode" << '\n';
	std::cout << "Key T = change texture mode" << '\n';
	std::cout << std::endl;

	try
	{
		program = glw->LoadShader("../Shaders/a2.vert", "../Shaders/a2.frag");
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
	textureModeID = glGetUniformLocation(program, "texture_mode");
	numberOfLightsID = glGetUniformLocation(program, "numberOfLights");
}

void setUniforms(glm::mat4 view, glm::mat4 model, lighting* light)
{
	glm::mat4 model_view = view * model;
	glm::mat3 normal_matrix = glm::transpose(glm::inverse(glm::mat3(model_view)));
	glUniformMatrix4fv(modelViewID, 1, GL_FALSE, &model_view[0][0]);
	glUniformMatrix3fv(normal_matrixID, 1, GL_FALSE, &normal_matrix[0][0]);
	glUniform1f(ambientID, light->getAmbient());
	glUniform1f(shininessID, light->getShininess());
	glUniform3fv(diffuseID, 1, &light->getDiffuse()[0]);
	glUniform3fv(specularID, 1, &light->getSpecular()[0]);
	glUniform3fv(emisiveID, 1, &light->getEmisive()[0]);
}

void display()
{
	transformation* lights[2] = { bulbOne, bulbTwo };

	/* Define the background colour */
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	/* Clear the colour and frame buffers */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* Enable depth test  */
	glEnable(GL_DEPTH_TEST);

	/* Make the compiled shader program current */
	glUseProgram(program);
	glUniform3fv(global_ambientID, 1, &global_ambient[0]);
	glUniform1ui(textureModeID, 0);
	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	projection = glm::perspective(30.0f, aspect_ratio, 0.1f, 100.0f);
	glUniformMatrix4fv(projectionID, 1, GL_FALSE, &projection[0][0]);

	glm::mat4 view = glm::lookAt(
		glm::vec3(0, 0, 4), // Camera is at (0,0,4), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
		);

	glm::mat4 model = glm::mat4(1.0f);
	model = globalTransform->getModel(); //global transformations

	//set number of lights
	glm::vec3 lightPosition_p;
	GLfloat lightsPositions[2 * 3];//number of lights * xyz
	for (int i = 0; i < numberOfLights; i++)
	{
		int step = i * 3;
		if (light_mode)
		{
			numberOfLights = 2;
			glUniform1ui(numberOfLightsID, numberOfLights);
			glm::mat4 lightModel = model * lights[i]->getModel();
			setUniforms(view, lightModel, bulbLight);
			lightBulb.drawObject();
			glm::vec4 lightPosition_h = view * lightModel * glm::vec4(lights[i]->getCoords(), 1.0);
			lightPosition_p = glm::vec3(lightPosition_h.x, lightPosition_h.y, lightPosition_h.z);
			lightsPositions[step] = lightPosition_p.x;
			lightsPositions[step + 1] = lightPosition_p.y;
			lightsPositions[step + 2] = lightPosition_p.z;
			glUniform3fv(light_posID, i + 1, &lightsPositions[0]);
		}
		else
		{
			numberOfLights = 1;
			glUniform1ui(numberOfLightsID, numberOfLights);
			lightPosition = glm::vec3(fixedLight_x, fixedLight_y, fixedLight_z);
			lightsPositions[step] = lightPosition.x;
			lightsPositions[step + 1] = lightPosition.y;
			lightsPositions[step + 2] = lightPosition.z;
			glUniform3fv(light_posID, i + 1, &lightsPositions[0]);
		}
	}
	glUniform1ui(lightModeID, light_mode);
	setUniforms(view, model, terrainLight);
	heightfield->drawObject(0);

	glm::mat4 cubeModel = model * cubeTransform->getModel();
	setUniforms(view, cubeModel, cubeLight);
	glUniform1ui(textureModeID, texture_mode);
	cube.drawObject();

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

	if (key == '1') bulbOne->translate(-0.05, 'x');
	if (key == '2') bulbOne->translate(0.05, 'x');
	if (key == '3') bulbOne->translate(-0.05, 'y');
	if (key == '4') bulbOne->translate(0.05, 'y');
	if (key == '5') bulbOne->translate(-0.05, 'z');
	if (key == '6') bulbOne->translate(0.05, 'z');

	if (key == 'D') bulbTwo->translate(-0.05, 'x');
	if (key == 'A') bulbTwo->translate(0.05, 'x');
	if (key == 'R') bulbTwo->translate(-0.05, 'y');
	if (key == 'E') bulbTwo->translate(0.05, 'y');
	if (key == 'S') bulbTwo->translate(-0.05, 'z');
	if (key == 'W') bulbTwo->translate(0.05, 'z');

	if (key == GLFW_KEY_UP) globalTransform->rotate(1.5, 'x');
	if (key == GLFW_KEY_DOWN) globalTransform->rotate(-1.5, 'x');
	if (key == GLFW_KEY_LEFT) globalTransform->rotate(1.5, 'y');
	if (key == GLFW_KEY_RIGHT) globalTransform->rotate(-1.5, 'y');
	if (key == GLFW_KEY_KP_ADD) globalTransform->rotate(1.5, 'z');
	if (key == GLFW_KEY_KP_SUBTRACT) globalTransform->rotate(-1.5, 'z');
	if (key == '+') globalTransform->translate(0.5f, 'z');
	if (key == '-') globalTransform->translate(-0.5f, 'z');

	if (action == GLFW_PRESS)
	{
		if (key == 'M')
		{
			light_mode = !light_mode;
			std::cout << "lightmode changed: " << light_mode << std::endl;
		}
		if (key == 'T')
		{
			texture_mode = !texture_mode;
			std::cout << "texture mode changed: " << texture_mode << std::endl;
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
	char* message = "Dave Ogle - Assignment 2 - Not The Tank";
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