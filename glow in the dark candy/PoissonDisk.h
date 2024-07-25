#pragma once

#include "OpenGL.h"


class PoissonDisk
{
public:

	PoissonDisk();
	~PoissonDisk();

	void Draw();
	unsigned NPoints() const{ return npoints; };

private:

	static const float disk[256];
	unsigned va, vb;
	unsigned npoints;
};
