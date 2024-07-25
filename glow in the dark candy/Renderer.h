#pragma once

#include "OpenGL.h"

/*
Stencil:
FRAME_PARITY
DRAW_LIGHTS
DRAW_AMBIENT
*/


class Camera;

class Renderer
{
public:

	enum
	{
		M_0				= 0x80,
		M_1				= 0x40,
		M_2				= 0x20,
		M_3				= 0x10,
		M_4				= 0x08,
		M_AMBIENT		= 0x04,
		M_LIGHTEN		= 0x02,
		M_FRAMEPARITY	= 0x01,

		NOISE_RES = 32
	};

	Renderer(int w, int h);
	~Renderer();

	unsigned TextureDepth() const { return texture[FBTX_Z]; };
	unsigned TextureNormalShininess() const { return texture[FBTX_NS]; };
	unsigned TextureDiffuse() const { return texture[FBTX_DIFF]; };
	unsigned TextureSpecular() const { return texture[FBTX_SPEC]; };
	unsigned TextureAccumulation() const { return texture[FBTX_ACCUM]; };
	unsigned TextureHalf() const { return texture[FBTX_HALF]; };
	unsigned TextureDOF() const { return texture[FBTX_DOF]; };
	unsigned TextureDOFAux0() const { return texture[FBTX_DOFAUX0]; };
	unsigned TextureDOFAux1() const { return texture[FBTX_DOFAUX1]; };
	unsigned TextureQuarter() const { return texture[FBTX_QUARTER]; };
	unsigned TextureGlow() const { return texture[FBTX_GLOW]; };
	unsigned TextureSRGB() const { return texture[FBTX_SRGB]; };
	unsigned TextureHalfAmbient() const { return texture[FBTX_HALFAMBIENT]; };
	unsigned TextureAmbient() const { return texture[FBTX_AMBIENT]; };
	unsigned TextureNoise() const { return texture[TX_NOISE]; };

	bool Error() const{ return error; };

	unsigned Width() const{ return width; };
	unsigned Height() const{ return height; };
	double Diagonal() const{ return diagonal; };

	void AmbientThickness(const float &t){ ambientthickness = t; };
	void AmbientRadius(const float &r){ ambientradius = r; };

	void AmbientPass(Camera &camera);
	void DrawAmbient(Camera &camera);

	void BeginFirstPass();
	void FinishFirstPass();

	void BeginLightPass(const bool &clear);
	void FinishLightPass();

	void BeginSecondPass();
	void FinishSecondPass();

	void RenderPost(Camera &camera);
	void RenderCombine();
	void RenderFXAA();
	void RenderSGRB();

	void Material(const unsigned &f);

	unsigned frame;

private:

	enum
	{
		FBTX_Z = 0,
		FBTX_NS,
		FBTX_DIFF,
		FBTX_SPEC,
		FBTX_ACCUM,
		FBTX_HALF,
		FBTX_DOF,
		FBTX_DOFAUX0,
		FBTX_DOFAUX1,
		FBTX_QUARTER,
		FBTX_GLOW,
		FBTX_SRGB,
		FBTX_HALFAMBIENT,
		FBTX_AMBIENT,
		TX_NOISE,
		N_FBTX,

		FB_DEFERRED = 0,	// Z, NS
		FB_LIGHT,			// DIFF, SPEC
		FB_ACCUM,			// ACCUM
		FB_HALF,			// HALF, DOF, DOFAUX
		FB_QUARTER,			// QUARTER, GLOW
		FB_SRGB,			// SRGB
		FB_HALFAMBIENT,
		FB_AMBIENT,
		N_FB,
	};

	
	
	bool error;
	int width, height;
	double diagonal;

	unsigned texture[N_FBTX];
	unsigned framebuffer[N_FB];

	float ambientradius, ambientthickness;
	float *disksamples;
	unsigned ndisksamples;
};
