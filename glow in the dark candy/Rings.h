#pragma once

#include "OpenGL.h"


class Camera;

class Rings
{
public:

	static void Update(double time, Camera &cam);
	static void FirstPass();
	static void SecondPass();

private:

	static Matrix4 m0, nm0, m1, nm1, m2, nm2, m3, nm3;
};