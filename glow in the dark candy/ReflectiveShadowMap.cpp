#include "ReflectiveShadowMap.h"
#include "Renderer.h"
#include "Camera.h"
#include "Shaders.h"
#include "Random.h"
#include "Quad.h"


ReflectiveShadowMap::ReflectiveShadowMap(unsigned size, unsigned npoints):
res(size), error(false), npoints(npoints)
{
	points = new float[2 * npoints];

	RandDisk(points, npoints);

	glGenVertexArrays(1, &pointsva);
	glGenBuffers(1, &pointsvb);

	glBindVertexArray(pointsva);
	glBindBuffer(GL_ARRAY_BUFFER, pointsvb);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*2*npoints, points, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glGenTextures(N_TEXTURES, texture);
	glGenFramebuffers(1, &framebuffer);

	glBindTexture(GL_TEXTURE_2D, TextureDepth());
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, res, res, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);

	glBindTexture(GL_TEXTURE_2D, TextureReflective());
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, res, res, 0, GL_RGBA, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, TextureReflective(), 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, TextureDepth(), 0);

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		error = true;
		return;
	}

	glGenVertexArrays(1, &tfva);
	glGenBuffers(3, tfvb);

	glBindVertexArray(tfva);

	glBindBuffer(GL_ARRAY_BUFFER, tfvb[B_POS]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*2*4*npoints, 0, GL_DYNAMIC_COPY);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, tfvb[B_NOR]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*2*4*npoints, 0, GL_DYNAMIC_COPY);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, tfvb[B_COL]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*2*4*npoints, 0, GL_DYNAMIC_COPY);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	glBindTexture(GL_TEXTURE_BUFFER, TexturePosition());
	glTexBuffer(GL_TEXTURE_BUFFER, GL_RGB32F, tfvb[B_POS]);

	glBindTexture(GL_TEXTURE_BUFFER, TextureNormal());
	glTexBuffer(GL_TEXTURE_BUFFER, GL_RGB32F, tfvb[B_NOR]);

	glBindTexture(GL_TEXTURE_BUFFER, TextureColor());
	glTexBuffer(GL_TEXTURE_BUFFER, GL_RGB32F, tfvb[B_COL]);
}


ReflectiveShadowMap::~ReflectiveShadowMap()
{
	delete [] points;
	
	glDeleteTextures(N_TEXTURES, texture);
	glDeleteFramebuffers(1, &framebuffer);

	glDeleteVertexArrays(1, &tfva);
	glDeleteBuffers(3, tfvb);

	glDeleteVertexArrays(1, &pointsva);
	glDeleteBuffers(1, &pointsvb);
}


void ReflectiveShadowMap::Begin()
{
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glViewport(0, 0, res, res);
	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glDepthMask(GL_TRUE);
}


void ReflectiveShadowMap::End(float spot, Matrix4 *lm, Matrix4 *cm)
{
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TextureReflective());

	Shader::RSMDisk::Use();
	Shader::RSMDisk::LZ(0);
	Shader::RSMDisk::w(1.0f/npoints);
	Shader::RSMDisk::tx(1.0f/res);
	Shader::RSMDisk::zs(-tan(spot/2.0f));
	
	Matrix4 m = inverse(*lm);
	m = *cm * m;

	Shader::RSMDisk::m((float*)&m);

	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, tfvb[0]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, tfvb[1]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, tfvb[2]);

	glBeginTransformFeedback(GL_POINTS);
	glBindVertexArray(pointsva);
	glDrawArrays(GL_POINTS, 0, npoints);
	glEndTransformFeedback();
}


void ReflectiveShadowMap::RenderLights(Renderer *render, Camera *camera)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_RECTANGLE, render->TextureDepth());
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_RECTANGLE, render->TextureNormalShininess());

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_BUFFER, TexturePosition());
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_BUFFER, TextureNormal());
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_BUFFER, TextureColor());

	Shader::RSMLights::Use();
	Shader::RSMLights::ZS(0);
	Shader::RSMLights::NS(1);
	Shader::RSMLights::P(2);
	Shader::RSMLights::N(3);
	Shader::RSMLights::C(4);

	Shader::RSMLights::za(camera->FarP()/(camera->FarP()-camera->NearP()));
	Shader::RSMLights::zb(camera->FarP()*camera->NearP()/(camera->NearP()-camera->FarP()));
	Shader::RSMLights::hfov(tan(camera->FieldOfViewH()/2.0), tan(camera->FieldOfViewV()/2.0));

	Shader::RSMLights::nlights(NLights());

	Quad::Draw();
}


void ReflectiveShadowMap::DrawPoints()
{
	glBindVertexArray(tfva);
	glDrawArrays(GL_POINTS, 0, npoints);
}
