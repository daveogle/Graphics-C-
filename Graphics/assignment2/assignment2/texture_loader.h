#pragma once
#pragma comment(lib, "soil.lib")
#include "SOIL.h"
#include "wrapper_glfw.h"
#include <vector>
#include <glm/glm.hpp>

class texture_loader
{
	unsigned int textureNo;

public:
	texture_loader();
	~texture_loader();
	static unsigned int loadTexture(const char* textureFile, int texNumber);
};



