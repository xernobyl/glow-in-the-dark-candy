#include "OpenGL.h"
#include "Ambient.h"


class ReflectiveCubeShadowMap
{
public:

	ReflectiveCubeShadowMap(unsigned size, unsigned npoints);
	~ReflectiveCubeShadowMap();

	bool Error() const{ return error; };

	unsigned TextureDepth() const{ return texture[TX_DEPTH]; };
	unsigned TextureReflective() const{ return texture[TX_REF]; };
	unsigned TexturePosition() const{ return texture[TX_POS]; };
	unsigned TextureNormal() const{ return texture[TX_NOR]; };
	unsigned TextureColor() const{ return texture[TX_COL]; };

	unsigned NLights() const{ return npoints; };

	void Begin();
	void End(float spot, Matrix4 *lm, Matrix4 *cm);

	void RenderLights(Ambient *ambient);

	void DrawPoints();

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

	unsigned res;
	unsigned texture[N_TEXTURES];
	unsigned framebuffer;
	bool error;

	unsigned npoints;
	Vector3 *rdirs;
	unsigned rdirsva, rdirsvb;

	unsigned tfvb[3], tfva;
};
