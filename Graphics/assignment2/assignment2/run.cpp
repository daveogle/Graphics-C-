/*Modified from example code on blackboard*/
/* Link to static libraries*/
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glload.lib")
#pragma comment(lib, "opengl32.lib")

/* Include the header to the GLFW wrapper class which
also includes the OpenGL extension initialisation*/
#include "wrapper_glfw.h"
#include "transformation.h"
#include "lighting.h"
#include "object_ldr.h"
#include "cuboid.h"
#include "terrain_object.h"
#include "points.h"
#include <iostream>
#include <stack>

GLboolean lightPass;
GLboolean shadows;
GLfloat zoom;
GLfloat angle_x;
GLfloat angle_x_inc;
GLfloat angle_y;
GLfloat angle_y_inc;
GLfloat angle_z;
GLfloat angle_z_inc;
GLfloat scale_x_value;
GLfloat scale_y_value;
GLfloat scale_z_value;
GLfloat x_tanslate;
GLfloat y_tanslate;
GLfloat z_tanslate;
GLuint shaders[3];

glm::mat4 shadow_matrix, scale_bias_matrix;

/* Include GLM core and matrix extensions*/
int drawmode = 0;									//points vs fill
GLfloat width, height;								/*window width & height*/
glm::mat4 projection;
glm::mat4 view[2];//camera / light
GLuint light_mode, numberOfLights, texture_mode, shadow_texture;
GLuint shadow_buffer, vao;					/*shaders & vertex array object*/
GLfloat aspect_ratio;								/* Aspect ratio of the window defined in the reshape callback*/
GLfloat fixedLight_x, fixedLight_y, fixedLight_z;	//fixed light position
glm::vec3 lightPosition, global_ambient;

//uniform IDs
GLuint projectionID, snowProjectionID, modelViewID, snowModelID, snowViewID, normal_matrixID, shininessID, 
	   ambientID, specularID, diffuseID, light_posID, emisiveID,
	   global_ambientID, lightModeID, numberOfLightsID, textureModeID,
	   shadowProjectionID, shadowModelViewID, shadow_matrixID, shadowModeID;

GLuint snowTexOne, snowTexTwo, coalTex, carrotTex, lampostTex, bulbTex;

std::stack<glm::mat4>model;

//snowman
transformation *snowballOne;
transformation *snowballTwo;
transformation *eyeOne;
transformation *eyeTwo;
transformation *carrot;
lighting *coal;
lighting *carrotLight;

//Terrain globals
terrain_object *heightfield;
int octaves;
GLfloat perlin_scale, perlin_frequency;
GLfloat land_size;
lighting *terrainLight;

//Snow
points *snowFlakes;

//objects
object_ldr sphere;
object_ldr cone;
object_ldr lampost;

//light bulbs
lighting *bulbLight;
transformation *bulbOne;
transformation *bulbTwo;
transformation *bulbThree;

//lamppost
lighting *lampostLight;
transformation *lampostModel;

void init(wrapper_glfw *glw)
{
	//load shaders
	try
	{
		std::cout << "loading shaders..." << std::endl;
		shaders[0] = glw->LoadShader("../Shaders/a2.vert", "../Shaders/a2.frag");
		shaders[1] = glw->LoadShader("../Shaders/shadow.vert", "../Shaders/shadow.frag");
		shaders[2] = glw->LoadShader("../Shaders/snowflake.vert", "../Shaders/snowflake.frag");
	}
	catch (std::exception &e)
	{
		std::cout << "Caught exception: " << e.what() << std::endl;
		std::cin.ignore();
		exit(0);
	}

	//load files
	sphere.load_obj("../Objects/sphere.obj");
	cone.load_obj("../Objects/cone.obj");
	lampost.load_obj("../Objects/lampost.obj");

	//global transformation values
	zoom = 10;
	angle_x = 0.0;
	angle_x_inc = 0.0;
	angle_y = 0.0;
	angle_y_inc = 0.0;
	angle_z = 0.0;
	angle_z_inc = 0.0;
	scale_x_value = 1;
	scale_y_value = 1;
	scale_z_value = 1;
	x_tanslate = 0.0;
	y_tanslate = -1.0;
	z_tanslate = 0.0;

	//enable blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	/* Create the heightfield object */
	octaves = 8;
	perlin_scale = 2.f;
	perlin_frequency = 1.f;
	land_size = 50.f;
	heightfield = new terrain_object(octaves, perlin_frequency, perlin_scale);
	heightfield->createTerrain(200, 200, land_size, land_size);
	//set texture
	snowTexOne = heightfield->setTexture("../Textures/packed_snow.png");
	int loc = glGetUniformLocation(shaders[0], "tex1");
	if (loc >= 0) glUniform1i(loc, 0); 
	heightfield->createObject();
	//terrain lighting
	terrainLight = new lighting(2.0f, 0.2f);
	terrainLight->setDiffuse(1.0, 1.0, 1.0);
	terrainLight->setSpecular(0.4, 0.4, 0.0);

	//create new snowflake object
	snowFlakes = new points(2000, -1.5, 0.01);
	snowFlakes->setTexture("../Textures/snowflake2.png");
	int snowLec = glGetUniformLocation(shaders[2], "snowTex");
	if (snowLec >= 0) glUniform1i(snowLec, 0);
	snowFlakes->create();
	
	//setup sphere 
	sphere.smoothNormals();
	snowTexTwo = sphere.setTexture("../Textures/icy_snow.png", 0);
	coalTex = sphere.setTexture("../Textures/coal.png", 0);
	bulbTex = sphere.setTexture("../Textures/frosted_glass.png", 0);
	sphere.createObject();

	//setup cone 
	cone.smoothNormals();
	carrotTex = cone.setTexture("../Textures/carrot.png", 0);
	cone.createObject();

	//setup lampost
	lampost.smoothNormals();
	lampostTex = lampost.setTexture("../Textures/metal.png", 0);
	lampost.createObject();

	/*Snowman transformations & lighting*/
	snowballOne = new transformation();
	snowballOne->scaleUniform(-0.6);
	snowballOne->translate(0.4, 'y');

	snowballTwo = new transformation();
	snowballTwo->scaleUniform(-0.8);
	snowballTwo->translate(0.9, 'y');

	//eyes
	coal = new lighting(20.0, 0.2);
	coal->setDiffuse(0.0, 0.0, 0.0);
	coal->setSpecular(0.01, 0.01, 0.01);
	eyeOne = new transformation();
	eyeOne->scaleUniform(-0.98);
	eyeOne->translate(-0.17, 'z');
	eyeOne->translate(1.0, 'y');
	eyeOne->translate(0.05, 'x');
	eyeTwo = new transformation();
	eyeTwo->scaleUniform(-0.98);
	eyeTwo->translate(-0.17, 'z');
	eyeTwo->translate(1.0, 'y');
	eyeTwo->translate(-0.05, 'x');
	//nose
	carrotLight = new lighting(20.0, 0.2);
	carrotLight->setDiffuse(1.0, 0.5490196078431373, 0.0);
	carrot = new transformation();
	carrot->scaleUniform(-0.96);
	carrot->translate(0.95, 'y');
	carrot->translate(-0.25, 'z');
	carrot->scale(0.06, 'y');
	carrot->rotate(90.0, 'x');

	//lamppost
	lampostModel = new transformation();
	lampostModel->translate(1.0, 'x');
	lampostModel->translate(1.5, 'y');
	lampostLight = new lighting(70.0, 1.0);
	lampostLight->setDiffuse(0.3, 0.8, 0.0);

	//create bulbs
	bulbOne = new transformation();
	bulbTwo = new transformation();
	bulbThree = new transformation();
	bulbLight = new lighting(90.0f, 0.2f);
	bulbLight->setDiffuse(0.4, 0.4, 0.0);
	bulbLight->emitLight(true);
	bulbLight->setEmisive(0.4, 0.4, 0.0);
	bulbOne->scaleUniform(-0.865);
	bulbOne->translate(0.68, 'x');
	bulbOne->translate(2.66, 'y');
	bulbOne->translate(-0.28, 'z');
	bulbTwo->scaleUniform(-0.865);
	bulbTwo->translate(1.33, 'x');
	bulbTwo->translate(2.66, 'y');
	bulbTwo->translate(-0.28, 'z');
	bulbThree->scaleUniform(-0.865);
	bulbThree->translate(1.00, 'x');
	bulbThree->translate(2.66, 'y');
	bulbThree->translate(0.28, 'z');

	//set aspect ratio
	aspect_ratio = width / height;

	//set fixed light values
	fixedLight_x = 0.0f;
	fixedLight_y = 100.0;
	fixedLight_z = 100.0;
	lightPosition = glm::vec3(fixedLight_x, fixedLight_y, fixedLight_z);
	global_ambient = glm::vec3(0.02);
	numberOfLights = 1;
	light_mode = 1; //light mode = in scene
	texture_mode = 1; //texture enabled
	
	//shadow flags
	lightPass = false;
	shadows = false;

	//print graphics card details
	GLint maxTextures;
	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxTextures);
	fprintf(stderr, "VENDOR: %s\n", (char *)glGetString(GL_VENDOR));
	fprintf(stderr, "VERSION: %s\n", (char *)glGetString(GL_VERSION));
	fprintf(stderr, "RENDERER: %s\n", (char *)glGetString(GL_RENDERER));
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	std::cout << "WINDOW SIZE = " << width << ':' << height << '\n';
	std::cout << "Aspect Ratio  = " << aspect_ratio << '\n';
	std::cout << "Max number of texture units  = " << maxTextures << '\n';
	std::cout << '\n';

	//print key press instructions
	std::cout << "Key S = Shadows" << '\n';
	std::cout << "Key up = move view up" << '\n';
	std::cout << "Key down = move view down" << '\n';
	std::cout << "Key left = move view left" << '\n';
	std::cout << "Key right = move view right" << '\n';
	std::cout << "Key - = move view out" << '\n';
	std::cout << "Key + = move view in" << '\n';
	std::cout << "Key M = Night Time / Day Time" << '\n';
	std::cout << "Key T = change texture mode" << '\n';
	std::cout << "Key Z = change draw mode" << '\n';
	std::cout << std::endl;

	// Generate index (name) for one vertex array object
	glGenVertexArrays(1, &vao);

	// Create the vertex array object and make it current
	glBindVertexArray(vao);

	//generate a framebuffer object with only depth attachment //from openGL superbible sixth edition pg.536
	glGenFramebuffers(1, &shadow_buffer);
	glBindFramebuffer(GL_FRAMEBUFFER, shadow_buffer);

	glGenTextures(1, &shadow_texture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, shadow_texture);
	glTexImage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT32F, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	glBindFramebuffer(GL_FRAMEBUFFER, shadow_buffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadow_texture, 1);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	int loc2 = glGetUniformLocation(shaders[0], "shadow_tex");
	if (loc2 >= 0) glUniform1i(loc2, 1);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glActiveTexture(GL_TEXTURE0);

	shadow_matrix = glm::mat4(1.0f);
	//set scale-bias matrix
	scale_bias_matrix = glm::mat4(glm::vec4(0.5f, 0.0f, 0.0f, 0.0f),
		glm::vec4(0.0f, 0.5f, 0.0f, 0.0f),
		glm::vec4(0.0f, 0.0f, 0.5f, 0.0f),
		glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));


	//Uniform locations Main
	projectionID = glGetUniformLocation(shaders[0], "projection");
	modelViewID = glGetUniformLocation(shaders[0], "model_view");
	shininessID = glGetUniformLocation(shaders[0], "shininess");
	ambientID = glGetUniformLocation(shaders[0], "ambient");
	specularID = glGetUniformLocation(shaders[0], "specular_colour");
	diffuseID = glGetUniformLocation(shaders[0], "diffuse_colour");
	light_posID = glGetUniformLocation(shaders[0], "light_pos");
	normal_matrixID = glGetUniformLocation(shaders[0], "normal_matrix");
	emisiveID = glGetUniformLocation(shaders[0], "emissive");
	global_ambientID = glGetUniformLocation(shaders[0], "global_ambient");
	lightModeID = glGetUniformLocation(shaders[0], "light_mode");
	textureModeID = glGetUniformLocation(shaders[0], "texture_mode");
	numberOfLightsID = glGetUniformLocation(shaders[0], "numberOfLights");
	shadow_matrixID = glGetUniformLocation(shaders[0], "shadow");
	shadowModeID = glGetUniformLocation(shaders[0], "shadow_mode");

	//Uniform locations Snow
	snowModelID = glGetUniformLocation(shaders[2], "model");
	snowViewID = glGetUniformLocation(shaders[2], "view");
	snowProjectionID = glGetUniformLocation(shaders[2], "projection");

	//Uniform locations Shadow
	shadowProjectionID = glGetUniformLocation(shaders[1], "projection");
	shadowModelViewID = glGetUniformLocation(shaders[1], "model_view");
}

/*
* Function to set uniform values
*/
void setUniforms(glm::mat4 model, lighting* light)
{
	glm::mat4 model_view = view[lightPass] * model;
	glUniformMatrix4fv(modelViewID, 1, GL_FALSE, &model_view[0][0]);
	if (!lightPass)
	{
		//setup shadow matrix
		shadow_matrix = scale_bias_matrix * projection * model * view[1];
		glUniformMatrix4fv(shadow_matrixID, 1, GL_FALSE, &shadow_matrix[0][0]);
		glm::mat3 normal_matrix = glm::transpose(glm::inverse(glm::mat3(model_view)));
		glUniformMatrix3fv(normal_matrixID, 1, GL_FALSE, &normal_matrix[0][0]);
		glUniform1f(ambientID, light->getAmbient());
		glUniform1f(shininessID, light->getShininess());
		glUniform3fv(diffuseID, 1, &light->getDiffuse()[0]);
		glUniform3fv(specularID, 1, &light->getSpecular()[0]);
		glUniform3fv(emisiveID, 1, &light->getEmisive()[0]);
	}
}

//Function to draw the scene
void drawScene()
{
	transformation* lights[3] = { bulbOne, bulbTwo, bulbThree };
	glm::vec3 lightPosition_p;
	GLfloat lightsPositions[3 * 3];//number of lights * xyz

	for (int i = 0; i < numberOfLights; i++)
	{
		int step = i * 3;
		if (light_mode)
		{
			numberOfLights = 3;
			if (!lightPass)	glUniform1ui(numberOfLightsID, numberOfLights);
			glm::mat4 lightModel = model.top() * lights[i]->getModel();
			setUniforms(lightModel, bulbLight);
			sphere.drawObject(bulbTex);
			
			if (!lightPass)
			{
				glm::vec4 lightPosition_h = view[lightPass] * lightModel * glm::vec4(lights[i]->getCoords(), 1.0);
				lightPosition_p = glm::vec3(lightPosition_h.x, lightPosition_h.y, lightPosition_h.z);
				lightsPositions[step] = lightPosition_p.x;
				lightsPositions[step + 1] = lightPosition_p.y;
				lightsPositions[step + 2] = lightPosition_p.z;
				glUniform3fv(light_posID, i + 1, &lightsPositions[0]);
			}
		}
		else
		{
			numberOfLights = 1;
			if (!lightPass)
			{
				glUniform1ui(numberOfLightsID, numberOfLights);
				lightsPositions[step] = lightPosition.x;
				lightsPositions[step + 1] = lightPosition.y;
				lightsPositions[step + 2] = lightPosition.z;
				glUniform3fv(light_posID, i + 1, &lightsPositions[0]);
			}
		}
	}

	setUniforms(model.top() * eyeOne->getModel(), coal);
	sphere.drawObject(coalTex);

	setUniforms(model.top() * eyeTwo->getModel(), coal);
	sphere.drawObject(coalTex);

	setUniforms(model.top() * carrot->getModel(), carrotLight);
	cone.drawObject(carrotTex);

	setUniforms(model.top() * snowballOne->getModel(), terrainLight);
	sphere.drawObject(snowTexTwo);

	setUniforms(model.top() * snowballTwo->getModel(), terrainLight);
	sphere.drawObject(snowTexTwo);

	setUniforms(model.top() * lampostModel->getModel(), lampostLight);
	lampost.drawObject(lampostTex);

	setUniforms(model.top(), terrainLight);
	heightfield->drawObject(drawmode);

	model.pop();
}

//display loop
void display()
{
	glViewport(0, 0, width, height);
	static const GLfloat ones[] = { 1.0f };
	static const GLfloat zero[] = { 0.0f };
	static const GLfloat gray[] = { 0.1f, 0.1f, 0.1f, 0.0f };
	lightPass = !lightPass;
	lightPass = lightPass * shadows;
	//array holding light bulbs

	/* Define the background colour */
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	/* Clear the colour and frame buffers */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* Enable depth test  */
	glEnable(GL_DEPTH_TEST);

	//both shaders
	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	projection = glm::perspective(30.0f, aspect_ratio, 0.1f, 100.0f);
	glUniformMatrix4fv(projectionID, 1, GL_FALSE, &projection[0][0]);

	model.push(glm::mat4(1.0));
	model.top() = glm::translate(model.top(), glm::vec3(x_tanslate, y_tanslate, z_tanslate));
	model.top() = glm::scale(model.top(), glm::vec3(scale_x_value, scale_y_value, scale_z_value));
	model.top() = glm::rotate(model.top(), angle_x, glm::vec3(1, 0, 0));
	model.top() = glm::rotate(model.top(), angle_y, glm::vec3(0, 1, 0));
	model.top() = glm::rotate(model.top(), angle_z, glm::vec3(0, 0, 1));
	/* Make the right shader program current */

	glUseProgram(shaders[lightPass]);
	if (lightPass)
	{
		projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 100.0f);
		glm::vec3 lightSource = lightPosition;
		if (light_mode == 1)
		{
			lightSource = glm::vec3(1.0, 2.66, 0.0f);
		}
		//if lightmode = 1
		view[lightPass] = glm::lookAt(
			glm::vec3(lightSource.x, lightSource.y, lightSource.z), // Camera is at light source, in World Space
			glm::vec3(0, 0, 0), // and looks at the origin
			glm::vec3(0, 1, 0)
			);

		glBindFramebuffer(GL_FRAMEBUFFER, shadow_buffer);
		glClear(GL_DEPTH_BUFFER_BIT);
		glUseProgram(shaders[lightPass]);
	}
	else
	{
		//set camera view
		view[lightPass] = glm::lookAt(
			glm::vec3(0, 0, zoom), // Camera is at (0,0,4), in World Space
			glm::vec3(0, 0, 0), // and looks at the origin
			glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
			);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(shaders[lightPass]);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, shadow_texture);

		//render the snow	
		glUseProgram(shaders[2]);
		/* Enable Blending for the analytic point sprite */
		glEnable(GL_BLEND);
		glm::mat4 flakeModel = glm::translate(model.top(), glm::vec3(0.0, 2.0, 0.0));
		glUniformMatrix4fv(snowModelID, 1, GL_FALSE, &flakeModel[0][0]);
		glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
		glUniformMatrix4fv(snowViewID, 1, GL_FALSE, &view[lightPass][0][0]);
		glUniformMatrix4fv(snowProjectionID, 1, GL_FALSE, &projection[0][0]);

		snowFlakes->draw();
		snowFlakes->animate();

		//main shader
		glUseProgram(shaders[lightPass]);
		glUniform3fv(global_ambientID, 1, &global_ambient[0]);
		glUniform1ui(textureModeID, 0);
		glUniformMatrix4fv(projectionID, 1, GL_FALSE, &projection[0][0]);
		glUniform1ui(lightModeID, light_mode);
		glUniform1ui(shadowModeID, shadows);
		glUniform1ui(textureModeID, texture_mode); //set Texture mode
	}

	drawScene();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
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

	if (key == GLFW_KEY_UP)
	{
		angle_x -= 1.5;
	}
	if (key == GLFW_KEY_DOWN)
	{
		angle_x += 1.5;
	}
	if (key == GLFW_KEY_LEFT)
	{
		angle_y -= 1.5;
	}
	if (key == GLFW_KEY_RIGHT)
	{
		angle_y += 1.5;
	}

	if (action == GLFW_PRESS)
	{
		if (key == GLFW_KEY_KP_ADD) zoom -= 1.0;
		if (key == GLFW_KEY_KP_SUBTRACT) zoom += 1.0;
		//std::cout << "bulb 1 xyz = " << bulbOne->getCoords().x << "," << bulbOne->getCoords().y << "," << bulbOne->getCoords().z << std::endl;
		//std::cout << "bulb 2 xyz = " << bulbTwo->getCoords().x << "," << bulbTwo->getCoords().y << "," << bulbTwo->getCoords().z << std::endl;
		if (key == 'M')
		{
			light_mode = !light_mode;
			std::cout << "lightmode changed: " << std::endl;
		}
		if (key == 'T')
		{
			texture_mode = !texture_mode;
			std::cout << "texture mode changed: " << texture_mode << std::endl;
		}
		if (key == 'Z')
			drawmode = !drawmode;

		if (key == 'S')
		{
			shadows = !shadows;
		};
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
	char* message = "Dave Ogle - Assignment 2 - Snowy Scene";
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