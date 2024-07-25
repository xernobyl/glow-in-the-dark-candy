#pragma once

#include "OpenGL.h"


class PostProcessing
{
public:

	PostProcessing(int width, int height);
	~PostProcessing();

	void Draw();

	bool Error() const{ return error; };

private:

	bool error;
	
	unsigned framebuffer;
	unsigned width, height;
};
