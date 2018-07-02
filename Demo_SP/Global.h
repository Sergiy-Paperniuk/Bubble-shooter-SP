#pragma once
#include <string>

#include <glew.h>
#include <glut.h>
#include "glm/vec4.hpp"  // glm::vec4
#include "glm/mat4x4.hpp"  // glm::mat4

#include "Resources_controller.h"
#include "GameRoot.h"
#include "GL_Render.h"

class Global
{
public:
	std::string Resources_base_path;

	Resources_controller RC;
	GL_Render gl_render;
	GameRoot game_root;

	Global();
	~Global();
};