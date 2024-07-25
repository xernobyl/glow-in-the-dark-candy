#include "PointLight.h"
#include "Camera.h"
#include "Shaders.h"
#include "Quad.h"


void PointLight::Draw(Camera &cam)
{
	Vector4 p = *cam.MV() * position;

	Shader::Point::Use();

	Shader::Point::zc(cam.ZC());
	Shader::Point::hfov(cam.HFOV());

	Shader::Point::color(value[0] * cam.Illuminance(), value[1] * cam.Illuminance(), value[2] * cam.Illuminance());
	Shader::Point::position((float*)&p);

	Quad::Draw();
}
