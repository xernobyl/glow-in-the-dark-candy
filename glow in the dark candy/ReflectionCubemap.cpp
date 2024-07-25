#include "ReflectionCubemap.h"
#include "OpenGL.h"
#include "Camera.h"


ReflectionCubemap::ReflectionCubemap(unsigned size)
{
	glGenFramebuffers(6, framebuffer);
	glGenTextures(2, texture);

	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, size, size, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

	glBindTexture(GL_TEXTURE_CUBE_MAP, texture[0]);

	for(int i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA16F, size, size, 0, GL_RGBA, GL_FLOAT, 0);

		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, texture[0], 0);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture[1], 0);

		if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			error = true;
			return;
		}
	}

	error = false;
}


ReflectionCubemap::~ReflectionCubemap()
{
	glDeleteTextures(2, texture);
	glDeleteFramebuffers(6, framebuffer);
}


void ReflectionCubemap::Begin(Camera &camera)
{
	// POSITIVE Z
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer[GL_TEXTURE_CUBE_MAP_POSITIVE_Z - GL_TEXTURE_CUBE_MAP_POSITIVE_X]);
	glClear(GL_DEPTH_BUFFER_BIT);

	// POSITIVE Y
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer[GL_TEXTURE_CUBE_MAP_POSITIVE_Y - GL_TEXTURE_CUBE_MAP_POSITIVE_X]);
	glClear(GL_DEPTH_BUFFER_BIT);

	// NEGATIVE Y
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer[GL_TEXTURE_CUBE_MAP_NEGATIVE_Y - GL_TEXTURE_CUBE_MAP_POSITIVE_X]);
	glClear(GL_DEPTH_BUFFER_BIT);

	// POSITIVE X
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer[GL_TEXTURE_CUBE_MAP_POSITIVE_X - GL_TEXTURE_CUBE_MAP_POSITIVE_X]);
	glClear(GL_DEPTH_BUFFER_BIT);

	// NEGATIVE X
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer[GL_TEXTURE_CUBE_MAP_NEGATIVE_X - GL_TEXTURE_CUBE_MAP_POSITIVE_X]);
	glClear(GL_DEPTH_BUFFER_BIT);
}


void ReflectionCubemap::Finish()
{
}
