#include "stdafx.h"
#include "Bubble.h"

#include "Global.h"

extern Global* G;

Bubble::Bubble(glm::vec2 pos_init, glm::vec2 speed_init) :
	pos(pos_init),
	speed(speed_init)
{}

void Bubble::Update(float dt)
{
	pos += speed * dt;

	const GameSettings& g_s = G->game_root.g_s;

	float min_x = g_s.bubble_width * 0.5f;
	float max_x = g_s.game_w - g_s.bubble_width * 0.5f;
	float min_y = g_s.bubble_height * 0.5f;
	float max_y = g_s.game_h - g_s.bubble_height * 0.5f;

	if( pos.x > max_x )
	{
		pos.x = max_x;
		speed.x = -speed.x;
	}
	else
	{
		if(pos.x < min_x )
		{
			pos.x = min_x;
			speed.x = -speed.x;
		}
	}

	if(pos.y > max_y)
	{
		pos.y = max_y;
		speed.y = -speed.y;
	}
	else
	{
		if(pos.y < min_y )
		{
			pos.y = min_y;
			speed.y = -speed.y;
		}
	}
}

glm::vec2 Bubble::GetPos() const
{
	return pos;
}

void Bubble::Draw(const GL_Render& gl_render, const GL_Texture& bubble_texture) const
{
	gl_render.Draw_quad(bubble_texture, pos.x, pos.y);
}
