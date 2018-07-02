#pragma once

#include "glm/vec2.hpp"  // glm::vec2
#include "GL_Render.h"

class Bubble
{
public:
	Bubble( glm::vec2 pos_init, glm::vec2 speed_init );

	void Update(float dt);
	void Draw(const GL_Render& gl_render, const GL_Texture& bubble_texture) const;

	glm::vec2 GetPos() const;

private:
	glm::vec2 pos;
	glm::vec2 speed;
};