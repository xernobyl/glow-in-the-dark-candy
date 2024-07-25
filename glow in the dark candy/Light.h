#pragma once

#include "OpenGL.h"
#include <list>

class Camera;


class Light
{
public:

	virtual void Draw(Camera &cam) = 0;

	const float Intensity();
	void Intensity(const float i);
	float *Color(){ return value; };
	void Color(const float *p);
	void Color(const float &r, const float &g, const float &b);

protected:

	enum
	{
		CAST_SHADOW = 1,
		REFLECTIVE_SM = 2,
		DO_AMBIENT = 4,		// average all lights for ambient?
		TEXTURE = 8,
	};

	unsigned char flags;

	float value[4];					// color / intensity

	static std::list<Light> lightlist;
};
