#pragma once


class Bloom
{
public:

	Bloom(int width, int height);
	~Bloom();

	void Render(unsigned txhdr);

	unsigned Texture() const{ return texture[2]; };

	bool Error() const{ return error; };

private:

	int width, height;
	bool error;

	unsigned texture[3];
	unsigned framebuffer[3];
};
