#pragma once

#include "Utils.h"
#include "GL_Mesh.h"

#include "GL_Texture.h"

class Resources_controller
{
public:  // Code
	Resources_controller();
	~Resources_controller();

	void Load_resources();
	void Load_geometry();
	void Load_shaders();
	void Load_textures();

	void Free_resources();
	void Free_textures();
	void Free_shaders();
	void Free_geometry();

public:  // Data
	// Geometry (GPU)
	GL_Mesh quad_mesh;

	// Shaders (GPU)
	GLuint simple_texture_vertex_shader = 0;
	GLuint simple_texture_fragment_shader = 0;
	GLuint simple_texture_program = 0;

	struct
	{
		GLint texture_ids[2] = { 0, 0 };
		GLint proj_matrix_id = 0;
	} uniforms;

	struct
	{
		GLint position_id = 0;
		GLint tex_coord_id = 0;
	} attributes;

	// Textures (GPU)
	GL_Texture bg_texture;

	GL_Texture bubble_texture;
	GL_Texture bubble_explosion_texture;
	GL_Texture gun_texture;
	GL_Texture bullet_texture;

	GL_Texture digit_textures[10];

	GL_Texture win_texture;
	GL_Texture lose_texture;
	GL_Texture bubbles_destroyed_texture;

	GL_Texture me_gusta_mucho_face_texture;
	GL_Texture awesome_face_texture;
};

