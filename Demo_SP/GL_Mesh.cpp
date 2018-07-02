#include "stdafx.h"
#include "GL_Mesh.h"
#include "glew.h"

GL_Mesh::GL_Mesh()
{
}

GL_Mesh::~GL_Mesh()
{
}

void GL_Mesh::Delete_from_GPU()
{
	glDeleteBuffers(1, &index_buffer_id);
	glDeleteBuffers(1, &tex_coord_buffer_id);
	glDeleteBuffers(1, &vertex_buffer_id);
}