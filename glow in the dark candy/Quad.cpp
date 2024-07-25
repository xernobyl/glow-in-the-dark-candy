#include "Quad.h"
#include "OpenGL.h"


namespace Quad
{
	static unsigned quadvbo, quadvao;

	static const float quad[] =
	{
		-200.0f, 300.0f,
		-200.0f, -200.0f,
		300.0f, -200.0f
	};

	void Init()
	{
		glGenBuffers(1, &quadvbo);
		glGenVertexArrays(1, &quadvao);

		glBindBuffer(GL_ARRAY_BUFFER, quadvbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*2*3, quad, GL_STATIC_DRAW);

		glBindVertexArray(quadvao);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glEnableVertexAttribArray(0);
	}

	void Draw()
	{
		glBindVertexArray(quadvao);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

	void Kill()
	{
		glDeleteVertexArrays(1, &quadvao);
		glDeleteBuffers(1, &quadvbo);
	}
}
