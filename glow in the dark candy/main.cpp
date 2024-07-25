	#include "Window.h"
#include "Shaders.h"
#include "Quad.h"
#include "Model.h"
#include "Random.h"
#include "Renderer.h"
#include "Camera.h"
#include "Dialog.h"
#include "stb_image.h"
#include "Resources.h"
#include "PointLight.h"
#include "DirectionalLight.h"
#include "Rings.h"
#include "ReflectionCubemap.h"
#include "GPUParticles.h"


Renderer *renderer;
Camera camera;

PointLight light0;
DirectionalLight light1, light2;

ReflectionCubemap *cubemap;

GPUParticles *particles;


void Loop()
{
	if(Window::KeyDown(VK_ESCAPE) || Window::KeyDown('q') || Window::ButtonDown(B_START))
		Window::Kill();

	if(Window::ButtonPressed(B_L2))
		camera.Focus(0.0, 0.0);
	else
		camera.Focus(Window::Stick1Y() * 20.0 + 28.2617497768, Window::Stick1X() * 0.1 + 0.1);
	
	//camera.Focus(10.0, 0.025);
	if(!Window::ButtonPressed(B_3))
		camera.LookAt(Point3((float)cos(Window::Time() * 0.25f) * 25.0f, 7.5f, (float)sin(Window::Time() * 0.25f) * 25.0f), Point3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f));

	//

	//particles->Update();

	light0.Color(1000000.0f, 500000.0f, 62500.0f);
	light0.Position(50.0f, 0.0f, 0.0f);

	light1.Color(0.75f, 0.75f, 0.75f);
	light1.Direction(0.0f, -1.0f, 0.0f);

	light2.Color(light1.Color()[0] * 0.318309886f, light1.Color()[1] * 0.318309886f, light1.Color()[2] * 0.318309886f);
	light2.Direction(0.0f, 1.0f, 0.0f);

	if(!Window::ButtonPressed(B_L1) || !Window::KeyPressed(VK_DOWN))
		Rings::Update(Window::Time(), camera);

	renderer->BeginFirstPass();

	renderer->Material(Renderer::M_LIGHTEN | Renderer::M_AMBIENT);
	Rings::FirstPass();

	renderer->FinishFirstPass();

	renderer->AmbientPass(camera);

	renderer->BeginLightPass(true);

	glStencilFunc(GL_EQUAL, (renderer->frame & 1) | Renderer::M_AMBIENT, Renderer::M_FRAMEPARITY | Renderer::M_AMBIENT);

	float v[3];
	v[0] = 0.0f;
	v[1] = 1.0f;
	v[2] = 0.0f;

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_RECTANGLE, renderer->TextureAmbient());

	Shader::AS2_HemiLight::Use();
	Shader::AS2_HemiLight::Z(0);
	Shader::AS2_HemiLight::N(1);
	Shader::AS2_HemiLight::zc(camera.ZC());
	//Shader::AS2_HemiLight::col0(1.0f * camera.Illuminance(), 1.0f * camera.Illuminance(), 1.0f * camera.Illuminance());
	//Shader::AS2_HemiLight::col1(0.3183099f * camera.Illuminance(), 0.3183099f * camera.Illuminance(), 0.3183099f * camera.Illuminance());
	Shader::AS2_HemiLight::amb(100.0f * camera.Illuminance(), 100.0f * camera.Illuminance(), 100.0f * camera.Illuminance());
	Shader::AS2_HemiLight::dir0(v);
	v[0] *= -1.0f;
	v[1] *= -1.0f;
	v[2] *= -1.0f;
	Shader::AS2_HemiLight::dir1(v);
	Shader::AS2_HemiLight::hfov(camera.HFOV());
	Shader::AS2_HemiLight::A(2);

	Quad::Draw();

	//

	//glStencilFunc(GL_EQUAL, (renderer->frame & 1) | Renderer::M_LIGHTEN, Renderer::M_FRAMEPARITY | Renderer::M_LIGHTEN);
	//light0.Draw(camera);

	renderer->FinishLightPass();

	//

	renderer->BeginSecondPass();

	Shader::Solid::Use();
	Shader::Solid::c(camera.COC());

	Rings::SecondPass();

	Shader::Background::Use();
	Shader::Background::b(light2.Color()[0] * camera.Illuminance(), light2.Color()[1] * camera.Illuminance(), light2.Color()[2] * camera.Illuminance(), (float)camera.COC()[1]);
	//Shader::Background::b(0.0f, 0.0f, 0.0f, (float)camera.COC()[1]);
	Quad::Draw();

	Matrix4 m = Matrix4::scale(Vector3(0.125f, 0.125f, 0.125f));
	m = Matrix4::translation(Vector3(50.0f, 0.0f, 0.0f)) * m;
	m = *camera.MVP() * m;

	Shader::Emissive::Use();
	Shader::Emissive::c(camera.COC());
	Shader::Emissive::ec(light0.Color()[0] * camera.Illuminance(), light0.Color()[1] * camera.Illuminance(), light0.Color()[2] * camera.Illuminance());
	Shader::Emissive::m((float*)&m);
	model[M_SPHERE].Draw();

	particles->Render();

	renderer->FinishSecondPass();

	renderer->RenderPost(camera);
	renderer->RenderCombine();
	
	if(Dialog::Quality() >= 4)
		renderer->RenderFXAA();
	else
		renderer->RenderSGRB();
}


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{	
#ifndef _DEBUG
		int w = 1600, h = 900;

		if(!Dialog::Start(w, h))
			return 0;
#endif

	if(Window::Init())
	{
		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

		RandSeed(1);

		if(!Shader::InitAll())
		{
			MessageBoxA(0, "Shaders kaput.", 0, 0);
			return 0;
		}
		else
		{
			Shader::Solid::Use();
			Shader::Solid::D(2);
			Shader::Solid::S(3);

			Shader::Albedo::Use();
			Shader::Albedo::D(2);
			Shader::Albedo::S(3);

			Shader::VO::Use();
			Shader::VO::Z(0);

			Shader::VOScale::Use();
			Shader::VOScale::Z(0);

			Shader::Directional::Use();
			Shader::Directional::Z(0);
			Shader::Directional::N(1);

			Shader::Point::Use();
			Shader::Point::Z(0);
			Shader::Point::N(1);
		}

		//cubemap = new ReflectionCubemap(512);
		//if(cubemap->Error())
		//{
		//	MessageBoxA(0, "Could not init CM buffers.", 0, 0);
		//	return 0;
		//}

#ifdef _DEBUG
		Window::FullScreen(false);
		renderer = new Renderer(Window::Width(), Window::Height());
#else
		Window::FullScreen(true);
		renderer = new Renderer(Dialog::Width(), Dialog::Height());
#endif
		
		if(!InitModels())
		{
			MessageBoxA(0, "Could not load models.", 0, 0);
			return 0;
		}
		
		if(renderer->Error())
		{
			MessageBoxA(0, "Could not init buffers.", 0, 0);
			return 0;
		}

		Quad::Init();

		Window::InitInput();

		camera.Illuminate(true);
		camera.FarP(100.0);
		camera.NearP(1.0);
		camera.AspectRatio(Window::AspectRatio(), renderer->Diagonal());
		camera.FocalLength(50.0);
		camera.UpdateProjection();

		particles = new GPUParticles(5000);

		InitTextures();

		Window::Loop(Loop);
	}

	return 0;
}
