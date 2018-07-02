#pragma once

#include <glew.h>

class GLSL_Shader
{
public:
	GLSL_Shader();
	~GLSL_Shader();
};

GLuint make_shader(GLenum type, const char *filename);
GLuint make_program(GLuint vertex_shader, GLuint fragment_shader);
