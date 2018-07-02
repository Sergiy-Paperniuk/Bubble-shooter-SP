#include "stdafx.h"
#include "GL_Render.h"

#include <glut.h>
#include <algorithm>  // std::max

#include "glm/gtc/matrix_transform.hpp"  // glm::translate, glm::rotate, glm::scale, glm::perspective
#include "Global.h"

extern Global* G;

void GL_Render::Draw_quad(const GL_Texture& gl_texture, const float dx, const float dy, const glm::mat4& quad_mat /* = glm::mat4(1.0f) */) const
{
	glm::mat4 final_quad_mat = glm::translate(glm::mat4(1.0f), glm::vec3(dx, dy, 0.0f));
	final_quad_mat = glm::scale(final_quad_mat, glm::vec3(gl_texture.w * 0.5f, gl_texture.h * 0.5f, 1.0f));  // Because quad width and height == 2.0f
	final_quad_mat = inv_opengl_to_game_coordinates_matrix * quad_mat * final_quad_mat;

	glUniformMatrix4fv(G->RC.uniforms.proj_matrix_id, 1, GL_FALSE, &final_quad_mat[0][0]);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gl_texture.GPU_texture_id);
	glUniform1i(G->RC.uniforms.texture_ids[0], 0);

	// For textures blending
	// glActiveTexture(GL_TEXTURE1);
	// glBindTexture(GL_TEXTURE_2D, gl_texture_2.GPU_texture_id);
	// glUniform1i(G->RC.uniforms.texture_ids[1], 1);

	const GL_Mesh& Quad_mesh = G->RC.quad_mesh;

	glBindBuffer(GL_ARRAY_BUFFER, Quad_mesh.vertex_buffer_id);
	glVertexAttribPointer(
		G->RC.attributes.position_id,  // attribute - Specifies the index of the generic vertex attribute to be modified
		3,                             // size - the number of components per generic vertex attribute. Must be 1, 2, 3, 4
		GL_FLOAT,                      // type - specifies the data type of each component in the array
		GL_FALSE,                      // normalized ?
		sizeof(GLfloat) * 3,           // stride - Specifies the byte offset between consecutive generic vertex attributes. If stride is 0, the generic vertex attributes are understood to be tightly packed in the array. The initial value is 0.
		(void*)0                       // array buffer offset - Specifies a offset of the first component of the first generic vertex attribute in the array 
	);
	glEnableVertexAttribArray(G->RC.attributes.position_id);

	glBindBuffer(GL_ARRAY_BUFFER, Quad_mesh.tex_coord_buffer_id);
	glVertexAttribPointer(
		G->RC.attributes.tex_coord_id,  // attribute - Specifies the index of the generic vertex attribute to be modified
		2,                              // size - the number of components per generic vertex attribute. Must be 1, 2, 3, 4
		GL_FLOAT,                       // type - specifies the data type of each component in the array
		GL_FALSE,                       // normalized ?
		sizeof(GLfloat) * 2,            // stride - Specifies the byte offset between consecutive generic vertex attributes. If stride is 0, the generic vertex attributes are understood to be tightly packed in the array. The initial value is 0.
		(void*)0                        // array buffer offset - Specifies a offset of the first component of the first generic vertex attribute in the array 
	);
	glEnableVertexAttribArray(G->RC.attributes.tex_coord_id);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Quad_mesh.index_buffer_id);
	glDrawElements(
		gl_primitive_id_to_draw,  // GL_TRIANGLES / GL_LINE_STRIP
		Quad_mesh.index_count,    // Vertex count
		GL_UNSIGNED_SHORT,        // type
		(void*)0                  // element array buffer offset
	);

	glDisableVertexAttribArray(G->RC.attributes.position_id);
	glDisableVertexAttribArray(G->RC.attributes.tex_coord_id);

	// Too many OpenGL API calls for a single quad. I know.
	// But there are not so many quads for now.
	// It was originally written for meshes which consist of a large number of triangles.
}

void GL_Render::Update_view_matrices()
{
	float window_w = static_cast<float>(glutGet(GLUT_WINDOW_WIDTH));
	float window_h = static_cast<float>(glutGet(GLUT_WINDOW_HEIGHT));

	Update_window_to_opengl_vp_matrix(window_w, window_h);
	Update_opengl_vp_to_game_vp_matrix(window_w, window_h);
	Update_game_vp_to_game_coordinates_matrix();

	inv_opengl_to_game_coordinates_matrix = inv_opengl_vp_to_game_vp_matrix * inv_game_vp_to_game_coordinates_matrix;
}

void GL_Render::Update_opengl_vp_to_game_vp_matrix(float window_w, float window_h)  // Bounding game view port into OpenGL view port
{
	// -----------------  Wide window
	// -   --------    -
	// -   --------    -
	// -   --------    -
	// -----------------

	// -------------  High window
	// -           -
	// -------------
	// -------------
	// -------------
	// -------------
	// -------------
	// -------------
	// -           -
	// -------------

	const float game_w = G->game_root.g_s.game_w;
	const float game_h = G->game_root.g_s.game_h;

	window_w = std::max(window_w, 1.0f);  // Protection from division by zero
	window_h = std::max(window_h, 1.0f);  // Protection from division by zero

	float kx = game_w / window_w;
	float ky = game_h / window_h;

	kx = std::max(kx, 0.0001f);  // Protection from division by zero
	ky = std::max(ky, 0.0001f);  // Protection from division by zero

	inv_opengl_vp_to_game_vp_matrix = glm::mat4(1.0f);

	if ((game_w * window_h) < (game_h * window_w))  // Aspect ratio comparison
	{
		float k = kx / ky;
		inv_opengl_vp_to_game_vp_matrix = glm::scale(inv_opengl_vp_to_game_vp_matrix, glm::vec3(k, 1.0f, 1.0f));  // Wide window
	}
	else
	{
		float k = ky / kx;
		inv_opengl_vp_to_game_vp_matrix = glm::scale(inv_opengl_vp_to_game_vp_matrix, glm::vec3(1.0f, k, 1.0f));  // High window
	}

	opengl_vp_to_game_vp_matrix = glm::inverse(inv_opengl_vp_to_game_vp_matrix);  // Calculate inverse matrix
}

void GL_Render::Update_window_to_opengl_vp_matrix(float window_w, float window_h)
{
	// Window coordinates ---> OpenGL view port
	window_to_opengl_vp_matrix = glm::mat4(1.0f);

	float dx = -1.0f;
	float dy = 1.0f;
	window_to_opengl_vp_matrix = glm::translate(window_to_opengl_vp_matrix, glm::vec3(dx, dy, 0.0f));  // Something like: [0.0f; 2.0f] ---> [-1.0f; 1.0f]

	float sx = 2.0f / window_w;
	float sy = -2.0f / window_h;
	window_to_opengl_vp_matrix = glm::scale(window_to_opengl_vp_matrix, glm::vec3(sx, sy, 1.0f));  // Pixels [0.0f; 1024.0f] ---> [0.0f; 2.0f]

	inv_window_to_opengl_vp_matrix = glm::inverse(window_to_opengl_vp_matrix);  // Calculate inverse matrix
}

void GL_Render::Update_game_vp_to_game_coordinates_matrix()
{
	game_vp_to_game_coordinates_matrix = glm::mat4(1.0f);

	// Game view port ---> Game coordinates (in pixels)
	float sx = G->game_root.g_s.game_w / 2.0f;
	float sy = G->game_root.g_s.game_h / 2.0f;
	game_vp_to_game_coordinates_matrix = glm::scale(game_vp_to_game_coordinates_matrix, glm::vec3(sx, sy, 1.0f));  // [0.0f; 2.0f] ---> Pixels [0.0f; 1024.0f]

	float dx = 1.0f;
	float dy = 1.0f;
	game_vp_to_game_coordinates_matrix = glm::translate(game_vp_to_game_coordinates_matrix, glm::vec3(dx, dy, 0.0f));  // Something like: [-1.0f; 1.0f] ---> [0.0f; 2.0f]

	inv_game_vp_to_game_coordinates_matrix = glm::inverse(game_vp_to_game_coordinates_matrix);  // Calculate inverse matrix
}

glm::vec2 GL_Render::To_game_coordinates(glm::vec2 pos) const
{
	glm::vec4 pos_vec4(pos, 0.0f, 1.0f);  // 1.0f to calculate glm::translate

	pos_vec4 = game_vp_to_game_coordinates_matrix *
		opengl_vp_to_game_vp_matrix *
		window_to_opengl_vp_matrix *
		pos_vec4;

	return glm::vec2(pos_vec4.x, pos_vec4.y);
}

void GL_Render::Draw_counter(unsigned int n, float x, float y) const
{
	if (n > 99)
	{
		return;  // Only [0..99] numbers supported.
	}

	int digit_1 = n / 10;  // Integer division
	int digit_2 = n % 10;  // Integer mod

	if (n < 10)
	{
		G->gl_render.Draw_quad(G->RC.digit_textures[digit_2], x, y);  // Digit 2
	}
	else
	{
		G->gl_render.Draw_quad(G->RC.digit_textures[digit_1], x - 17.0f, y);  // Digit 1
		G->gl_render.Draw_quad(G->RC.digit_textures[digit_2], x + 17.0f, y);  // Digit 2
	}
}