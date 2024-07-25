#include "GPUParticles.h"
#include "OpenGL.h"


GPUParticles::GPUParticles(unsigned nparticles):
nparticles(nparticles)
{
	typedef struct
	{
		float pos[3];
		float vel[3];
		float life;
	} Particle;

	Particle* particles = new Particle[nparticles];

	for(auto i = 0; i < nparticles; ++i)
	{
		particles[i].pos[0] = particles[i].pos[1] = particles[i].pos[2] = 0.0f;
		particles[i].vel[0] = particles[i].vel[1] = particles[i].vel[2] = 0.0f;

		float life = 0.0f;
	}

	glGenBuffers(2, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, nparticles * sizeof(Particle), particles, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, nparticles * sizeof(Particle), particles, GL_DYNAMIC_DRAW);

	delete [] particles;
}


GPUParticles::~GPUParticles()
{
	glDeleteBuffers(2, vbo);
}


void GPUParticles::Update()
{
	static auto frame = 0;
	
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, vbo[frame & 1]);
	
	glBeginTransformFeedback(GL_POINTS);
	glDrawArrays(GL_POINTS, 0, nparticles);
	glEndTransformFeedback();

	++frame;
}


void GPUParticles::Render()
{
}
