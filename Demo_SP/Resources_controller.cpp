#include "stdafx.h"
#include "Resources_controller.h"
#include "GL_Texture.h"
#include "GLSL_Shader.h"
#include "Global.h"
#include "Quad_mesh.h"

extern Global* G;

Resources_controller::Resources_controller()
{
}

Resources_controller::~Resources_controller()
{
}

void Resources_controller::Load_resources()
{
	Load_geometry();
	Load_shaders();
	Load_textures();
}

void Resources_controller::Load_geometry()
{
	Load_quad_mesh_to_GPU( quad_mesh );
}

void Resources_controller::Load_shaders()
{
	simple_texture_vertex_shader = make_shader(
		GL_VERTEX_SHADER,
		(G->Resources_base_path + "Resources\\Shaders\\simple_texture_glsl.vsh").c_str()
	);
	if (simple_texture_vertex_shader == 0)
	{
		return;
	}

	simple_texture_fragment_shader = make_shader(
		GL_FRAGMENT_SHADER,
		(G->Resources_base_path + "Resources\\Shaders\\simple_texture_glsl.fsh").c_str()
	);
	if (simple_texture_fragment_shader == 0)
	{
		return;
	}

	simple_texture_program = make_program(G->RC.simple_texture_vertex_shader, G->RC.simple_texture_fragment_shader);
	if (simple_texture_program == 0)
	{
		return;
	}

	uniforms.proj_matrix_id = glGetUniformLocation(G->RC.simple_texture_program, "proj_matrix");

	uniforms.texture_ids[0] = glGetUniformLocation(G->RC.simple_texture_program, "textures[0]");
	uniforms.texture_ids[1] = glGetUniformLocation(G->RC.simple_texture_program, "textures[1]");

	attributes.position_id = glGetAttribLocation(G->RC.simple_texture_program, "position");
	attributes.tex_coord_id = glGetAttribLocation(G->RC.simple_texture_program, "tex_coord");
}

void Resources_controller::Load_textures()
{
	/*
	  bg.tga ------------------- 24 bit -- RGB
	  bubble.tga --------------- 32 bit -- RGBA
	  bubble_explosion.tga ----- 32 bit -- RGBA
	  gun.tga ------------------ 32 bit -- RGBA
	  bullet.tga --------------- 32 bit -- RGBA

	  Digit_0.tga -------------- 32 bit -- RGBA
	  ...
	  Digit_9.tga -------------- 32 bit -- RGBA

	  win.tga ------------------ 32 bit -- RGBA
	  lose.tga ----------------- 32 bit -- RGBA
	  bubbles_destroyed.tga ---- 32 bit -- RGBA

	  me_gusta_mucho_face.tga -- 32 bit -- RGBA
	  awesome_face.tga --------- 32 bit -- RGBA
	*/

	bg_texture.Make_texture((G->Resources_base_path + "Resources\\Textures\\bg.tga").c_str(), GL_LINEAR, GL_LINEAR, GL_RGB8, GL_BGR, GL_UNSIGNED_BYTE);

	//bubble_texture.Make_texture((G->Resources_base_path + "Resources\\Textures\\bubble.tga").c_str(), GL_NEAREST, GL_NEAREST, GL_RGBA8, GL_BGRA, GL_UNSIGNED_BYTE);
	bubble_texture.Make_texture((G->Resources_base_path + "Resources\\Textures\\bubble.tga").c_str(), GL_LINEAR, GL_LINEAR, GL_RGBA8, GL_BGRA, GL_UNSIGNED_BYTE);

	bubble_explosion_texture.Make_texture((G->Resources_base_path + "Resources\\Textures\\bubble_explosion.tga").c_str(), GL_LINEAR, GL_LINEAR, GL_RGBA8, GL_BGRA, GL_UNSIGNED_BYTE);

	// gun_texture.make_texture((G->Resources_base_path + "Resources\\Textures\\gun.tga").c_str(), GL_NEAREST, GL_NEAREST, GL_RGBA8, GL_BGRA, GL_UNSIGNED_BYTE);
	gun_texture.Make_texture((G->Resources_base_path + "Resources\\Textures\\gun.tga").c_str(), GL_LINEAR, GL_LINEAR, GL_RGBA8, GL_BGRA, GL_UNSIGNED_BYTE);

	bullet_texture.Make_texture((G->Resources_base_path + "Resources\\Textures\\bullet.tga").c_str(), GL_LINEAR, GL_LINEAR, GL_RGBA8, GL_BGRA, GL_UNSIGNED_BYTE);

	for (int i = 0; i < 10; i++)
	{
		std::string file_name = G->Resources_base_path + std::string("Resources\\Textures\\Digits\\Digit_") + std::to_string(i) + std::string(".tga");
		digit_textures[i].Make_texture(file_name.c_str(), GL_LINEAR, GL_LINEAR, GL_RGBA8, GL_BGRA, GL_UNSIGNED_BYTE);
	}

	win_texture.Make_texture((G->Resources_base_path + "Resources\\Textures\\win.tga").c_str(), GL_LINEAR, GL_LINEAR, GL_RGBA8, GL_BGRA, GL_UNSIGNED_BYTE);
	lose_texture.Make_texture((G->Resources_base_path + "Resources\\Textures\\lose.tga").c_str(), GL_LINEAR, GL_LINEAR, GL_RGBA8, GL_BGRA, GL_UNSIGNED_BYTE);
	bubbles_destroyed_texture.Make_texture((G->Resources_base_path + "Resources\\Textures\\bubbles_destroyed.tga").c_str(), GL_LINEAR, GL_LINEAR, GL_RGBA8, GL_BGRA, GL_UNSIGNED_BYTE);

	me_gusta_mucho_face_texture.Make_texture((G->Resources_base_path + "Resources\\Textures\\me_gusta_mucho_face.tga").c_str(), GL_LINEAR, GL_LINEAR, GL_RGBA8, GL_BGRA, GL_UNSIGNED_BYTE);
	awesome_face_texture.Make_texture((G->Resources_base_path + "Resources\\Textures\\awesome_face.tga").c_str(), GL_LINEAR, GL_LINEAR, GL_RGBA8, GL_BGRA, GL_UNSIGNED_BYTE);
}

void Resources_controller::Free_resources()
{
	Free_textures();
	Free_shaders();
	Free_geometry();
}

void Resources_controller::Free_textures()
{
	awesome_face_texture.Delete_from_GPU();
	me_gusta_mucho_face_texture.Delete_from_GPU();

	bubbles_destroyed_texture.Delete_from_GPU();
	lose_texture.Delete_from_GPU();
	win_texture.Delete_from_GPU();

	for (int i = 0; i < 10; i++)
	{
		digit_textures[i].Delete_from_GPU();
	}

	bullet_texture.Delete_from_GPU();
	gun_texture.Delete_from_GPU();

	bubble_explosion_texture.Delete_from_GPU();
	bubble_texture.Delete_from_GPU();

	bg_texture.Delete_from_GPU();
}

void Resources_controller::Free_shaders()
{
	glDeleteProgram(simple_texture_program);

	glDeleteShader(simple_texture_fragment_shader);
	glDeleteShader(simple_texture_vertex_shader);
}

void Resources_controller::Free_geometry()
{
	quad_mesh.Delete_from_GPU();
}