#pragma once

#include "Light.h"

class Camera;


class DirectionalLight: public Light
{
public:

	void Draw(Camera &cam);

	void Direction(const Vector3 &p){ direction = p; };
	//void Direction(const float *p){ direction = Vector3(*(__m128*)p); };
	void Direction(const float &x, const float &y, const float &z){ direction = Vector3(x, y, z); };

	Vector3 *Direction(){ return &direction; };

private:

	Vector3 direction;
};
