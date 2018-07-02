#include "stdafx.h"
#include "Quad_mesh.h"

#include <vector>
#include "Utils.h"

void Load_quad_mesh_to_GPU(GL_Mesh& Quad_mesh)
{
	// Generate quad from two triangles:
	//
	// (-1;+1)   (0)   (+1;+1)
	//       V1-------V2
	//       |       / |
	//   (0) |    0    |
	//       | /       |
	//       V0-------V3
	// (-1;-1)         (+1;-1)

	std::vector<float> quad_vertexes{
		-1.0f,  // X0  V0
		-1.0f,  // Y0
		 0.0f,  // Z0

		-1.0f,  // X1  V1
		 1.0f,  // Y1
		 0.0f,  // Z1

		 1.0f,  // X2  V2
		 1.0f,  // Y2
		 0.0f,  // Z2

		 1.0f,  // X3  V3
		-1.0f,  // Y3
		 0.0f,  // Z3
	};

	std::vector<float> quad_tex_coords{
		0.0f,  // U0    V0
		0.0f,  // V0

		0.0f,  // U1    V1
		1.0f,  // V1

		1.0f,  // U2    V2
		1.0f,  // V2

		1.0f,  // U3    V3
		0.0f,  // V3
	};

	std::vector<GLushort> quad_indexes
	{
		0,1,2,  // Triangle 0
		0,2,3,  // Triangle 1
	};

	// Vertexes
	Quad_mesh.vertex_buffer_id = utils::gl_make_buffer(
		GL_ARRAY_BUFFER,
		&(quad_vertexes[0]),
		quad_vertexes.size() * sizeof(float)
	);

	Quad_mesh.vertex_count = static_cast<GLuint>(quad_vertexes.size() / 3); // Integer division

	// Texture coordinates
	Quad_mesh.tex_coord_buffer_id = utils::gl_make_buffer(
		GL_ARRAY_BUFFER,
		&(quad_tex_coords[0]),
		quad_tex_coords.size() * sizeof(float)
	);

	Quad_mesh.tex_coord_count = static_cast<GLuint>(quad_tex_coords.size() / 2); // Integer division

	// Indexes
	Quad_mesh.index_buffer_id = utils::gl_make_buffer(
		GL_ELEMENT_ARRAY_BUFFER,
		&(quad_indexes)[0],
		quad_indexes.size() * sizeof(GLushort)
	);

	Quad_mesh.index_count = static_cast<GLuint>(quad_indexes.size());
}