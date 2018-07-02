#include "stdafx.h"
#include "Bullet.h"

#include "Global.h"

extern Global* G;

Bullet::Bullet(glm::vec2 pos_init, glm::vec2 speed_init, float life_time_init, float life_start_time_init) :
	pos(pos_init),
	speed(speed_init),
	life_time(life_time_init),  // Seconds
	life_start_time(life_start_time_init)  // Seconds
{}

Bullet::~Bullet()
{}

void Bullet::Update(float dt)
{
	pos += speed * dt;

	const GameSettings& g_s = G->game_root.g_s;

	float min_x = g_s.bullet_width * 0.5f;
	float max_x = g_s.game_w - g_s.bullet_width * 0.5f;
	float min_y = g_s.bullet_height * 0.5f;
	float max_y = g_s.game_h - g_s.bullet_height * 0.5f;

	if (pos.x > max_x)
	{
		pos.x = max_x;
		speed.x = -speed.x;
	}
	else
	{
		if (pos.x < min_x)
		{
			pos.x = min_x;
			speed.x = -speed.x;
		}
	}

	if (pos.y > max_y)
	{
		pos.y = max_y;
		speed.y = -speed.y;
	}
	else
	{
		if (pos.y < min_y)
		{
			pos.y = min_y;
			speed.y = -speed.y;
		}
	}
}

void Bullet::Draw(const GL_Render& gl_render, const GL_Texture& bullet_texture) const
{
	gl_render.Draw_quad(bullet_texture, pos.x, pos.y);
}

glm::vec2 Bullet::GetPos() const
{
	return pos;
}

bool Bullet::IsLifetimeEnded(float current_time)
{
	bool is_lifetime_ended = (current_time > (life_start_time + life_time));
	return is_lifetime_ended;
}