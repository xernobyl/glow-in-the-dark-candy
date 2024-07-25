#pragma once

class GPUParticles
{
public:

	GPUParticles(unsigned nparticles);
	~GPUParticles();

	void Update();
	void Render();

private:

	unsigned nparticles;
	unsigned vbo[2];
};
