#pragma once

#include <glew.h>

class GL_Texture
{
public:
	GL_Texture();
	~GL_Texture();

	GLuint GPU_texture_id = 0;
	int w = 0;
	int h = 0;

	void Make_texture(const char* filename, GLint min_filter, GLint mag_filter, GLint internalPixelFormat, GLenum externalPixelFormat, GLenum colorChannelType);
	void Delete_from_GPU();
};