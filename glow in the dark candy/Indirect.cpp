#include "Indirect.h"
#include "Camera.h"
#include "Shaders.h"
#include "Renderer.h"
#include "Quad.h"


Indirect::Indirect(int w, int h)
{
	qwidth = w/4;
	qheight = h/4;
	error = false;

	glGenTextures(4, texture);
	glGenFramebuffers(1, &framebuffer);

	glBindTexture(GL_TEXTURE_RECTANGLE, TextureSmall());
	glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGBA16F, qwidth, qheight, 0, GL_RGBA, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_RECTANGLE, TextureNormals());
	glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGB8, qwidth, qheight, 0, GL_RGB, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, TextureSmall(), 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, TextureNormals(), 0);
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		error = true;
		return;
	}
}


Indirect::~Indirect()
{
	glDeleteTextures(2, texture);
	glDeleteFramebuffers(1, &framebuffer);
}


void Indirect::Begin(Camera &camera)
{
	static const GLenum bufs[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glViewport(0, 0, qwidth, qheight);

	glDrawBuffers(2, bufs);

	glDisable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);

	glDepthMask(GL_FALSE);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
}


void Indirect::Finish()
{
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
}


void Indirect::Resize(Camera &camera, Renderer *render)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_RECTANGLE, TextureSmall());
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_RECTANGLE, TextureNormals());
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_RECTANGLE, render->TextureNormalShininess());
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_RECTANGLE, render->TextureDepth());

	Shader::GIScale::Use();
	Shader::GIScale::I(0);
	Shader::GIScale::N(1);
	Shader::GIScale::NS(2);
	Shader::GIScale::ZS(3);
	Shader::GIScale::t(0.125f);
	Shader::GIScale::za(camera.FarP()/(camera.FarP()-camera.NearP()));
	Shader::GIScale::zb(camera.FarP()*camera.NearP()/(camera.NearP()-camera.FarP()));

	Quad::Draw();
}
