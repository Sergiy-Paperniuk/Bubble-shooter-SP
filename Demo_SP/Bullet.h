#pragma once

#include "glm/vec2.hpp"  // glm::vec2
#include "GL_Render.h"

class Bullet
{
public:
	Bullet( glm::vec2 pos_init, glm::vec2 speed_init, float life_time_init, float life_start_time_init);
	~Bullet();

	void Update(float dt);
	void Draw(const GL_Render& gl_render, const GL_Texture& bullet_texture) const;
	
	glm::vec2 GetPos() const;

	bool IsLifetimeEnded(float current_time);

private:
	glm::vec2 pos;
	glm::vec2 speed;

	float life_time;  // Seconds
	float life_start_time;  // Seconds
};