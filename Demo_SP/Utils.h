#pragma once

#include <windows.h>
#include <string>
#include <glew.h>

namespace utils
{
	// Constants
	const float PI = 3.14159265358979323846264338327950288f;

	void show_info_log(GLuint object, PFNGLGETSHADERIVPROC glGet__iv, PFNGLGETSHADERINFOLOGPROC glGet__InfoLog);
	bool LPWSTR2String(std::string& outString, const LPWSTR inLPWSTR, UINT codepage = CP_ACP);
	std::string Get_exe_path();

	// OpenGL helpers:
	GLuint gl_make_buffer(GLenum target, const void *buffer_data, size_t buffer_size);

	float RandomFloat(float min, float max);

	void PrintToOutputWindow(const char* format, va_list va);
	void Log(const char* format, ...);

	template<typename T>
	T sqr(T n)
	{
		return n * n;
	}
}