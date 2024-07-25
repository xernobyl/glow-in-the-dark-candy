#pragma once

#include "OpenGL.h"
#include <vector>


class Model
{
public:

	~Model();

	bool Load(const char* filepath);
	void Draw();

private:

	struct Triangle
	{
		float normal[3], bitangent[3], tangent[3];
		unsigned short vertex[3];
		unsigned short uv[6];
	};

	enum
	{
		F_WEIGHT0	= 0x01,
		F_WEIGHT1	= 0x02,
		F_WEIGHTS	= 0x03,
		F_FLOAT		= 0x04,
		F_STRIPS	= 0x08,
		F_HASUVS	= 0x10,
		F_5			= 0x20,
		F_6			= 0x40,
		F_7			= 0x80,
	};

	void BestFitNormal(const Vector3 &v, unsigned char *c);

	unsigned ntriangles;
	unsigned vbo, vao;
	unsigned *stripidx;
	unsigned *striplen;

	std::vector<std::vector<struct Triangle>> strip;
};
