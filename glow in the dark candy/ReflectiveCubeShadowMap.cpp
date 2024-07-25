#include "ReflectiveCubeShadowMap.h"
#include "Shaders.h"


ReflectiveCubeShadowMap::ReflectiveCubeShadowMap(unsigned size, unsigned npoints):
res(size), error(false), npoints(npoints)
{
	rdirs = (Vector3*)_aligned_malloc(npoints * sizeof(Vector3), 16);
	
	for(unsigned i=0; i<npoints; ++i)
	{
		float u, v;
		u = (float) rand() / RAND_MAX;
		v = (float) rand() / RAND_MAX;

		float theta = 6.2831853072f * u;
		float phi = acos(2.0f*v - 1.0f);

		rdirs[i] = Vector3(cos(theta) * sin(phi), sin(theta) * sin(phi), cos(phi));
	}

	glGenVertexArrays(1, &rdirsva);
	glGenBuffers(1, &rdirsvb);

	glBindVertexArray(rdirsva);
	glBindBuffer(GL_ARRAY_BUFFER, rdirsvb);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3)*npoints, rdirs, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), 0);
	glEnableVertexAttribArray(0);

	////

	glGenTextures(N_TEXTURES, texture);
	glGenFramebuffers(1, &framebuffer);

	glBindTexture(GL_TEXTURE_CUBE_MAP, TextureDepth());
	glTexImage2D(GL_TEXTURE_CUBE_MAP, 0, GL_DEPTH_COMPONENT32F, res, res, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, TextureReflective());
	glTexImage2D(GL_TEXTURE_CUBE_MAP, 0, GL_RGBA16F, res, res, 0, GL_RGBA, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

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


ReflectiveCubeShadowMap::~ReflectiveCubeShadowMap()
{
	_aligned_free(rdirs);
	
	glDeleteTextures(N_TEXTURES, texture);
	glDeleteFramebuffers(1, &framebuffer);

	glDeleteVertexArrays(1, &tfva);
	glDeleteBuffers(3, tfvb);

	glDeleteVertexArrays(1, &rdirsva);
	glDeleteBuffers(1, &rdirsvb);
}


void ReflectiveCubeShadowMap::Begin()
{
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glViewport(0, 0, res, res);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}


void ReflectiveCubeShadowMap::End(float spot, Matrix4 *lm, Matrix4 *cm)
{
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
	glBindVertexArray(rdirsva);
	glDrawArrays(GL_POINTS, 0, npoints);
	glEndTransformFeedback();
}


void ReflectiveCubeShadowMap::RenderLights(Ambient *ambient)
{
	Shader::RSMLights::Use();
}


void ReflectiveCubeShadowMap::DrawPoints()
{
	glBindVertexArray(tfva);
	glDrawArrays(GL_POINTS, 0, npoints);
}
