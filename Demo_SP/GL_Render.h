#pragma once

#include "glm/mat4x4.hpp"  // glm::mat4

#include "glew.h" // glm::mat4
#include "GL_Mesh.h"
#include "GL_Texture.h"

class GL_Render
{
public:  // Code
	void Draw_quad(const GL_Texture& gl_texture, const float dx, const float dy, const glm::mat4& quad_mat = glm::mat4(1.0f)) const;
	void Draw_counter(unsigned int n, float x, float y) const;
	glm::vec2 To_game_coordinates(glm::vec2 pos) const;

	void Update_view_matrices();
	void Update_opengl_vp_to_game_vp_matrix(float window_w, float window_h);  // Bounding game view port into OpenGL view port
	void Update_window_to_opengl_vp_matrix(float window_w, float window_h);
	void Update_game_vp_to_game_coordinates_matrix();

public:  // Data
	int gl_primitive_id_to_draw = GL_TRIANGLES;  // Solid drawing mode
	//int gl_primitive_id_to_draw = GL_LINE_STRIP;  // Wireframe drawing mode

	// inv - inverse (matrices to move from game coordinates to window coordinates)
	// vp - view port
	//
	// Window:  [0.0f..1024.0f][0..768.0f]   Center - top and left side. Positive X - right. Positive Y - down.  (Variable aspect ratio)
	// OpenGL view port:  [-1.0f..1.0f][-1.0f..1.0f]   Center - screen center. Positive X - right. Positive Y - up.  (Variable aspect ratio)
	// Game view port:  [-1.0f..1.0f][-1.0f..1.0f]   Center - screen center. Positive X - right. Positive Y - up.  (Fixed aspect ratio, bounded inside of window)
	// Game coordinates:  [0.0f..1024.0f][0..768.0f]   Center - bottom and left side. Positive X - right. Positive Y - up.  (Fixed aspect ratio. Always 1024 x 768.)

	glm::mat4 window_to_opengl_vp_matrix = glm::mat4(1.0f);  // Window coordinates ---> OpenGL view port.  Mouse clicks are in "window" coordinates.
	glm::mat4 inv_window_to_opengl_vp_matrix = glm::mat4(1.0f);

	glm::mat4 opengl_vp_to_game_vp_matrix = glm::mat4(1.0f);  // OpenGL view port [-1.0f; 1.0f] ---> Game view port [-1.0f; 1.0f].  OpenGL drawing is in OpenGL coordinates.
	glm::mat4 inv_opengl_vp_to_game_vp_matrix = glm::mat4(1.0f);

	glm::mat4 game_vp_to_game_coordinates_matrix = glm::mat4(1.0f);  // Game view port ---> Game coordinates (in pixels).  Game phusics and other game logic are in game coordinates.
	glm::mat4 inv_game_vp_to_game_coordinates_matrix = glm::mat4(1.0f);

	glm::mat4 inv_opengl_to_game_coordinates_matrix = glm::mat4(1.0f);  //  == inv_opengl_vp_to_game_vp_matrix * inv_game_vp_to_game_coordinates_matrix; - For faster calculations
};
