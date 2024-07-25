#include "Camera.h"


void Camera::LookAt(const Point3 &pos, const Point3 &t, const Vector3 &up)
{
	Quat q;
	
	viewm = Matrix4::lookAt(pos, t, up);
	mvpm = projm * viewm;
}


void Camera::FocalLength(const double &fovmm)
{
	double dfov = 2.0*atan(18.0/fovmm);
	fov = sqrt(dfov*dfov/(1.0+aspect*aspect));
	focallength = fovmm / 1000.0;
}


void Camera::UpdateProjection()
{
	zc[0] = (nearp - farp) / (farp * nearp);
	zc[1] = (farp + nearp) / (nearp * (farp - nearp));

	hfov[0] = tan(aspect * fov * 0.5);
	hfov[1] = tan(fov * 0.5);

	projm = Matrix4::perspective((float)fov, (float)aspect, (float)nearp, (float)farp);
}


void Camera::Focus(const double &distance, const double &aperture)
{
	double diskar = 1.0;
	
	focusdistance = distance;
	this->aperture = aperture;

	coc[0] = -aperture * diagonal * focallength * distance / (distance - focallength);
	coc[1] = aperture * diagonal * focallength / (distance - focallength);

	if(illuminate)
		illuminance = aperture / sqrt(2.0);
	else
		illuminance = 1.0;

	float angle0 = 13.37 * aperture;	// random'ish
	float angle1 = angle0 + 2.0943951023931954923084289221863f;
	float angle2 = angle0 - 2.0943951023931954923084289221863f;

	step0[0] = sin(angle0) * diskar;
	step0[1] = cos(angle0);
	float tmp = sqrt(step0[0] * step0[0] + step0[1] * step0[1]);
	step0[0] /= tmp;
	step0[1] /= tmp;

	step1[0] = sin(angle1) * diskar;
	step1[1] = cos(angle1);
	tmp = sqrt(step1[0] * step1[0] + step1[1] * step1[1]);
	step1[0] /= tmp;
	step1[1] /= tmp;

	step2[0] = sin(angle2) * diskar;
	step2[1] = cos(angle2);
	tmp = sqrt(step2[0] * step2[0] + step2[1] * step2[1]);
	step2[0] /= tmp;
	step2[1] /= tmp;
}
