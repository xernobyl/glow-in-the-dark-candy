#include "TextureManager.h"
#include "OpenGL.h"


unsigned *list;
int nunits;


void TextureManager::Init()
{
	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &nunits);

	list = new unsigned[nunits];
}


void TextureManager::Kill()
{
	delete [] list;
}
