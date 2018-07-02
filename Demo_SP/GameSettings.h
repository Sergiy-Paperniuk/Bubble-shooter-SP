#pragma once

#include "glm/vec2.hpp"  // glm::vec2

class GameSettings
{
public:
	GameSettings();

	float game_w;  // Pixels
	float game_h;  // Pixels

	int initial_window_x;  // Pixels
	int initial_window_y;  // Pixels

	int initial_window_w;  // Pixels
	int initial_window_h;  // Pixels

	glm::vec2 gun_pos;  // Pixels
	float bullet_spawn_offset;  // Pixels

	float bullet_life_time;  // Seconds
	float min_shooting_period;  // Seconds
	float bubble_min_speed;  // Pixels/second
	float bubble_max_speed;  // Pixels/second
	
	unsigned int bubbles_number;
	float bullet_speed;
	float round_time;  // Seconds. Time to play one round.

	float bubble_width;  // Pixels
	float bubble_height;  // Pixels

	float bullet_width;  // Pixels
	float bullet_height;  // Pixels

private:
	void SetDefaultSettings();
};