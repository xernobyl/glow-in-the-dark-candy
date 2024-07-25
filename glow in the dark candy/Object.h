#pragma once

#include "Shaders.h"
#include "Camera.h"


class Object
{
public:

	virtual void Draw(Camera *camera) = 0;

	Transform3 *TM(){ return &matrix; };

protected:

	Transform3 matrix;
};
