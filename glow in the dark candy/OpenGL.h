#pragma once

#pragma comment(lib, "opengl32.lib")


#include <windows.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/wglext.h>

#ifdef _DEBUG
#include "vectormath_scalar\vectormath_aos.h"
#else
#include "vectormath\vectormath_aos.h"
#endif
using namespace Vectormath::Aos;

bool InitWGLExtensions();
bool InitOpenGLExtensions();
//unsigned CreateShader(const char* filepath, GLenum type);
//bool CompileProgram(unsigned &prog, const char* attriblist, const char* outlist);
unsigned CreateProgram(const char* attriblist, const char* outlist, const GLenum &buffermode, unsigned n, ...);
//unsigned CreateProgramTF(const char* attriblist, GLsizei count, const char **varyings, GLenum bufferMode, unsigned n, ...);
void UniformTexture(int ul, unsigned unit, unsigned texture, GLenum target = GL_TEXTURE_2D);
bool InitShaders(unsigned *shaders, const unsigned *shadertable, const char **fragmentshaders, const unsigned &nfragmentshaders, const char **vertexshaders, const unsigned &nvertexshaders, const char **geometryshaders, const unsigned &ngeometryshaders);


extern PFNWGLSWAPINTERVALEXTPROC			wglSwapIntervalEXT;
extern PFNWGLCHOOSEPIXELFORMATARBPROC		wglChoosePixelFormatARB;
extern PFNWGLCREATECONTEXTATTRIBSARBPROC	wglCreateContextAttribsARB;
extern PFNGLACTIVETEXTUREPROC				glActiveTexture;
extern PFNGLMULTIDRAWARRAYSPROC				glMultiDrawArrays;
extern PFNGLMULTIDRAWELEMENTSPROC			glMultiDrawElements;
extern PFNGLBINDBUFFERPROC					glBindBuffer;
extern PFNGLDELETEBUFFERSPROC				glDeleteBuffers;
extern PFNGLGENBUFFERSPROC					glGenBuffers;
extern PFNGLBUFFERDATAPROC					glBufferData;
extern PFNGLBUFFERSUBDATAPROC				glBufferSubData;
extern PFNGLBLENDEQUATIONSEPARATEPROC		glBlendEquationSeparate;
extern PFNGLATTACHSHADERPROC				glAttachShader;
extern PFNGLCOMPILESHADERPROC				glCompileShader;
extern PFNGLCREATEPROGRAMPROC				glCreateProgram;
extern PFNGLCREATESHADERPROC				glCreateShader;
extern PFNGLDELETEPROGRAMPROC				glDeleteProgram;
extern PFNGLDELETESHADERPROC				glDeleteShader;
extern PFNGLGETPROGRAMIVPROC				glGetProgramiv;
extern PFNGLGETSHADERIVPROC					glGetShaderiv;
extern PFNGLGETUNIFORMLOCATIONPROC			glGetUniformLocation;
extern PFNGLLINKPROGRAMPROC					glLinkProgram;
extern PFNGLSHADERSOURCEPROC				glShaderSource;
extern PFNGLUSEPROGRAMPROC					glUseProgram;
extern PFNGLBLENDCOLORPROC					glBlendColor;
extern PFNGLUNIFORM1FPROC					glUniform1f;
extern PFNGLUNIFORM2FPROC					glUniform2f;
extern PFNGLUNIFORM2DPROC					glUniform2d;
extern PFNGLUNIFORM3FPROC					glUniform3f;
extern PFNGLUNIFORM4FPROC					glUniform4f;
extern PFNGLUNIFORM1IPROC					glUniform1i;
extern PFNGLUNIFORM1UIPROC					glUniform1ui;
extern PFNGLUNIFORM1FVPROC					glUniform1fv;
extern PFNGLUNIFORM2FVPROC					glUniform2fv;
extern PFNGLUNIFORM2DVPROC					glUniform2dv;
extern PFNGLUNIFORM3FVPROC					glUniform3fv;
extern PFNGLUNIFORM4FVPROC					glUniform4fv;
extern PFNGLUNIFORMMATRIX4FVPROC			glUniformMatrix4fv;
extern PFNGLGENFRAMEBUFFERSPROC				glGenFramebuffers;
extern PFNGLBINDFRAMEBUFFERPROC				glBindFramebuffer;
extern PFNGLBINDRENDERBUFFERPROC			glBindRenderbuffer;
extern PFNGLGENRENDERBUFFERSPROC			glGenRenderbuffers;
extern PFNGLRENDERBUFFERSTORAGEPROC			glRenderbufferStorage;
extern PFNGLFRAMEBUFFERTEXTUREPROC			glFramebufferTexture;
extern PFNGLFRAMEBUFFERTEXTURE2DPROC		glFramebufferTexture2D;
extern PFNGLDRAWBUFFERSPROC					glDrawBuffers;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC		glEnableVertexAttribArray;
extern PFNGLDISABLEVERTEXATTRIBARRAYPROC	glDisableVertexAttribArray;
extern PFNGLVERTEXATTRIBPOINTERPROC			glVertexAttribPointer;
extern PFNGLVERTEXATTRIBIPOINTERPROC		glVertexAttribIPointer;
extern PFNGLBINDATTRIBLOCATIONPROC			glBindAttribLocation;
extern PFNGLCHECKFRAMEBUFFERSTATUSPROC		glCheckFramebufferStatus;
extern PFNGLGETSHADERINFOLOGPROC			glGetShaderInfoLog;
extern PFNGLGETPROGRAMINFOLOGPROC			glGetProgramInfoLog;
extern PFNGLBINDVERTEXARRAYPROC				glBindVertexArray;
extern PFNGLDELETEVERTEXARRAYSPROC			glDeleteVertexArrays;
extern PFNGLGENVERTEXARRAYSPROC				glGenVertexArrays;
extern PFNGLMAPBUFFERPROC					glMapBuffer;
extern PFNGLUNMAPBUFFERPROC					glUnmapBuffer;
extern PFNGLBLENDFUNCSEPARATEPROC			glBlendFuncSeparate;
extern PFNGLGETATTRIBLOCATIONPROC			glGetAttribLocation;
extern PFNGLGENERATEMIPMAPPROC				glGenerateMipmap;
extern PFNGLENABLEIPROC						glEnablei;
extern PFNGLDISABLEIPROC					glDisablei;
extern PFNGLBEGINTRANSFORMFEEDBACKPROC		glBeginTransformFeedback;
extern PFNGLENDTRANSFORMFEEDBACKPROC		glEndTransformFeedback;
extern PFNGLTRANSFORMFEEDBACKVARYINGSPROC	glTransformFeedbackVaryings;
extern PFNGLFRAMEBUFFERRENDERBUFFERPROC		glFramebufferRenderbuffer;
extern PFNGLBINDFRAGDATALOCATIONPROC		glBindFragDataLocation;
extern PFNGLBINDBUFFERBASEPROC				glBindBufferBase;
extern PFNGLBEGINQUERYPROC					glBeginQuery;
extern PFNGLENDQUERYPROC					glEndQuery;
extern PFNGLGETTRANSFORMFEEDBACKVARYINGPROC	glGetTransformFeedbackVarying;
extern PFNGLGETQUERYOBJECTUIVPROC			glGetQueryObjectuiv;
extern PFNGLGENQUERIESPROC					glGenQueries;
extern PFNGLBINDBUFFERRANGEPROC				glBindBufferRange;
extern PFNGLDELETEFRAMEBUFFERSPROC			glDeleteFramebuffers;
extern PFNGLTEXBUFFERPROC					glTexBuffer;
extern PFNGLBLITFRAMEBUFFERPROC				glBlitFramebuffer;
