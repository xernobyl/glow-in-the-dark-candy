#include "DirectionalLight.h"
#include "Camera.h"
#include "Shaders.h"
#include "Quad.h"


void DirectionalLight::Draw(Camera &cam)
{
	Vector4 v = *cam.MV() * direction;
	v = -v;
	v = normalize(v);

	Shader::Directional::Use();
	Shader::Directional::Z(0);
	Shader::Directional::N(1);
	Shader::Directional::zc(cam.ZC());
	Shader::Directional::hfov(cam.HFOV());

	Shader::Directional::color(value[0] * cam.Illuminance(), value[1] * cam.Illuminance(), value[2] * cam.Illuminance());
	Shader::Directional::direction((float*)&v);

	Quad::Draw();
}
