#pragma once

#include "OpenGL.h"

class Camera;
class Renderer;


class Indirect
{
public:

	Indirect(int w, int h);
	~Indirect();

	void Begin(Camera &camera);
	void Finish();
	void Resize(Camera &camera, Renderer *render);
	bool Error() const{ return error; };

private:

	unsigned TextureSmall() const{ return texture[0]; };
	unsigned TextureNormals() const{ return texture[1]; };

	bool error;
	unsigned framebuffer, texture[2];

	int qwidth, qheight;
};
