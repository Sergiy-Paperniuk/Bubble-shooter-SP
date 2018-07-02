#include "stdafx.h"
#include "Utils.h"

namespace utils
{
	void show_info_log( GLuint object, PFNGLGETSHADERIVPROC glGet__iv, PFNGLGETSHADERINFOLOGPROC glGet__InfoLog )
	{
		GLint log_length;
		char *log;

		glGet__iv(object, GL_INFO_LOG_LENGTH, &log_length);
		log = reinterpret_cast<char*>(malloc(static_cast<size_t>(log_length)));
		glGet__InfoLog(object, log_length, nullptr, log);

		if (log != nullptr)
		{
			fprintf(stdout, "%s", log);
			free(log);
		}
	}

	bool LPWSTR2String( std::string& outString, const LPWSTR inLPWSTR, UINT codepage )
	{
		bool retCode = false;
		char* temp = 0;
		size_t bytesRequired;
		bytesRequired = WideCharToMultiByte(codepage, 0, inLPWSTR, -1, 0, 0, 0, 0);

		if (bytesRequired > 0)
		{
			temp = new char[bytesRequired];
			int rc = WideCharToMultiByte(codepage, 0, inLPWSTR, -1, temp, static_cast<int>(bytesRequired), 0, 0);

			if (rc != 0)
			{
				temp[bytesRequired - 1] = 0;
				outString = temp;
				retCode = true;
			}
		}

		delete[] temp;
		return retCode;
	}

	std::string Get_exe_path()
	{
		wchar_t buffer[MAX_PATH];
		GetModuleFileName(nullptr, buffer, MAX_PATH);

		std::string file_path;
		LPWSTR2String(file_path, buffer);

		std::string::size_type pos = file_path.find_last_of("\\/");

		return std::string(file_path).substr(0, pos);
	}

	GLuint gl_make_buffer(GLenum target, const void *buffer_data, size_t buffer_size)
	{
		GLuint buffer;
		glGenBuffers(1, &buffer);
		glBindBuffer(target, buffer);
		glBufferData(target, buffer_size, buffer_data, GL_STATIC_DRAW);

		return buffer;
	}

	float RandomFloat(float min, float max)
	{
		float random = ((float)rand()) / (float)RAND_MAX;
		float diff = max - min;
		float r = random * diff;

		return min + r;
	}

	void PrintToOutputWindow(const char* format, va_list va)
	{
		const size_t N = 1024;
		char  buf[N];

		int bytes_used = vsprintf_s(buf, N, format, va);

		OutputDebugStringA(buf);
	}

	void Log(const char* format, ...)
	{
		va_list va;
		__crt_va_start(va, format);

		PrintToOutputWindow(format, va);

		__crt_va_end(va);
	}
}