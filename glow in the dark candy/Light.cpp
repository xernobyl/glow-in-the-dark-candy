#include "Light.h"


const float Light::Intensity()
{
	return value[3];
}


void Light::Intensity(const float i)
{
	float t = value[0] > value[1] ? (value[2] > value[0] ? value[2] : value[0]) : (value[2] > value[1] ? value[2] : value[1]);

	value[0] *= i/t;
	value[1] *= i/t;
	value[2] *= i/t;
	value[3] = i;
}


void Light::Color(const float *p)
{
	value[0] = p[0];
	value[1] = p[1];
	value[2] = p[2];
	value[3] = value[0] > value[1] ? (value[2] > value[0] ? value[2] : value[0]) : (value[2] > value[1] ? value[2] : value[1]);
}


void Light::Color(const float &r, const float &g, const float &b)
{
	value[0] = r;
	value[1] = g;
	value[2] = b;
	value[3] = value[0] > value[1] ? (value[2] > value[0] ? value[2] : value[0]) : (value[2] > value[1] ? value[2] : value[1]);
}