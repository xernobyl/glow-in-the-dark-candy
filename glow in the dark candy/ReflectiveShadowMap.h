#include "OpenGL.h"
#include "Ambient.h"

class Renderer;


class ReflectiveShadowMap
{
public:

	ReflectiveShadowMap(unsigned size, unsigned npoints);
	~ReflectiveShadowMap();

	bool Error() const{ return error; };

	void Begin();
	void End(float spot, Matrix4 *lm, Matrix4 *cm);

	void RenderLights(Renderer *render, Camera *camera);

	void DrawPoints();

	unsigned TextureDepth() const{ return texture[TX_DEPTH]; };

private:

	enum
	{
		TX_DEPTH = 0,
		TX_REF,
		TX_POS,
		TX_NOR,
		TX_COL,
		N_TEXTURES,
		B_POS = 0,
		B_NOR,
		B_COL
	};

	unsigned TextureReflective() const{ return texture[TX_REF]; };
	unsigned TexturePosition() const{ return texture[TX_POS]; };
	unsigned TextureNormal() const{ return texture[TX_NOR]; };
	unsigned TextureColor() const{ return texture[TX_COL]; };

	unsigned NLights() const{ return npoints; };

	unsigned res;
	unsigned texture[N_TEXTURES];
	unsigned framebuffer;
	bool error;

	unsigned npoints;
	float *points;
	unsigned pointsva, pointsvb;

	unsigned tfvb[3], tfva;
};
