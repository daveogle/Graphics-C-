/* Link to static libraries*/
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glloadD.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "soil.lib")

/* Include the header to the GLFW wrapper class which
also includes the OpenGL extension initialisation*/
#include "wrapper_glfw.h"
#include <iostream>
/* Include GLM core and matrix extensions*/
#include "soil.h"
#include "cylinder.h"
#include "cuboid.h"
#include "sphere.h"
#include "track.h"
#include "body.h"

glm::mat4 projection;
GLuint light_mode, numberOfLights, texID;
GLuint program, vao;			/*shader & vertex array object*/
GLfloat speed_r, speed_l, turret_spin;
GLfloat aspect_ratio;			/* Aspect ratio of the window defined in the reshape callback*/
GLfloat width, height;			/*window width & height*/
GLfloat view_x, view_y, view_z;
GLfloat fixedLight_x, fixedLight_y, fixedLight_z;
GLuint projectionID, modelViewID, normal_matrixID, shininessID, ambientID, specularID, diffuseID, light_posID, emisiveID, 
	   global_ambientID, lightModeID, numberOfLightsID;
glm::vec3 lightPosition, global_ambient;

track* trackOne;
track* trackTwo;
sphere* lightOne;
sphere* lightTwo;
body* tankBody;
transformation* globalTransform;


void loadTexture(std::string filePath)
{
	const char *c_filePath = filePath.c_str();
	try
	{
		/* Not actually needed if using one texture at a time */
		//glActiveTexture(GL_TEXTURE0);

		/* load an image file directly as a new OpenGL texture */
		texID = SOIL_load_OGL_texture(c_filePath, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
			SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);

		/* check for an error during the load process */
		if (texID == 0)
		{
			printf("TexID SOIL loading error: '%s'\n", SOIL_last_result());
		}

		/* Standard bit of code to enable a uniform sampler for our texture */
		int loc = glGetUniformLocation(program, "tex1");
		if (loc >= 0) glUniform1i(loc, 0);
	}
	catch (std::exception &e)
	{
		printf("\nImage file loading failed.");
	}
}

void init(wrapper_glfw *glw)
{
	loadTexture("Textures/metal_tread.png");
	aspect_ratio = width / height;
	fixedLight_x = 0.0f;
	fixedLight_y = 0.0;
	fixedLight_z = 100.0;
	global_ambient = glm::vec3(0.005);
	numberOfLights = 2;
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
	std::cout << "Key . = move right tracks faster" << '\n';
	std::cout << "Key , = move right tracks slower" << '\n';
	std::cout << "Key X = move left tracks faster" << '\n';
	std::cout << "Key Z = move left tracks slower" << '\n';
	std::cout << "Key G = move turret left" << '\n';
	std::cout << "Key H = move turret right" << '\n';
	std::cout << "Key M = change light mode" << '\n';

	try
	{
		program = glw->LoadShader("Shaders/tank.vert", "Shaders/tank.frag");
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
	lightOne->transform->translate(-1.0, 'x');

	lightTwo = new sphere(200, 200, 0.5, 10.0);
	lightTwo->light->setDiffuse(1.0, 1.0, 1.0);
	lightTwo->light->emitLight(true);
	lightTwo->transform->scaleUniform(-0.9);
	lightTwo->transform->translate(1.0, 'y');
	lightTwo->transform->translate(1.0, 'x');

	tankBody = new body(90.0, 0.02);
	tankBody->light->setDiffuse(tankBody->getColour().x, tankBody->getColour().y, tankBody->getColour().z);
	tankBody->transform->rotate(180.0, 'y');

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
	glUniform3fv(specularID, 1, &light->getSpecular()[0]);
	glUniform3fv(emisiveID, 1, &light->getEmisive()[0]);
}

void display()
{
	track* tracks[2] = { trackOne, trackTwo };
	sphere* lights[2] = { lightOne, lightTwo };

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
		glm::vec3(0, 0, -7), // Camera is at (0,0,4), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
		);
	glm::mat4 model = glm::mat4(1.0f);
	model = globalTransform->getModel(); //global transformations

	glUniform1ui(numberOfLightsID, numberOfLights);
	glm::vec3 lightPosition_p;
	GLfloat lightsPositions[2 * 3];//number of lights
	for (int i = 0; i < numberOfLights; i++)
	{
		int step = i * 3;
		if (light_mode)
		{
			glm::mat4 lightModel = model * lights[i]->transform->getModel();
			setUniforms(view, lightModel, lights[i]->light);
			lights[i]->drawSphere();
			glm::vec4 lightPosition_h = view * lightModel * glm::vec4(lights[i]->transform->getCoords(), 1.0);
			lightPosition_p = glm::vec3(lightPosition_h.x, lightPosition_h.y, lightPosition_h.z);
			lightsPositions[step] = lightPosition_p.x;
			lightsPositions[step + 1] = lightPosition_p.y;
			lightsPositions[step + 2] = lightPosition_p.z;
			glUniform3fv(light_posID, i + 1, &lightsPositions[0]);
		}
		else
		{
			lightPosition = glm::vec3(fixedLight_x, fixedLight_y, fixedLight_z);
			lightsPositions[step] = lightPosition.x;
			lightsPositions[step + 1] = lightPosition.y;
			lightsPositions[step + 2] = lightPosition.z;
			glUniform3fv(light_posID, i + 1, &lightsPositions[0]);
		}
	}
	glUniform1ui(lightModeID, light_mode);

	glm::mat4 tankModel = model * tankBody->transform->getModel(); //tank body transforms

	setUniforms(view, tankModel, tankBody->light);
	tankBody->drawBody();
	
	glm::mat4 turretModel = tankBody->spinTurret(turret_spin);

	for (int i = 0; i < 3; i++) //3 parts to gun
	{
		glm::mat4 guntransform = turretModel * tankBody->getGunTransformations()[i]->getModel();
		guntransform = tankModel * guntransform;
		setUniforms(view, guntransform, tankBody->light);
		tankBody->getBaseCylider()->drawCyclinder();
	}

	for (int j = 0; j < 2; j++)
	{
		glm::mat4 trackModel = tankModel * tracks[j]->getTrack()->transform->getModel(); //track transforms

		for (int i = 0; i < tracks[j]->getTracks().size(); i++)
		{
			model = trackModel * tracks[j]->getTracks()[i]->getModel(); //individual tracks
			setUniforms(view, model, tracks[j]->getTrack()->light);
			tracks[j]->getTrack()->drawTrack(texID);
		}
		for (int i = 0; i < 4; i++)
		{
			model = trackModel * tracks[j]->getWheels()[i]->getModel(); //wheels
			setUniforms(view, model, tracks[j]->getBaseWheel()->light);
			tracks[j]->getBaseWheel()->drawCyclinder();
		}
	}

	trackOne->move(speed_r);
	trackTwo->move(speed_l);
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

	if (key == 'D') lightTwo->transform->translate(-0.05, 'x');
	if (key == 'A') lightTwo->transform->translate(0.05, 'x');
	if (key == 'R') lightTwo->transform->translate(-0.05, 'y');
	if (key == 'E') lightTwo->transform->translate(0.05, 'y');
	if (key == 'S') lightTwo->transform->translate(-0.05, 'z');
	if (key == 'W') lightTwo->transform->translate(0.05, 'z');

	if (key == GLFW_KEY_UP) globalTransform->rotate(1.0, 'x');
	if (key == GLFW_KEY_DOWN) globalTransform->rotate(-1.0, 'x');
	if (key == GLFW_KEY_LEFT) globalTransform->rotate(1.0, 'y');
	if (key == GLFW_KEY_RIGHT) globalTransform->rotate(-1.0, 'y');
	if (key == GLFW_KEY_KP_ADD) globalTransform->rotate(1.0, 'z');
	if (key == GLFW_KEY_KP_SUBTRACT) globalTransform->rotate(-1.0, 'z');
	if (key == 'H') turret_spin += 1;
	if (key == 'G') turret_spin -= 1;


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