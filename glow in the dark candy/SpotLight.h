#pragma once

#include "Light.h"

class Camera;


class SpotLight: Light
{
public:

	void Draw(Camera &cam);

	void Position(const Point3 &p){ position = p; };
	//void Position(const float *p){ position = Point3(*(__m128*)p); };
	void Position(const float &x, const float &y, const float &z){ position = Point3(x, y, z); };

	void Direction(const Vector3 &p){ direction = p; };
	//void Direction(const float *p){ direction = Vector3(*(__m128*)p); };
	void Direction(const float &x, const float &y, const float &z){ direction = Vector3(x, y, z); };

	void Spot(const float &spot, const float &hardness);

	Point3 *Position(){ return &position; };
	Vector3 *Direction(){ return &direction; };

	float Spot() const{ return spot; };
	float *Cone() const{ return (float*)s; };
	float Cone0() const{ return s[0]; };
	float Cone1() const{ return s[1]; };

private:

	//friend class ShadowMapCamera;

	Point3 position;
	Vector3 direction;

	float spot, hardness;
	float s[2];
};
