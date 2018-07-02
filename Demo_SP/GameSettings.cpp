#include "stdafx.h"
#include "GameSettings.h"

GameSettings::GameSettings()
{
	SetDefaultSettings();
}

void GameSettings::SetDefaultSettings()
{
	game_w = 1024.0f;  // Pixels
	game_h = 768.0f;  // Pixels

	initial_window_x = 100;  // Pixels
	initial_window_y = 25;  // Pixels

	// initial_window_w = 1500;  // Pixels    // For debug
	initial_window_w = 1024;  // Pixels
	initial_window_h = 768;  // Pixels

	gun_pos = glm::vec2( 512.0f, 150.0f );  // Pixels
	bullet_spawn_offset = 80.0f;  // Pixels

	bullet_life_time = 5.0f;  // Seconds
	min_shooting_period = 0.5f;  // Seconds
	bubble_min_speed = 30.0f;  // Pixels/second
	bubble_max_speed = 60.0f;  // Pixels/second

	bubbles_number = 20;
	bullet_speed = 200.0f;  // Pixels/second
	round_time = 30.0f;  // Seconds. Time to play one round.

	bubble_width = 64.0f;  // Pixels
	bubble_height = 64.0f;  // Pixels

	bullet_width = 10.0f;  // Pixels
	bullet_height = 10.0f;  // Pixels
}