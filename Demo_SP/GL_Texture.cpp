#include "stdafx.h"

#include <stdlib.h>  // malloc, free, rand
#include <stdio.h>  // fprintf

#include "GL_Texture.h"
#include "TGA_loader.h"

GL_Texture::GL_Texture()
{
}

GL_Texture::~GL_Texture()
{
}

void GL_Texture::Make_texture(const char* filename, GLint min_filter, GLint mag_filter, GLint internalPixelFormat, GLenum externalPixelFormat, GLenum colorChannelType)
{
	void *pixels = read_tga(filename, &w, &h);

	if (pixels == nullptr)
	{
		return;
	}

	glGenTextures(1, &GPU_texture_id);

	if (GPU_texture_id == 0)
	{
		fprintf(stdout, "Failed to load texture into VRAM: %s\n", filename);
		free(pixels);
		pixels = nullptr;
		return;
	}

	glBindTexture(GL_TEXTURE_2D, GPU_texture_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(
		GL_TEXTURE_2D, 0,                       // target, level
		internalPixelFormat,                    // internal format
		w, h, 0,                                // width, height, border
		externalPixelFormat, colorChannelType,  // external format, type
		pixels                                  // pixels
	);

	//glTexImage2D(
	//	GL_TEXTURE_2D, 0,                       // target, level
	//	GL_RGB8,                                // internal format
	//	width, height, 0,                       // width, height, border
	//	GL_BGR, GL_UNSIGNED_BYTE,               // external format, type
	//	pixels                                  // pixels
	//);

	free(pixels);
	pixels = nullptr;
}

void GL_Texture::Delete_from_GPU()
{
	glDeleteTextures(1, &GPU_texture_id);
}