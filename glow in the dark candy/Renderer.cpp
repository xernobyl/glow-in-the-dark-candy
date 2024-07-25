#include "Renderer.h"
#include "Window.h"
#include "Shaders.h"
#include "Quad.h"
#include "Camera.h"
#include "Random.h"


Renderer::Renderer(int w, int h):
width(w), height(h), error(false)
{	
	diagonal = sqrt((double)(w*w + h*h));

	glGenTextures(N_FBTX, texture);
	glGenFramebuffers(N_FB, framebuffer);

	glBindTexture(GL_TEXTURE_RECTANGLE, TextureDepth());
	glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, 0);
	//glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_DEPTH32F_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_FLOAT_32_UNSIGNED_INT_24_8_REV, 0);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_RECTANGLE, TextureNormalShininess());
	glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_RECTANGLE, TextureDiffuse());
	glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_R11F_G11F_B10F, width, height, 0, GL_RGB, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_RECTANGLE, TextureSpecular());
	glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_R11F_G11F_B10F, width, height, 0, GL_RGB, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	glBindTexture(GL_TEXTURE_RECTANGLE, TextureAccumulation());
	glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_RECTANGLE, TextureHalf());
	glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGBA16F, width / 2, height / 2, 0, GL_RGBA, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	glBindTexture(GL_TEXTURE_RECTANGLE, TextureDOFAux0());
	glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGBA16F, width / 2, height / 2, 0, GL_RGBA, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	glBindTexture(GL_TEXTURE_RECTANGLE, TextureDOFAux1());
	glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGBA16F, width / 2, height / 2, 0, GL_RGBA, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	glBindTexture(GL_TEXTURE_RECTANGLE, TextureDOF());
	glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_R11F_G11F_B10F, width / 2, height / 2, 0, GL_RGB, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_RECTANGLE, TextureQuarter());
	glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_R11F_G11F_B10F, width / 4, height / 4, 0, GL_RGB, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_RECTANGLE, TextureGlow());
	glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_R11F_G11F_B10F, width / 4, height / 4, 0, GL_RGB, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_RECTANGLE, TextureSRGB());
	glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_SRGB8_ALPHA8, width, height, 0, GL_RGBA, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_RECTANGLE, TextureHalfAmbient());
	glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGBA8, width / 2, height / 2, 0, GL_RGBA, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_RECTANGLE, TextureAmbient());
	glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	unsigned char *noisebuffer = new unsigned char[NOISE_RES * NOISE_RES * 2];

	for(unsigned i = 0; i < NOISE_RES * NOISE_RES * 2; ++i)
		noisebuffer[i] = (unsigned char)Rand(256.0f);

	glBindTexture(GL_TEXTURE_2D, TextureNoise());
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RG8, NOISE_RES, NOISE_RES, 0, GL_RG, GL_UNSIGNED_BYTE, noisebuffer);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	delete [] noisebuffer;

	// Framebuffers
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer[FB_DEFERRED]);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, TextureNormalShininess(), 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, TextureDepth(), 0);
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		error = true;
		return;
	}

	glClearStencil(-1);
	glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer[FB_LIGHT]);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, TextureDiffuse(), 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, TextureSpecular(), 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, TextureDepth(), 0);

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		error = true;
		return;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer[FB_ACCUM]);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, TextureAccumulation(), 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, TextureDepth(), 0);

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		error = true;
		return;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer[FB_HALF]);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, TextureHalf(), 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, TextureDOFAux0(), 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, TextureDOFAux1(), 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, TextureDOF(), 0);
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		error = true;
		return;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer[FB_QUARTER]);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, TextureQuarter(), 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, TextureGlow(), 0);
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		error = true;
		return;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer[FB_SRGB]);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, TextureSRGB(), 0);
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		error = true;
		return;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer[FB_HALFAMBIENT]);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, TextureHalfAmbient(), 0);
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		error = true;
		return;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer[FB_AMBIENT]);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, TextureAmbient(), 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, TextureDepth(), 0);
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		error = true;
		return;
	}

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_RECTANGLE, TextureDepth());
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_RECTANGLE, TextureNormalShininess());
	glActiveTexture(GL_TEXTURE2);

	ambientradius = 48.0f;
	ndisksamples = 24;
	disksamples = new float[3*ndisksamples];
	RandDisk(disksamples, ndisksamples, 1.0f, 3);

	float totalweight = 0.0f;

	for(unsigned i=0; i<ndisksamples; ++i)
		totalweight += (disksamples[i*3 + 2] = (float)sqrt(1.0 - disksamples[i*3 + 0] * disksamples[i*3 + 0] - disksamples[i*3 + 1] * disksamples[i*3 + 1]));

	Shader::VO::Use();
	Shader::VO::Z(0);
	Shader::VO::R(2);
	Shader::VO::sample(ndisksamples, disksamples);
	Shader::VO::tw(1.0f / totalweight);

	frame = -1;
}


Renderer::~Renderer()
{
	delete [] disksamples;

	glDeleteTextures(N_FBTX, texture);
	glDeleteFramebuffers(N_FB, framebuffer);
}


void Renderer::BeginFirstPass()
{
	++frame;

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer[FB_DEFERRED]);
	glViewport(0, 0, width, height);

	glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	//glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

	glEnable(GL_STENCIL_TEST);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}


void Renderer::FinishFirstPass()
{
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_STENCIL_TEST);

	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
}


void Renderer::Material(const unsigned &f)
{
	glStencilFunc(GL_ALWAYS, (frame & 1) | f, 0xff);
}


void Renderer::BeginLightPass(const bool &clear)
{
	static const unsigned buffers[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer[FB_LIGHT]);
	glViewport(0, 0, width, height);
	glDrawBuffers(2, buffers);

	if(clear)
	{
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_RECTANGLE, TextureDepth());
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_RECTANGLE, TextureNormalShininess());

	glBlendFunc(GL_ONE, GL_ONE);
	
	glDepthMask(GL_FALSE);

	glStencilFunc(GL_EQUAL, (frame & 1) | M_LIGHTEN, M_FRAMEPARITY | M_LIGHTEN);
	glEnable(GL_STENCIL_TEST);

	//glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
}


void Renderer::FinishLightPass()
{
	glDepthMask(GL_TRUE);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glDisable(GL_STENCIL_TEST);
}


void Renderer::BeginSecondPass()
{
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer[FB_ACCUM]);
	glViewport(0, 0, width, height);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_RECTANGLE, TextureDiffuse());
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_RECTANGLE, TextureSpecular());

	glDepthFunc(GL_EQUAL);
	glDepthMask(GL_FALSE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}


void Renderer::FinishSecondPass()
{
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);
}


void Renderer::RenderPost(Camera &camera)
{	
	static const unsigned buffers[2] = { GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_RECTANGLE, TextureAccumulation());
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_RECTANGLE, TextureHalf());
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_RECTANGLE, TextureDOFAux0());
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_RECTANGLE, TextureDOFAux1());
	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_RECTANGLE, TextureDOF());
	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_RECTANGLE, TextureQuarter());
	glActiveTexture(GL_TEXTURE8);
	glBindTexture(GL_TEXTURE_RECTANGLE, TextureGlow());

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer[FB_HALF]);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	glViewport(0, 0, width / 2, height / 2);

	Shader::Half::Use();
	Shader::Half::S(2);
	Quad::Draw();
	
	glDrawBuffers(2, buffers);

	Shader::DOF0::Use();
	Shader::DOF0::S(3);
	Shader::DOF0::step0(camera.Step0());
	Shader::DOF0::step1(camera.Step1());
	Quad::Draw();

	glDrawBuffer(GL_COLOR_ATTACHMENT3);

	Shader::DOF1::Use();
	Shader::DOF1::S(3);
	Shader::DOF1::S0(4);
	Shader::DOF1::S1(5);
	Shader::DOF1::step1(camera.Step1());
	Shader::DOF1::step2(camera.Step2());
	Quad::Draw();

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer[FB_QUARTER]);
	glDrawBuffer(GL_COLOR_ATTACHMENT1);
	glViewport(0, 0, width / 4, height / 4);

	Shader::Max::Use();
	Shader::Max::S(6);
	Quad::Draw();

	glDrawBuffer(GL_COLOR_ATTACHMENT0);

	Shader::Glow0::Use();
	Shader::Glow0::S(8);
	Quad::Draw();

	glDrawBuffer(GL_COLOR_ATTACHMENT1);

	Shader::Glow1::Use();
	Shader::Glow1::S(7);
	Quad::Draw();
}


void Renderer::RenderCombine()
{
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_RECTANGLE, TextureAccumulation());
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_RECTANGLE, TextureDOF());
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_RECTANGLE, TextureGlow());
	
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer[FB_SRGB]);
	glViewport(0, 0, Width(), Height());

	glEnable(GL_FRAMEBUFFER_SRGB);

	Shader::Combine::Use();
	Shader::Combine::A(2);
	Shader::Combine::D(3);
	Shader::Combine::G(4);
	Quad::Draw();

	glDisable(GL_FRAMEBUFFER_SRGB);
}


void Renderer::RenderFXAA()
{
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_RECTANGLE, TextureSRGB());

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, Window::Width(), Window::Height());
	
	glEnable(GL_FRAMEBUFFER_SRGB);

	Shader::FXAA::Use();
	Shader::FXAA::S(2);
	Shader::FXAA::s((double)Width() / Window::Width(), (double)Height() / Window::Height());
	Quad::Draw();

	glDisable(GL_FRAMEBUFFER_SRGB);
}


void Renderer::RenderSGRB()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, Window::Width(), Window::Height());
	
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_RECTANGLE, TextureSRGB());

	glEnable(GL_FRAMEBUFFER_SRGB);

	Shader::Texture::S(2);
	Quad::Draw();

	glDisable(GL_FRAMEBUFFER_SRGB);
}


void Renderer::AmbientPass(Camera &camera)
{
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer[FB_HALFAMBIENT]);
	glViewport(0, 0, width / 2, height / 2);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, TextureNoise());
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_RECTANGLE, texture[0]);

	//SIZE PIXELS TO SPACE = z * size * 2.0f * tan(camera.FieldOfViewV()/2.0f) / Window::Height()
	//SIZE SPACE TO PIXELS = size * Window::Height() / (2.0f * z * tan(camera.FieldOfViewV()/2.0f))

	Shader::VO::Use();
	Shader::VO::zc(camera.ZC());
	Shader::VO::r(ambientradius);

	Quad::Draw();

	//

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer[FB_AMBIENT]);
	glViewport(0, 0, width, height);

	glStencilFunc(GL_EQUAL, (frame & 1) | M_AMBIENT, M_FRAMEPARITY | M_AMBIENT);
	glEnable(GL_STENCIL_TEST);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_RECTANGLE, TextureHalfAmbient());

	Shader::AS1::Use();
	Shader::AS1::Z(0);
	Shader::AS1::A(2);
	Shader::AS1::zc(camera.ZC());

	Quad::Draw();

	glDisable(GL_STENCIL_TEST);
}


void Renderer::DrawAmbient(Camera &camera)
{
	glStencilFunc(GL_EQUAL, (frame & 1) | M_AMBIENT, M_FRAMEPARITY | M_AMBIENT);
	glEnable(GL_STENCIL_TEST);
	
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_RECTANGLE, TextureAmbient());

	Shader::AS2::Use();
	Shader::AS2::A(2);
	Shader::AS2::zc(camera.ZC());

	Quad::Draw();

	glDisable(GL_STENCIL_TEST);
}
