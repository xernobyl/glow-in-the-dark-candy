#include "ShadowMapCamera.h"

__declspec(align(16)) const float ShadowMapCamera::biasmatrix[4*4] =
{
	0.5f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.5f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.5f, 0.0f,
	0.5f, 0.5f, 0.5f, 1.0f
};


void ShadowMapCamera::Update(SpotLight *spot)
{
	nearp = 1.0f;
	farp = 50.0f;

	projectionmatrix = Matrix4::perspective(spot->spot, 1.0f, nearp, farp);
	modelviewmatrix = Matrix4::lookAt(*spot->Position(), *spot->Position() + *spot->Direction(), Vector3(1.0, 1.0, 1.0));
	modelviewprojectionmatrix = projectionmatrix * modelviewmatrix;
}