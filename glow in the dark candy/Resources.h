#pragma once

#include "Model.h"
#include "stb_image.h"


enum
{
	M_TEXTUREDCUBE,
	M_SPHERE,
	M_NINJA,
	M_R0,
	M_R1,
	M_R2,
	M_R3,
	N_MODELS
};

enum
{
	T_ROCKSNM,
	T_ROCKSA,
	T_NINJANM,
	N_TEXTURES
};

extern Model model[N_MODELS];
extern unsigned texture[N_TEXTURES];

bool InitModels();
bool InitTextures();
