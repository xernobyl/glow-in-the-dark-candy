#pragma once

#include "OpenGL.h"


class Camera
{
public:

	void Focus(const double &distance, const double &aperture);
	double *Step0(){ return step0; };
	double *Step1(){ return step1; };
	double *Step2(){ return step2; };

	double *COC(){ return coc; };

	double Illuminance() const{ return illuminance; };

	void LookAt(const Point3 &pos, const Point3 &t, const Vector3 &up);

	void UpdateProjection();

	void FocalLength(const double &fovmm);
	void FieldOfView(const double &fovy){ fov = fovy; };
	double FieldOfViewH() const{ return fov * aspect; };
	double FieldOfViewV() const{ return fov; };

	void AspectRatio(const double &ar, const double &d){ aspect = ar; diagonal = d * 1000.0 / 35.0; };
	double AspectRatio() const{ return aspect; };

	void NearP(const double &n){ nearp = n; };
	void FarP(const double &f){ farp = f; };

	void Illuminate(const bool &b){ illuminate = b; };

	double NearP() const{ return nearp; };
	double FarP() const{ return farp; };

	double *ZC(){ return zc; };

	double *HFOV(){ return hfov; };

	Matrix4 *MVP(){ return &mvpm; };
	Matrix4 *MV(){ return &viewm; };
	Matrix4 *P(){ return &projm; };

private:

	Matrix4 viewm, projm, mvpm, lastm;

	double fov;				// vertical fov
	double aspect;			// aspect ratio
	double nearp, farp;

	double zc[2];
	double coc[2];
	double hfov[2];

	bool illuminate;

	double focallength, focusdistance, aperture, diagonal;
	double illuminance;
	double step0[2], step1[2], step2[2];
};
