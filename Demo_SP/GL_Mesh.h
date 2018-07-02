#pragma once

#include <glew.h>

class GL_Mesh
{
public:
	// Geometry (GPU)
	GLuint vertex_buffer_id = 0;
	GLuint vertex_count = 0;

	GLuint tex_coord_buffer_id = 0;
	GLuint tex_coord_count = 0;

	GLuint index_buffer_id = 0;
	GLuint index_count = 0;

	void Delete_from_GPU();

	GL_Mesh();
	~GL_Mesh();
};