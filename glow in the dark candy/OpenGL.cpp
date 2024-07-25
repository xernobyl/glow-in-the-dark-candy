#include "OpenGL.h"
#include "Window.h"
#include <stdio.h>

#define _INITEXT_(TYPE,NAME) NAME = (TYPE)wglGetProcAddress(#NAME); if(!NAME) return false;

PFNWGLSWAPINTERVALEXTPROC				wglSwapIntervalEXT;
PFNWGLCHOOSEPIXELFORMATARBPROC			wglChoosePixelFormatARB;
PFNWGLCREATECONTEXTATTRIBSARBPROC		wglCreateContextAttribsARB;
PFNGLACTIVETEXTUREPROC					glActiveTexture;
PFNGLMULTIDRAWARRAYSPROC				glMultiDrawArrays;
PFNGLMULTIDRAWELEMENTSPROC				glMultiDrawElements;
PFNGLBINDBUFFERPROC						glBindBuffer;
PFNGLDELETEBUFFERSPROC					glDeleteBuffers;
PFNGLGENBUFFERSPROC						glGenBuffers;
PFNGLBUFFERDATAPROC						glBufferData;
PFNGLBUFFERSUBDATAPROC					glBufferSubData;
PFNGLBLENDEQUATIONSEPARATEPROC			glBlendEquationSeparate;
PFNGLATTACHSHADERPROC					glAttachShader;
PFNGLCOMPILESHADERPROC					glCompileShader;
PFNGLCREATEPROGRAMPROC					glCreateProgram;
PFNGLCREATESHADERPROC					glCreateShader;
PFNGLDELETEPROGRAMPROC					glDeleteProgram;
PFNGLDELETESHADERPROC					glDeleteShader;
PFNGLGETPROGRAMIVPROC					glGetProgramiv;
PFNGLGETSHADERIVPROC					glGetShaderiv;
PFNGLGETUNIFORMLOCATIONPROC				glGetUniformLocation;
PFNGLLINKPROGRAMPROC					glLinkProgram;
PFNGLSHADERSOURCEPROC					glShaderSource;
PFNGLUSEPROGRAMPROC						glUseProgram;
PFNGLBLENDCOLORPROC						glBlendColor;
PFNGLUNIFORM1FPROC						glUniform1f;
PFNGLUNIFORM2FPROC						glUniform2f;
PFNGLUNIFORM2DPROC						glUniform2d;
PFNGLUNIFORM3FPROC						glUniform3f;
PFNGLUNIFORM4FPROC						glUniform4f;
PFNGLUNIFORM1IPROC						glUniform1i;
PFNGLUNIFORM1UIPROC						glUniform1ui;
PFNGLUNIFORM1FVPROC						glUniform1fv;
PFNGLUNIFORM2FVPROC						glUniform2fv;
PFNGLUNIFORM2DVPROC						glUniform2dv;
PFNGLUNIFORM3FVPROC						glUniform3fv;
PFNGLUNIFORM4FVPROC						glUniform4fv;
PFNGLUNIFORMMATRIX4FVPROC				glUniformMatrix4fv;
PFNGLGENFRAMEBUFFERSPROC				glGenFramebuffers;
PFNGLBINDFRAMEBUFFERPROC				glBindFramebuffer;
PFNGLBINDRENDERBUFFERPROC				glBindRenderbuffer;
PFNGLGENRENDERBUFFERSPROC				glGenRenderbuffers;
PFNGLRENDERBUFFERSTORAGEPROC			glRenderbufferStorage;
PFNGLFRAMEBUFFERTEXTUREPROC				glFramebufferTexture;
PFNGLFRAMEBUFFERTEXTURE2DPROC			glFramebufferTexture2D;
PFNGLDRAWBUFFERSPROC					glDrawBuffers;
PFNGLENABLEVERTEXATTRIBARRAYPROC		glEnableVertexAttribArray;
PFNGLDISABLEVERTEXATTRIBARRAYPROC		glDisableVertexAttribArray;
PFNGLVERTEXATTRIBPOINTERPROC			glVertexAttribPointer;
PFNGLVERTEXATTRIBIPOINTERPROC			glVertexAttribIPointer;
PFNGLBINDATTRIBLOCATIONPROC				glBindAttribLocation;
PFNGLCHECKFRAMEBUFFERSTATUSPROC			glCheckFramebufferStatus;
PFNGLGETSHADERINFOLOGPROC				glGetShaderInfoLog;
PFNGLGETPROGRAMINFOLOGPROC				glGetProgramInfoLog;
PFNGLBINDVERTEXARRAYPROC				glBindVertexArray;
PFNGLDELETEVERTEXARRAYSPROC				glDeleteVertexArrays;
PFNGLGENVERTEXARRAYSPROC				glGenVertexArrays;
PFNGLMAPBUFFERPROC						glMapBuffer;
PFNGLUNMAPBUFFERPROC					glUnmapBuffer;
PFNGLBLENDFUNCSEPARATEPROC				glBlendFuncSeparate;
PFNGLGETATTRIBLOCATIONPROC				glGetAttribLocation;
PFNGLGENERATEMIPMAPPROC					glGenerateMipmap;
PFNGLENABLEIPROC						glEnablei;
PFNGLDISABLEIPROC						glDisablei;
PFNGLBEGINTRANSFORMFEEDBACKPROC			glBeginTransformFeedback;
PFNGLENDTRANSFORMFEEDBACKPROC			glEndTransformFeedback;
PFNGLTRANSFORMFEEDBACKVARYINGSPROC		glTransformFeedbackVaryings;
PFNGLFRAMEBUFFERRENDERBUFFERPROC		glFramebufferRenderbuffer;
PFNGLBINDFRAGDATALOCATIONPROC			glBindFragDataLocation;
PFNGLBINDBUFFERBASEPROC					glBindBufferBase;
PFNGLBEGINQUERYPROC						glBeginQuery;
PFNGLENDQUERYPROC						glEndQuery;
PFNGLGETTRANSFORMFEEDBACKVARYINGPROC	glGetTransformFeedbackVarying;
PFNGLGETQUERYOBJECTUIVPROC				glGetQueryObjectuiv;
PFNGLGENQUERIESPROC						glGenQueries;
PFNGLBINDBUFFERRANGEPROC				glBindBufferRange;
PFNGLDELETEFRAMEBUFFERSPROC				glDeleteFramebuffers;
PFNGLTEXBUFFERPROC						glTexBuffer;
PFNGLBLITFRAMEBUFFERPROC				glBlitFramebuffer;


bool InitWGLExtensions()
{
	_INITEXT_(PFNWGLCHOOSEPIXELFORMATARBPROC,		wglChoosePixelFormatARB);
	_INITEXT_(PFNWGLCREATECONTEXTATTRIBSARBPROC,	wglCreateContextAttribsARB);

	return true;
}


bool InitOpenGLExtensions()
{	
	_INITEXT_(PFNWGLSWAPINTERVALEXTPROC,			wglSwapIntervalEXT);
	_INITEXT_(PFNGLACTIVETEXTUREPROC,				glActiveTexture);
	_INITEXT_(PFNGLMULTIDRAWARRAYSPROC,				glMultiDrawArrays);
	_INITEXT_(PFNGLMULTIDRAWELEMENTSPROC,			glMultiDrawElements);
	_INITEXT_(PFNGLBINDBUFFERPROC,					glBindBuffer);
	_INITEXT_(PFNGLDELETEBUFFERSPROC,				glDeleteBuffers);
	_INITEXT_(PFNGLGENBUFFERSPROC,					glGenBuffers);
	_INITEXT_(PFNGLBUFFERDATAPROC,					glBufferData);
	_INITEXT_(PFNGLBUFFERSUBDATAPROC,				glBufferSubData);
	_INITEXT_(PFNGLBLENDEQUATIONSEPARATEPROC,		glBlendEquationSeparate);
	_INITEXT_(PFNGLATTACHSHADERPROC,				glAttachShader);
	_INITEXT_(PFNGLCOMPILESHADERPROC,				glCompileShader);
	_INITEXT_(PFNGLCREATEPROGRAMPROC,				glCreateProgram);
	_INITEXT_(PFNGLCREATESHADERPROC,				glCreateShader);
	_INITEXT_(PFNGLDELETEPROGRAMPROC,				glDeleteProgram);
	_INITEXT_(PFNGLDELETESHADERPROC,				glDeleteShader);
	_INITEXT_(PFNGLGETPROGRAMIVPROC,				glGetProgramiv);
	_INITEXT_(PFNGLGETSHADERIVPROC,					glGetShaderiv);
	_INITEXT_(PFNGLGETUNIFORMLOCATIONPROC,			glGetUniformLocation);
	_INITEXT_(PFNGLLINKPROGRAMPROC,					glLinkProgram);
	_INITEXT_(PFNGLSHADERSOURCEPROC,				glShaderSource);
	_INITEXT_(PFNGLUSEPROGRAMPROC,					glUseProgram);
	_INITEXT_(PFNGLBLENDCOLORPROC,					glBlendColor);
	_INITEXT_(PFNGLUNIFORM1FPROC,					glUniform1f);
	_INITEXT_(PFNGLUNIFORM2FPROC,					glUniform2f);
	_INITEXT_(PFNGLUNIFORM2DPROC,					glUniform2d);
	_INITEXT_(PFNGLUNIFORM3FPROC,					glUniform3f);
	_INITEXT_(PFNGLUNIFORM4FPROC,					glUniform4f);
	_INITEXT_(PFNGLUNIFORM1IPROC,					glUniform1i);
	_INITEXT_(PFNGLUNIFORM1UIPROC,					glUniform1ui);
	_INITEXT_(PFNGLUNIFORM1FVPROC,					glUniform1fv);
	_INITEXT_(PFNGLUNIFORM2FVPROC,					glUniform2fv);
	_INITEXT_(PFNGLUNIFORM2DVPROC,					glUniform2dv);
	_INITEXT_(PFNGLUNIFORM3FVPROC,					glUniform3fv);
	_INITEXT_(PFNGLUNIFORM4FVPROC,					glUniform4fv);
	_INITEXT_(PFNGLUNIFORMMATRIX4FVPROC,			glUniformMatrix4fv);
	_INITEXT_(PFNGLGENFRAMEBUFFERSPROC,				glGenFramebuffers);
	_INITEXT_(PFNGLBINDFRAMEBUFFERPROC,				glBindFramebuffer);
	_INITEXT_(PFNGLBINDRENDERBUFFERPROC,			glBindRenderbuffer);
	_INITEXT_(PFNGLGENRENDERBUFFERSPROC,			glGenRenderbuffers);
	_INITEXT_(PFNGLRENDERBUFFERSTORAGEPROC,			glRenderbufferStorage);
	_INITEXT_(PFNGLFRAMEBUFFERTEXTUREPROC,			glFramebufferTexture);
	_INITEXT_(PFNGLFRAMEBUFFERTEXTURE2DPROC,		glFramebufferTexture2D);
	_INITEXT_(PFNGLDRAWBUFFERSPROC,					glDrawBuffers);
	_INITEXT_(PFNGLENABLEVERTEXATTRIBARRAYPROC,		glEnableVertexAttribArray);
	_INITEXT_(PFNGLDISABLEVERTEXATTRIBARRAYPROC,	glDisableVertexAttribArray);
	_INITEXT_(PFNGLVERTEXATTRIBPOINTERPROC,			glVertexAttribPointer);
	_INITEXT_(PFNGLVERTEXATTRIBIPOINTERPROC,		glVertexAttribIPointer);
	_INITEXT_(PFNGLBINDATTRIBLOCATIONPROC,			glBindAttribLocation);
	_INITEXT_(PFNGLCHECKFRAMEBUFFERSTATUSPROC,		glCheckFramebufferStatus);
	_INITEXT_(PFNGLGETSHADERINFOLOGPROC,			glGetShaderInfoLog);
	_INITEXT_(PFNGLGETPROGRAMINFOLOGPROC,			glGetProgramInfoLog);
	_INITEXT_(PFNGLBINDVERTEXARRAYPROC,				glBindVertexArray);
	_INITEXT_(PFNGLDELETEVERTEXARRAYSPROC,			glDeleteVertexArrays);
	_INITEXT_(PFNGLGENVERTEXARRAYSPROC,				glGenVertexArrays);
	_INITEXT_(PFNGLMAPBUFFERPROC,					glMapBuffer);
	_INITEXT_(PFNGLUNMAPBUFFERPROC,					glUnmapBuffer);
	_INITEXT_(PFNGLBLENDFUNCSEPARATEPROC,			glBlendFuncSeparate);
	_INITEXT_(PFNGLGETATTRIBLOCATIONPROC,			glGetAttribLocation);
	_INITEXT_(PFNGLGENERATEMIPMAPPROC,				glGenerateMipmap);
	_INITEXT_(PFNGLENABLEIPROC,						glEnablei);
	_INITEXT_(PFNGLDISABLEIPROC,					glDisablei);
	_INITEXT_(PFNGLBEGINTRANSFORMFEEDBACKPROC,		glBeginTransformFeedback);
	_INITEXT_(PFNGLENDTRANSFORMFEEDBACKPROC,		glEndTransformFeedback);
	_INITEXT_(PFNGLTRANSFORMFEEDBACKVARYINGSPROC,	glTransformFeedbackVaryings);
	_INITEXT_(PFNGLFRAMEBUFFERRENDERBUFFERPROC,		glFramebufferRenderbuffer);
	_INITEXT_(PFNGLBINDFRAGDATALOCATIONPROC,		glBindFragDataLocation);
	_INITEXT_(PFNGLBINDBUFFERBASEPROC,				glBindBufferBase);
	_INITEXT_(PFNGLBEGINQUERYPROC,					glBeginQuery);
	_INITEXT_(PFNGLENDQUERYPROC,					glEndQuery);
	_INITEXT_(PFNGLGETTRANSFORMFEEDBACKVARYINGPROC,	glGetTransformFeedbackVarying);
	_INITEXT_(PFNGLGETQUERYOBJECTUIVPROC,			glGetQueryObjectuiv);
	_INITEXT_(PFNGLGENQUERIESPROC,					glGenQueries);
	_INITEXT_(PFNGLBINDBUFFERRANGEPROC,				glBindBufferRange);
	_INITEXT_(PFNGLDELETEFRAMEBUFFERSPROC,			glDeleteFramebuffers);
	_INITEXT_(PFNGLTEXBUFFERPROC,					glTexBuffer);
	_INITEXT_(PFNGLBLITFRAMEBUFFERPROC,				glBlitFramebuffer);

	return true;
}


static const char defattriblist[] =
{
	'A', '0', '\0', 0,
	'A', '1', '\0', 1,
	'A', '2', '\0', 2,
	'A', '3', '\0', 3,
	'A', '4', '\0', 4,
	0
};

static const char defoutlist[] =
{
	'O', '0', '\0', 0,
	'O', '1', '\0', 1,
	'O', '2', '\0', 2,
	'O', '3', '\0', 3,
	0
};


unsigned CreateProgram(const char* attriblist, const char* outlist, const GLenum &buffermode, unsigned n, ...)
{
	va_list ap;
	int result;

	unsigned sp = glCreateProgram();

	va_start(ap, n);
	for(unsigned i=0; i<n; ++i)
		glAttachShader(sp, va_arg(ap, unsigned));
	va_end(ap);

	if(!attriblist)
		attriblist = defattriblist;

	while(*attriblist)
	{
		const char *attrib = attriblist;
		attriblist += strlen(attrib)+1;
		unsigned nattrib = (unsigned)*attriblist;

		glBindAttribLocation(sp, nattrib, attrib);

		++attriblist;
	}

	if(buffermode)
	{
		unsigned len = strlen(outlist);
		char *s = new char[len+1];
		strcpy_s(s, len+1, outlist);
		unsigned count = 0;
		const char **varyings = new const char*[8];
		varyings[count++] = (char*)s;

		for(unsigned i=0; i<len; ++i)
			if(s[i] == ' ')
			{
				s[i++] = '\0';
				varyings[count++] = (char*)s+i;
			}

		glTransformFeedbackVaryings(sp, count, varyings, buffermode);

		delete [] s;
		delete [] varyings;

		for(unsigned i=0; i<count; ++i)
			delete [] varyings[i];

		delete [] varyings;
	}
	else if(!outlist)
		outlist = defoutlist;

	while(*outlist)
	{
		const char *name = outlist;
		outlist += strlen(name)+1;
		unsigned nout = (unsigned)*outlist;
		glBindFragDataLocation(sp, nout, name);
		++outlist;
	}

	glLinkProgram(sp);
	glGetProgramiv(sp, GL_LINK_STATUS, &result);

	if(result != GL_TRUE)
	{
		glGetProgramiv(sp, GL_INFO_LOG_LENGTH, &result);
		char* tmp = new char[result];
		glGetProgramInfoLog(sp, result, 0, tmp);
		MessageBoxA(0, tmp, "Program Error", 0);
		PostQuitMessage(0);
		return 0;
	}

	return sp;
}


inline bool CompileProgram(unsigned &sp, const char* attriblist, const char* outlist)
{
	int result;

	if(!attriblist)
		attriblist = defattriblist;

	while(*attriblist)
	{
		const char *attrib = attriblist;
		attriblist += strlen(attrib)+1;
		unsigned nattrib = (unsigned)*attriblist;

		glBindAttribLocation(sp, nattrib, attrib);

		++attriblist;
	}

	if(!outlist)
		outlist = defoutlist;

	while(*outlist)
	{
		const char *name = outlist;
		outlist += strlen(name)+1;
		unsigned nout = (unsigned)*outlist;
		glBindFragDataLocation(sp, nout, name);
		++outlist;
	}

	glLinkProgram(sp);
	glGetProgramiv(sp, GL_LINK_STATUS, &result);

	if(result != GL_TRUE)
	{
		glGetProgramiv(sp, GL_INFO_LOG_LENGTH, &result);
		char* tmp = new char[result];
		glGetProgramInfoLog(sp, result, 0, tmp);
		MessageBoxA(0, tmp, "Program Error", 0);
		PostQuitMessage(0);
		return 0;
	}

	return true;
}


/*unsigned CreateProgramTF(const char* attriblist, GLsizei count, const char **varyings, GLenum bufferMode, unsigned n, ...)
{
	va_list ap;
	int result;

	unsigned sp = glCreateProgram();

	va_start(ap, n);
	for(unsigned i=0; i<n; ++i)
		glAttachShader(sp, va_arg(ap, unsigned));
	va_end(ap);

	if(attriblist)
	{
		while(*attriblist)
		{
			const char *attrib = attriblist;
			attriblist += strlen(attrib)+1;
			unsigned nattrib = (unsigned)*attriblist;

			glBindAttribLocation(sp, nattrib, attrib);

			++attriblist;
		}
	}

	glTransformFeedbackVaryings(sp, count, varyings, bufferMode);

	glLinkProgram(sp);
	glGetProgramiv(sp, GL_LINK_STATUS, &result);

	if(result != GL_TRUE)
	{
		glGetProgramiv(sp, GL_INFO_LOG_LENGTH, &result);
		char* tmp = new char[result];
		glGetProgramInfoLog(sp, result, 0, tmp);
		MessageBoxA(0, tmp, "Program Error", 0);
		PostQuitMessage(0);
		return 0;
	}

	return sp;
}*/


unsigned CreateShader(const char* filepath, GLenum type)
{
	int result, shaderfilelen;

	char* shaderfile = (char*)Window::LoadFile(filepath, &shaderfilelen);
	if(!shaderfile)
	{
		MessageBoxA(0, "Could not open", filepath, 0);
		PostQuitMessage(0);
		return 0;
	}

	unsigned shader = glCreateShader(type);
	glShaderSource(shader, 1, (const GLchar**)&shaderfile, &shaderfilelen);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);

	if(result != GL_TRUE)
	{
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &result);
		char* tmp = new char[result];
		glGetShaderInfoLog(shader, result, 0, tmp);
		MessageBoxA(0, tmp, filepath, 0);
		return 0;
	}

	delete [] shaderfile;

	return shader;
}


bool InitShaders(unsigned *shaders, const unsigned *shadertable, const char **fragmentshaders, const unsigned &nfragmentshaders, const char **vertexshaders, const unsigned &nvertexshaders, const char **geometryshaders, const unsigned &ngeometryshaders)
{
	unsigned *fs = new unsigned[nfragmentshaders];
	unsigned *vs = new unsigned[nvertexshaders];
	unsigned *gs = new unsigned[ngeometryshaders];

	char tmp[32];

	for(unsigned i=0; i<nfragmentshaders; ++i)
	{
		strcpy_s(tmp, 32, "shaders/");
		strcat_s(tmp, 32, fragmentshaders[i]);
		strcat_s(tmp, 32, ".fs");

		if(!(fs[i] = CreateShader(tmp, GL_FRAGMENT_SHADER)))
			return false;
	}

	for(unsigned i=0; i<nvertexshaders; ++i)
	{
		strcpy_s(tmp, 32, "shaders/");
		strcat_s(tmp, 32, vertexshaders[i]);
		strcat_s(tmp, 32, ".vs");

		if(!(vs[i] = CreateShader(tmp, GL_VERTEX_SHADER)))
			return false;
	}

	for(unsigned i=0; i<ngeometryshaders; ++i)
	{
		strcpy_s(tmp, 32, "shaders/");
		strcat_s(tmp, 32, geometryshaders[i]);
		strcat_s(tmp, 32, ".gs");

		if(!(gs[i] = CreateShader(tmp, GL_GEOMETRY_SHADER)))
			return false;
	}

	while(unsigned nshaders = *shadertable++)
	{
		bool transformfeedback = true;

		*shaders = glCreateProgram();

		for(unsigned i=0; i<nshaders; ++i)
		{
			unsigned t = *shadertable++;
			unsigned s = t & 0x3fff;

			if(t & 0x8000)
				s = gs[s];
			else if(t & 0x4000)
				s = vs[s];
			else
			{
				s = fs[s];
				transformfeedback = false;
			}

			glAttachShader(*shaders, s);
		}

		if(transformfeedback)
		{
			unsigned count = *shadertable++;
			const char **varyings = (const char**)shadertable;
			shadertable += count * sizeof(const char *)/sizeof(unsigned);
			GLenum buffermode = *shadertable++;
			glTransformFeedbackVaryings(*shaders, count, varyings, buffermode);
		}

		if(!CompileProgram(*shaders, 0, 0))
			return false;

		++shaders;
	}

	for(unsigned i=0; i<nfragmentshaders; ++i)
		glDeleteShader(fs[i]);

	for(unsigned i=0; i<nvertexshaders; ++i)
		glDeleteShader(vs[i]);

	for(unsigned i=0; i<ngeometryshaders; ++i)
		glDeleteShader(gs[i]);

	delete [] fs;
	delete [] vs;
	delete [] gs;

	return true;
}
