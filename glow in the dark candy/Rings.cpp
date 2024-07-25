#include "Rings.h"
#include "Camera.h"
#include "Shaders.h"
#include "Resources.h"


Matrix4 Rings::m0, Rings::nm0, Rings::m1, Rings::nm1, Rings::m2, Rings::nm2, Rings::m3, Rings::nm3;


void Rings::Update(double time, Camera &cam)
{
	//m0 = Matrix4::rotation(time, Vector3(-0.1f, 0.34f, 0.9f));
	nm0 = *cam.MV();// * m0;
	m0 = *cam.MVP();// * m0;

	m1 = Matrix4::rotation(time, normalize(Vector3(0.871f, -0.344f, -0.2f)));
	nm1 = *cam.MV() * m1;
	m1 = *cam.MVP() * m1;

	m2 = Matrix4::rotation(time, normalize(Vector3(0.4641f, 0.34f, -0.49f)));
	nm2 = *cam.MV() * m2;
	m2 = *cam.MVP() * m2;

	m3 = Matrix4::rotation(time, normalize(Vector3(-0.1f, -.634f, 0.5f)));
	nm3 = *cam.MV() * m3;
	m3 = *cam.MVP() * m3;
}


void Rings::FirstPass()
{
	Shader::Object::Use();
	Shader::Object::s(8.0f/255.0f);

	Shader::Object::m((float*)&m3);
	Shader::Object::nm((float*)&nm3);
	model[M_R3].Draw();

	Shader::Object::m((float*)&m2);
	Shader::Object::nm((float*)&nm2);
	model[M_R2].Draw();

	Shader::Object::m((float*)&m1);
	Shader::Object::nm((float*)&nm1);
	model[M_R1].Draw();

	Shader::Object::m((float*)&m0);
	Shader::Object::nm((float*)&nm0);
	model[M_R0].Draw();
}


void Rings::SecondPass()
{
	Shader::Solid::Use();

	//Shader::Solid::c(...);	// const for all objects per frame

	Shader::Solid::dc(0.0625f, 0.0625f, 0.0625f);
	Shader::Solid::sc(1.0f, 1.0f, 1.0f);

	Shader::Solid::m((float*)&m0);
	model[M_R0].Draw();

	//Shader::Solid::dc(0.1f, 0.0f, 0.1f);
	Shader::Solid::m((float*)&m1);
	model[M_R1].Draw();

	//Shader::Solid::dc(0.1f, 0.1f, 0.0f);
	Shader::Solid::m((float*)&m2);
	model[M_R2].Draw();

	//Shader::Solid::dc(0.0f, 0.1f, 0.1f);
	Shader::Solid::m((float*)&m3);
	model[M_R3].Draw();
}