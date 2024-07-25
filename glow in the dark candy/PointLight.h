#pragma once

#include "Light.h"

class Camera;


class PointLight: public Light
{
public:

	void Draw(Camera &cam);

	void Position(const Point3 &p){ position = p; };
	//void Position(const float *p){ position = Point3(*(__m128*)p); };
	void Position(const float &x, const float &y, const float &z){ position = Point3(x, y, z); };

	Point3 *Position(){ return &position; };

private:

	Point3 position;
};
