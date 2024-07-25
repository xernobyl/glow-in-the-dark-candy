#include "PostProcessing.h"
#include "Quad.h"


PostProcessing::PostProcessing(int w, int h):
width(w), height(h)
{
	bool error = false;

	glGenFramebuffers(1, &framebuffer);
}


PostProcessing::~PostProcessing()
{
	glDeleteFramebuffers(1, &framebuffer);
}


void PostProcessing::Draw()
{
	Quad::Draw();
}
