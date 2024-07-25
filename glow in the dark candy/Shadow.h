#include "OpenGL.h"


class Shadow
{
public:

	Shadow(const int &w, const int &h);
	~Shadow();

private:

	unsigned texture, framebuffer, width, height;
};
