#include "Resources.h"

Model model[N_MODELS];
unsigned texture[N_TEXTURES];


enum
{
	TX_REPEAT		= 0x01,
	TX_SRGB			= 0x02,
	TX_ANISO		= 0x04,
	TX_COMPRESS		= 0x08,
	TX_MIPMAP		= 0x10,
	TX_NEAREST		= 0x20,
	TX_6			= 0x30,
	TX_7			= 0x40,
};


static const char *modelpath[N_MODELS] =
{
	"3d/texturedcube.3d",
	"3d/sphere.3d",
	"3d/ninjahead.3d",
	"3d/r0.3d",
	"3d/r1.3d",
	"3d/r2.3d",
	"3d/r3.3d",
};

static const char *texturepath[N_TEXTURES] =
{
	"2d/rocksnm.png",
	"2d/rocksa.png",
	"2d/ninjanm.png",
};

static const char textureparam[N_TEXTURES] =
{
	TX_MIPMAP | TX_REPEAT | TX_ANISO,
	TX_MIPMAP | TX_REPEAT | TX_ANISO | TX_SRGB,
	TX_MIPMAP | TX_REPEAT | TX_ANISO,
};


bool InitModels()
{
	for(auto i = 0; i < N_MODELS; ++i)
	{
		if(!model[i].Load(modelpath[i]))
		{
			MessageBoxA(0, "Could not load model.", 0, 0);
			return false;
		}
	}

	return true;
}


bool InitTextures()
{
	float maxanisotropy;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxanisotropy);

	glGenTextures(N_TEXTURES, texture);

	for(auto i = 0; i < N_TEXTURES; ++i)
	{
		int x, y, n;
		unsigned char *data = stbi_load(texturepath[i], &x, &y, &n, 3);

		unsigned format, iformat;

		if(!data)
			return false;

		switch(n)
		{
		case 1:
			iformat = GL_R8;
			format = GL_RED;
			break;

		case 2:
			iformat = GL_RG8;
			format = GL_RG;
			break;

		case 3:
			if(textureparam[i] | TX_SRGB)
				iformat = GL_SRGB8;
			else
				iformat = GL_RGB8;

			format = GL_RGB;
			break;

		case 4:
			if(textureparam[i] | TX_SRGB)
				iformat = GL_SRGB8_ALPHA8;
			else
				iformat = GL_RGBA8;

			format = GL_RGBA;
			break;

		default:
			return false;
		}

		glBindTexture(GL_TEXTURE_2D, texture[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, iformat, x, y, 0, format, GL_UNSIGNED_BYTE, data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		if(textureparam[i] | TX_ANISO)
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxanisotropy);

		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(data);
	}

	return true;
}
