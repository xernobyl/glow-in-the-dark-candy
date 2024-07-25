#pragma once

#include "OpenGL.h"

#define L(X) sizeof(X)/sizeof(char*)

#define START_SHADER(NAME, ...)\
namespace NAME\
{\
	__declspec(selectany) const char *uniforms[] = { __VA_ARGS__ };\
	__declspec(selectany) int uniformloc[L(uniforms)];\
	inline void InitUniforms()\
	{\
		for(unsigned i=0; i<L(uniforms); ++i)\
			uniformloc[i] = glGetUniformLocation(shaders[SP_##NAME], uniforms[i]);\
	}\
	inline void Use(){ glUseProgram(shaders[SP_##NAME]); }

#define END_SHADER()\
}

#define HIDOUBLEWORD(x)	((x>>32) & 0xffffffff)
#define LODOUBLEWORD(x)	((x)     & 0xffffffff)


#ifdef _M_X64
#define P(X) (unsigned)LODOUBLEWORD((unsigned __int64)X), (unsigned)HIDOUBLEWORD((unsigned __int64)X)
#else
#define P(X) (unsigned)X
#endif


namespace Shader
{
#define FRAGMENTSHADERS	\
	FS(object)			\
	FS(objectnm)		\
	FS(objectosnm)		\
	FS(solid)			\
	FS(albedo)			\
	FS(emissive)		\
	FS(half)			\
	FS(max)				\
	FS(dof0)			\
	FS(dof1)			\
	FS(glow0)			\
	FS(glow1)			\
	FS(vo)				\
	FS(voblur)			\
	FS(voscale)			\
	FS(texture)			\
	FS(directional)		\
	FS(point)			\
	FS(combine)			\
	FS(fxaa)			\
	FS(background)		\
	FS(as1)				\
	FS(as2)				\
	FS(hemilight)		\
	FS(as2_hemilight)	\

#define VERTEXSHADERS	\
	_VS(object, 0x4000)	\
	VS(objectnm)		\
	VS(objectosnm)		\
	VS(solid)			\
	VS(albedo)			\
	VS(flat)			\
	VS(light)			\
	VS(background)		\

/*#define GEOMETRYSHADERS	\
	_GS(none, 0x8000)	\
	GS(none)			\*/

#include "shaders.def"

enum
{
	SP_Object,
	SP_ObjectNM,
	SP_ObjectOSNM,
	SP_Solid,
	SP_Albedo,
	SP_Emissive,
	SP_Half,
	SP_Max,
	SP_DOF0,
	SP_DOF1,
	SP_Glow0,
	SP_Glow1,
	SP_Texture,					// temp
	SP_VO,
	SP_VOBlur,
	SP_VOScale,
	SP_Directional,
	SP_Point,
	SP_Combine,
	SP_FXAA,
	SP_Background,
	SP_AS1,
	SP_AS2,
	SP_HemiLight,
	SP_AS2_HemiLight,
	N_SHADERPROGS
};

static const unsigned shadertable[] =
{
	2, FS_object, VS_object,
	2, FS_objectnm, VS_objectnm,
	2, FS_objectosnm, VS_objectosnm,
	2, FS_solid, VS_solid,
	2, FS_albedo, VS_albedo,
	2, FS_emissive, VS_solid,
	2, FS_half, VS_flat,
	2, FS_max, VS_flat,
	2, FS_dof0, VS_flat,
	2, FS_dof1, VS_flat,
	2, FS_glow0, VS_flat,
	2, FS_glow1, VS_flat,
	2, FS_texture, VS_flat,		// temp
	2, FS_vo, VS_flat,
	2, FS_voblur, VS_flat,
	2, FS_voscale, VS_flat,
	2, FS_directional, VS_light,
	2, FS_point, VS_light,
	2, FS_combine, VS_flat,
	2, FS_fxaa, VS_flat,
	2, FS_background, VS_background,
	2, FS_as1, VS_flat,
	2, FS_as2, VS_flat,
	2, FS_hemilight, VS_light,
	2, FS_as2_hemilight, VS_light,
	0
};

__declspec(selectany) unsigned shaders[N_SHADERPROGS];

#undef P

	START_SHADER(Object, "m", "nm", "s");
		inline void m(const float *a){ glUniformMatrix4fv(uniformloc[0], 1, 0, a); }
		inline void nm(const float *a){ glUniformMatrix4fv(uniformloc[1], 1, 0, a); }
		inline void s(const float &a){ glUniform1f(uniformloc[2], a); }
		inline void s(const unsigned &a){ glUniform1ui(uniformloc[2], a); }
	END_SHADER();

	START_SHADER(ObjectNM, "m", "nm", "s", "N");
		inline void m(const float *a){ glUniformMatrix4fv(uniformloc[0], 1, 0, a); }
		inline void nm(const float *a){ glUniformMatrix4fv(uniformloc[1], 1, 0, a); }
		inline void s(const float &a){ glUniform1f(uniformloc[2], a); }
		inline void s(const unsigned &a){ glUniform1ui(uniformloc[2], a); }
		inline void N(const int &a){ glUniform1i(uniformloc[3], a); }
	END_SHADER();

	START_SHADER(ObjectOSNM, "m", "nm", "s", "N");
		inline void m(const float *a){ glUniformMatrix4fv(uniformloc[0], 1, 0, a); }
		inline void nm(const float *a){ glUniformMatrix4fv(uniformloc[1], 1, 0, a); }
		inline void s(const float &a){ glUniform1f(uniformloc[2], a); }
		inline void s(const unsigned &a){ glUniform1ui(uniformloc[2], a); }
		inline void N(const int &a){ glUniform1i(uniformloc[3], a); }
	END_SHADER();

	START_SHADER(Solid, "m", "D", "S", "c", "dc", "sc");
		inline void m(const float *a){ glUniformMatrix4fv(uniformloc[0], 1, 0, a); }
		inline void D(const int &a){ glUniform1i(uniformloc[1], a); }
		inline void S(const int &a){ glUniform1i(uniformloc[2], a); }
		inline void c(const double *a){ glUniform2dv(uniformloc[3], 1, a); }
		inline void dc(const float *a){ glUniform3fv(uniformloc[4], 1, a); }
		inline void dc(const float &r, const float &g, const float &b){ glUniform3f(uniformloc[4], r, g, b); }
		inline void sc(const float *a){ glUniform3fv(uniformloc[5], 1, a); }
		inline void sc(const float &r, const float &g, const float &b){ glUniform3f(uniformloc[5], r, g, b); }
	END_SHADER();

	START_SHADER(Albedo, "m", "D", "S", "c", "A");
		inline void m(const float *a){ glUniformMatrix4fv(uniformloc[0], 1, 0, a); }
		inline void D(const int &a){ glUniform1i(uniformloc[1], a); }
		inline void S(const int &a){ glUniform1i(uniformloc[2], a); }
		inline void c(const double *a){ glUniform2dv(uniformloc[3], 1, a); }
		inline void A(const int &a){ glUniform1i(uniformloc[4], a); }		
	END_SHADER();

	START_SHADER(Emissive, "m", "c", "ec");
		inline void m(const float *a){ glUniformMatrix4fv(uniformloc[0], 1, 0, a); }
		inline void c(const double *a){ glUniform2dv(uniformloc[1], 1, a); }
		inline void ec(const float *a){ glUniform3fv(uniformloc[2], 1, a); }
		inline void ec(const float &r, const float &g, const float &b){ glUniform3f(uniformloc[2], r, g, b); }
	END_SHADER();

	START_SHADER(Half, "S");
		inline void S(const int &a){ glUniform1i(uniformloc[0], a); }
	END_SHADER();

	START_SHADER(Max, "S");
		inline void S(const int &a){ glUniform1i(uniformloc[0], a); }
	END_SHADER();

	START_SHADER(DOF0, "S", "step0", "step1");
		inline void S(const int &a){ glUniform1i(uniformloc[0], a); }
		inline void step0(const double *a){ glUniform2dv(uniformloc[1], 1, a); }
		inline void step1(const double *a){ glUniform2dv(uniformloc[2], 1, a); }
	END_SHADER();

	START_SHADER(DOF1, "S0", "S1", "step1", "step2", "S");
		inline void S0(const int &a){ glUniform1i(uniformloc[0], a); }
		inline void S1(const int &a){ glUniform1i(uniformloc[1], a); }
		inline void step1(const double *a){ glUniform2dv(uniformloc[2], 1, a); }
		inline void step2(const double *a){ glUniform2dv(uniformloc[3], 1, a); }
		inline void S(const int &a){ glUniform1i(uniformloc[4], a); }
	END_SHADER();

	START_SHADER(Glow0, "S");
		inline void S(const int &a){ glUniform1i(uniformloc[0], a); }
	END_SHADER();

	START_SHADER(Glow1, "S");
		inline void S(const int &a){ glUniform1i(uniformloc[0], a); }
	END_SHADER();

	START_SHADER(Texture, "S", "s", "b");
		inline void S(const int &a){ glUniform1i(uniformloc[0], a); }
	END_SHADER();

	START_SHADER(VO, "Z", "tw", "zc", "sample", "r", "R");
		inline void Z(const int &a){ glUniform1i(uniformloc[0], a); }
		inline void tw(const float &a){ glUniform1f(uniformloc[1], a); }
		inline void zc(const double *a){ glUniform2dv(uniformloc[2], 1, a); }
		inline void sample(const unsigned &n, const float *a){ glUniform3fv(uniformloc[3], n, a); }
		inline void r(const float &a){ glUniform1f(uniformloc[4], a); }
		inline void R(const int &a){ glUniform1i(uniformloc[5], a); }
	END_SHADER();

	START_SHADER(VOBlur, "S", "t");
		inline void S(const int &a){ glUniform1i(uniformloc[0], a); }
		inline void t(const float &a){ glUniform1f(uniformloc[1], a); }
	END_SHADER();

	START_SHADER(VOScale, "A", "Z", "at", "zc");
		inline void A(const int &a){ glUniform1i(uniformloc[0], a); }
		inline void Z(const int &a){ glUniform1i(uniformloc[1], a); }
		inline void at(const float &a){ glUniform1f(uniformloc[2], a); }
		inline void zc(const double *a){ glUniform2dv(uniformloc[3], 1, a); }
	END_SHADER();

	START_SHADER(Directional, "direction", "color", "zc", "N", "Z", "hfov");
		inline void direction(const float *a){ glUniform3fv(uniformloc[0], 1, a); }
		inline void color(const float *a){ glUniform3fv(uniformloc[1], 1, a); }
		inline void color(const float &r, const float &g, const float &b){ glUniform3f(uniformloc[1], r, g, b); }
		inline void zc(const double *a){ glUniform2dv(uniformloc[2], 1, a); }
		inline void N(const int &a){ glUniform1i(uniformloc[3], a); }
		inline void Z(const int &a){ glUniform1i(uniformloc[4], a); }
		inline void hfov(const double *a){ glUniform2dv(uniformloc[5], 1, a); }
	END_SHADER();

	START_SHADER(Point, "position", "color", "zc", "N", "Z", "hfov");
		inline void position(const float *a){ glUniform3fv(uniformloc[0], 1, a); }
		inline void color(const float *a){ glUniform3fv(uniformloc[1], 1, a); }
		inline void color(const float &r, const float &g, const float &b){ glUniform3f(uniformloc[1], r, g, b); }
		inline void zc(const double *a){ glUniform2dv(uniformloc[2], 1, a); }
		inline void N(const int &a){ glUniform1i(uniformloc[3], a); }
		inline void Z(const int &a){ glUniform1i(uniformloc[4], a); }
		inline void hfov(const double *a){ glUniform2dv(uniformloc[5], 1, a); }
	END_SHADER();

	START_SHADER(Combine, "A", "D", "G");
		inline void A(const int &a){ glUniform1i(uniformloc[0], a); }
		inline void D(const int &a){ glUniform1i(uniformloc[1], a); }
		inline void G(const int &a){ glUniform1i(uniformloc[2], a); }
	END_SHADER();

	START_SHADER(FXAA, "S", "s");
		inline void S(const int &a){ glUniform1i(uniformloc[0], a); }
		inline void s(const double &a, const double &b){ glUniform2d(uniformloc[1], a, b); }
	END_SHADER();

	START_SHADER(Background, "b");
		inline void b(const float &r, const float &g, const float &b, const float &a){ glUniform4f(uniformloc[0], r, g, b, a); }
	END_SHADER();

	START_SHADER(AS1, "A", "Z", "zc", "h");
		inline void A(const int &a){ glUniform1i(uniformloc[0], a); }
		inline void Z(const int &a){ glUniform1i(uniformloc[1], a); }
		inline void zc(const double *a){ glUniform2dv(uniformloc[2], 1, a); }
		inline void h(const float &a){ glUniform1f(uniformloc[3], a); }
	END_SHADER();

	START_SHADER(AS2, "A", "h", "zc");
		inline void A(const int &a){ glUniform1i(uniformloc[0], a); }
		inline void h(const float &a){ glUniform1f(uniformloc[1], a); }
		inline void zc(const double *a){ glUniform2dv(uniformloc[2], 1, a); }
	END_SHADER();

	START_SHADER(HemiLight, "dir0", "col0", "dir1", "col1", "amb", "zc", "N", "Z", "hfov");
		inline void dir0(const float *a){ glUniform3fv(uniformloc[0], 1, a); }
		inline void col0(const float *a){ glUniform3fv(uniformloc[1], 1, a); }
		inline void col0(const float &r, const float &g, const float &b){ glUniform3f(uniformloc[1], r, g, b); }
		inline void dir1(const float *a){ glUniform3fv(uniformloc[2], 1, a); }
		inline void col1(const float *a){ glUniform3fv(uniformloc[3], 1, a); }
		inline void col1(const float &r, const float &g, const float &b){ glUniform3f(uniformloc[3], r, g, b); }
		inline void amb(const float *a){ glUniform3fv(uniformloc[4], 1, a); }
		inline void amb(const float &r, const float &g, const float &b){ glUniform3f(uniformloc[4], r, g, b); }
		inline void zc(const double *a){ glUniform2dv(uniformloc[5], 1, a); }
		inline void N(const int &a){ glUniform1i(uniformloc[6], a); }
		inline void Z(const int &a){ glUniform1i(uniformloc[7], a); }
		inline void hfov(const double *a){ glUniform2dv(uniformloc[8], 1, a); }
	END_SHADER();

	START_SHADER(AS2_HemiLight, "dir0", "col0", "dir1", "col1", "amb", "zc", "N", "Z", "hfov", "A");
		inline void dir0(const float *a){ glUniform3fv(uniformloc[0], 1, a); }
		inline void col0(const float *a){ glUniform3fv(uniformloc[1], 1, a); }
		inline void col0(const float &r, const float &g, const float &b){ glUniform3f(uniformloc[1], r, g, b); }
		inline void dir1(const float *a){ glUniform3fv(uniformloc[2], 1, a); }
		inline void col1(const float *a){ glUniform3fv(uniformloc[3], 1, a); }
		inline void col1(const float &r, const float &g, const float &b){ glUniform3f(uniformloc[3], r, g, b); }
		inline void amb(const float *a){ glUniform3fv(uniformloc[4], 1, a); }
		inline void amb(const float &r, const float &g, const float &b){ glUniform3f(uniformloc[4], r, g, b); }
		inline void zc(const double *a){ glUniform2dv(uniformloc[5], 1, a); }
		inline void N(const int &a){ glUniform1i(uniformloc[6], a); }
		inline void Z(const int &a){ glUniform1i(uniformloc[7], a); }
		inline void hfov(const double *a){ glUniform2dv(uniformloc[8], 1, a); }
		inline void A(const int &a){ glUniform1i(uniformloc[9], a); }
	END_SHADER();

	inline bool InitAll()
	{
		if(!InitShaders(shaders, shadertable, fragmentshaders, L(fragmentshaders), vertexshaders, L(vertexshaders), 0, 0))	//geometryshaders, L(geometryshaders)))
			return false;

		Shader::Object::InitUniforms();
		Shader::ObjectNM::InitUniforms();
		Shader::ObjectOSNM::InitUniforms();
		Shader::Solid::InitUniforms();
		Shader::Albedo::InitUniforms();
		Shader::Emissive::InitUniforms();
		Shader::Half::InitUniforms();
		Shader::Max::InitUniforms();
		Shader::DOF0::InitUniforms();
		Shader::DOF1::InitUniforms();
		Shader::Glow0::InitUniforms();
		Shader::Glow1::InitUniforms();
		Shader::Texture::InitUniforms();
		Shader::VO::InitUniforms();
		Shader::VOBlur::InitUniforms();
		Shader::VOScale::InitUniforms();
		Shader::Directional::InitUniforms();
		Shader::Point::InitUniforms();
		Shader::Combine::InitUniforms();
		Shader::FXAA::InitUniforms();
		Shader::Background::InitUniforms();
		Shader::AS1::InitUniforms();
		Shader::AS2::InitUniforms();
		Shader::HemiLight::InitUniforms();
		Shader::AS2_HemiLight::InitUniforms();

		return true;
	}
};
