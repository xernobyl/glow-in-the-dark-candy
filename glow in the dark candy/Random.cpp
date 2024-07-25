#include "Random.h"
#include <cmath>


static unsigned rnd;

// returns random number between -1.0 to 1.0
float Rand()
{
	union
	{
		unsigned u;
		float f;
	} a;
	
	a.u = (rnd&0x007fffff) | 0x40000000;
	rnd *= 16807;
	return a.f - 3.0f;
}


float URand()
{
	union
	{
		unsigned u;
		float f;
	} a;
	
	a.u = (rnd&0x007fffff) | 0x40000000;
	rnd *= 16807;
	return a.f * 0.5f - 1.0f;
}


float Rand(float min, float max)
{
	return URand()*(max-min)+min;
}


float Rand(float max)
{
	return URand()*max;
}


void RandSeed(unsigned int seed)
{
	rnd = seed;
}


void RandDisk(float *in, const unsigned npoints, const float rad, const unsigned offset)
{
	for(unsigned i = 0; i < npoints; ++i)
	{
		float r = sqrt(Rand(rad));
		float theta = Rand(3.14159265359f);

		in[i*offset + 0] = r * cos(theta);
		in[i*offset + 1] = r * sin(theta);
	}
}
