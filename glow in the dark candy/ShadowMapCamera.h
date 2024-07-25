#pragma once

#include "OpenGL.h"
#include "SpotLight.h"


class ShadowMapCamera
{
public:

	void Update(SpotLight *spot);
	Matrix4 *MV(){ return &modelviewmatrix; };
	Matrix4 *MVP(){ return &modelviewprojectionmatrix; };

	float Near() const{ return nearp; };
	float Far() const{ return farp; };

	static Matrix4 *BiasMatrix(){ return (Matrix4*)biasmatrix; };

private:

	float fov, nearp, farp;

	Matrix4 projectionmatrix;
	Matrix4 modelviewmatrix;
	Matrix4 modelviewprojectionmatrix;

	static const float biasmatrix[4*4];
};
