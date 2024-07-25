#include "SpotLight.h"
#include "Camera.h"
#include "Shaders.h"
#include "Quad.h"


void SpotLight::Draw(Camera &cam)
{
	Vector4 d = *cam.MV() * direction;
	d = normalize(d);

	Vector4 p = *cam.MV() * position;

	/*Shader::Spot::Use();

	Shader::Spot::Z(0);
	Shader::Spot::N(1);
	Shader::Spot::zc(cam.ZC());
	Shader::Spot::hfov(cam.HFOV());

	Shader::Spot::color(value[0] * cam.Illuminance(), value[1] * cam.Illuminance(), value[2] * cam.Illuminance());
	Shader::Spot::position((float*)&p);
	Shader::Spot::direction((float*)&p);*/

	Quad::Draw();
}


void SpotLight::Spot(const float &spot, const float &hardness)
{
	this->spot = spot;
	this->hardness = hardness;

	float csi = cos(hardness*spot * 0.0);
	float cso = cos(spot * 0.5);

	s[0] = -1.0f / (csi - cso);
	s[1] = s[0] * cso;
}
