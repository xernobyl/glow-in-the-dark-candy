#include "Ambient2.h"
#include "Camera.h"
#include "Shaders.h"
#include "Quad.h"
#include "Renderer.h"
#include "Random.h"


Ambient2::Ambient2(Renderer *render):
error(false)
{
	enum
	{
		NOISE_RES = 32
	};
	
	unsigned char *buffer = new unsigned char[NOISE_RES * NOISE_RES * 2];

	for(unsigned i = 0; i < NOISE_RES * NOISE_RES * 2; ++i)
		buffer[i] = (unsigned char)Rand(256.0f);

	glGenTextures(1, &texturenoise);
	glBindTexture(GL_TEXTURE_2D, texturenoise);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RG8, NOISE_RES, NOISE_RES, 0, GL_RG, GL_UNSIGNED_BYTE, buffer);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	delete [] buffer;

	texturenormal = render->TextureNormalShininess();
	texturedepth = render->TextureDepth();

	hwidth = render->Width()/2;
	hheight = render->Height()/2;

	ndisksamples = 8;
	radius = 1.0f;
	thickness = 1.0f;

	disksamples = new float[3*ndisksamples];
	RandDisk(disksamples, ndisksamples, 1.0f, 3);

	for(unsigned i=0; i<ndisksamples; ++i)
		disksamples[i*3 + 2] = (float)sqrt(1.0 - disksamples[i*3 + 0] * disksamples[i*3 + 0] - disksamples[i*3 + 1] * disksamples[i*3 + 1]);

	glGenTextures(2, texture);
	glGenFramebuffers(2, framebuffer);

	glBindTexture(GL_TEXTURE_RECTANGLE, texture[0]);
	glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGBA8, hwidth, hheight, 0, GL_RGBA, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_RECTANGLE, texture[1]);
	glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGBA8, 2 * hwidth, 2 * hheight, 0, GL_RGBA, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer[0]);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture[0], 0);
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		error = true;
		return;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer[1]);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture[1], 0);
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		error = true;
		return;
	}
}


Ambient2::~Ambient2()
{
	glDeleteTextures(2, texture);
	glDeleteFramebuffers(2, framebuffer);

	delete [] disksamples;
}


void Ambient2::Radius(float r)
{
	radius = r;
}


void Ambient2::Thickness(float t)
{
	thickness = t;
}


void Ambient2::Render(Camera &cam)
{
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer[0]);
	glViewport(0, 0, hwidth, hheight);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, texturenoise);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_RECTANGLE, texture[0]);

	//SIZE PIXELS TO SPACE = z * size * 2.0f * tan(camera.FieldOfViewV()/2.0f) / Window::Height()
	//SIZE SPACE TO PIXELS = size * Window::Height() / (2.0f * z * tan(camera.FieldOfViewV()/2.0f))

	Shader::VO::Use();
	Shader::VO::Z(0);
	Shader::VO::N(1);
	Shader::VO::R(2);
	Shader::VO::sample(ndisksamples, disksamples);
	Shader::VO::zc(cam.ZC());
	Shader::VO::wc((float)hheight * radius / tan((float)cam.FieldOfViewV()));

	Quad::Draw();

	//

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer[1]);
	glViewport(0, 0, 2 * hwidth, 2 * hheight);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_RECTANGLE, texture[0]);

	Shader::AS1::Use();
	Shader::AS1::Z(0);
	Shader::AS1::A(2);
	Shader::AS1::zc(cam.ZC());

	Quad::Draw();
}


void Ambient2::Resize(Camera &cam)
{	
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_RECTANGLE, texture[1]);

	Shader::AS2::Use();
	Shader::AS2::A(2);
	Shader::AS2::zc(cam.ZC());

	Quad::Draw();
}
