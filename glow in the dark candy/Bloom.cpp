#include "Bloom.h"
#include "Quad.h"
#include "OpenGL.h"
#include "Shaders.h"


Bloom::Bloom(int width, int height):
error(false),
width(width),
height(height)
{
	glGenTextures(3, texture);
	glGenFramebuffers(3, framebuffer);

	glBindTexture(GL_TEXTURE_RECTANGLE, texture[0]);
	glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGB8, width/2, height/2, 0, GL_RGB, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_RECTANGLE, texture[1]);
	glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGB8, width/4, height/4, 0, GL_RGB, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_RECTANGLE, texture[2]);
	glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGB8, width/4, height/4, 0, GL_RGB, GL_FLOAT, 0);
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

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer[2]);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture[2], 0);
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		error = true;
		return;
	}
}


Bloom::~Bloom()
{
	glDeleteTextures(3, texture);
	glDeleteFramebuffers(3, framebuffer);
}


void Bloom::Render(unsigned txhdr)
{
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer[0]);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_RECTANGLE, txhdr);
	Shader::Bloom2x::Use();
	Shader::Bloom2x::T(0);
	Quad::Draw();

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer[1]);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_RECTANGLE, texture[0]);
	Shader::Bloom4x::Use();
	Shader::Bloom4x::T(1);
	Quad::Draw();

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer[2]);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_RECTANGLE, texture[1]);
	Shader::Bloom::Use();
	Shader::Bloom::T(2);
	Quad::Draw();
}
