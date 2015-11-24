#include "texture_loader.h"


texture_loader::texture_loader()
{
	textureNo = 0;
}


texture_loader::~texture_loader()
{
}


unsigned int texture_loader::loadTexture(const char* textureFile, int texNumber)
{
	unsigned int texID = 0;
	try
	{
		glActiveTexture(GL_TEXTURE0 + texNumber);

		/* load an image file directly as a new OpenGL texture */
		texID = SOIL_load_OGL_texture(textureFile, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
			SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);

		/* check for an error during the load process */
		if (texID == 0)
		{
			printf("TexID SOIL loading error: '%s'\n", SOIL_last_result());
		}
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	}
	catch (std::exception &e)
	{
		printf("\nImage file loading failed.");
		return 0;
	}
	return texID;
}