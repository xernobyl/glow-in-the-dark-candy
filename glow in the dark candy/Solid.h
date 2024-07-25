#pragma once

#include "Object.h"
#include "Model.h"
#include "Shaders.h"
#include "Camera.h"
#include "ShadowMapCamera.h"


class Solid: public Object
{
public:

	Solid(Model* m):
	model(m)
	{
		matrix = matrix.identity();
	}
	
	void Draw(Camera *camera)
	{
		Matrix4 m = *camera->MVP() * matrix;
		Matrix4 nm = *camera->MV() * matrix;

		Shader::Object::Use();
		Shader::Object::m((float*)&m);
		Shader::Object::nm((float*)&nm);

		model->Draw();
	}

	void DrawRSM(ShadowMapCamera *smc)
	{
	}

	void DrawModel()
	{
		model->Draw();
	}


	void Albedo(const float &r, const float &g, const float &b)
	{
		albedo = Vector3(r, g, b);
	}

	float *Albedo(){ return (float*)&albedo; }

	void DiffuseSpecularMix(const float &dsm)
	{
		diffusespecularmix = dsm;
	}

	void Shininess(const float &s)
	{
		shininess = s;
	}

private:

	Model *model;

	float diffusespecularmix;
	float shininess;
	Vector3 albedo;
};
