#pragma once

class Camera;


class ReflectionCubemap
{
public:

	ReflectionCubemap(unsigned size);
	~ReflectionCubemap();

	void Begin(Camera &cam);
	void Finish();

	bool Error() const{ return error; };

private:

	bool error;
	unsigned texture[7], framebuffer[6];
};