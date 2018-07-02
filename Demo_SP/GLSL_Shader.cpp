#include "stdafx.h"
#include "GLSL_Shader.h"

#include "Utils.h"
#include "File_IO.h"

GLSL_Shader::GLSL_Shader()
{
}

GLSL_Shader::~GLSL_Shader()
{
}

GLuint make_shader(GLenum type, const char *filename)
{
	size_t length_size_t;
	GLchar *source = reinterpret_cast<GLchar*>(get_file_contents(filename, &length_size_t));

	GLint length = static_cast<GLint>(length_size_t);
	GLuint shader;
	GLint shader_ok;

	if (!source)
	{
		return 0;
	}

	shader = glCreateShader(type);
	glShaderSource(shader, 1, (const GLchar**)&source, &length);
	free(source);
	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &shader_ok);
	if (!shader_ok)
	{
		fprintf(stdout, "Failed to compile %s:\n", filename);
		utils::show_info_log(shader, glGetShaderiv, glGetShaderInfoLog);
		glDeleteShader(shader);
		return 0;
	}

	return shader;
}

GLuint make_program(GLuint vertex_shader, GLuint fragment_shader)
{
	GLint program_ok;

	GLuint program = glCreateProgram();

	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &program_ok);
	if (!program_ok)
	{
		fprintf(stdout, "Failed to link shader program:\n");
		utils::show_info_log(program, glGetProgramiv, glGetProgramInfoLog);
		glDeleteProgram(program);
		return 0;
	}

	return program;
}