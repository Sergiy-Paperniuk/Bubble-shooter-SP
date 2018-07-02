#include "stdafx.h"
#include "GameRoot.h"

#include "Global.h"
#include "Utils.h"

// For glm::distance2 (squared distance)
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/norm.hpp"  // glm::distance2

extern Global* G;

GameRoot::GameRoot() :
	gameState( GAME_STATE::NONE )
{
	gun.SetPos( glm::vec2( 0.0f, 0.0f ) );
	gun.SetAngle(0.0f);
}

GameRoot::~GameRoot()
{
  DeleteAllBubbles();
  DeleteAllBullets();
}

void GameRoot::Init( )
{
	InitRound(0.0f);
}

void GameRoot::Update()
{
	curr_milliseconds = glutGet(GLUT_ELAPSED_TIME);
	float dt = static_cast<float>(curr_milliseconds - G->game_root.prev_milliseconds) * 0.001f;
	prev_milliseconds = curr_milliseconds;

	float current_time = static_cast<float>(curr_milliseconds) * 0.001f;
	// float is not precise enough for big time values. 
	// But for times like few hours - it should be OK.

	// TryShoot(currentTime);  // For debug

	if( gameState != GAME_STATE::GAME )
	{
		return;
	}

	UpdateBubbles(dt);
	UpdateBullets(dt);

	HandleBulletsLifeTime(current_time);
	HandleBulletHits();

	TryAutoShoot(current_time);
	TryFinishRound(current_time);
}

void GameRoot::Draw(const GL_Render& gl_render) const
{
	DrawBackground(gl_render);

	if (gameState == GAME_STATE::GAME)
	{
		DrawBubbles(gl_render);
		DrawBullets(gl_render);
	}

	DrawGun(gl_render);
	DrawTimeLeftCounter(gl_render);

	if (gameState == GAME_STATE::RESULT)
	{
		DrawWinLoseScreen(gl_render);
	}
}

void GameRoot::LMB_down(glm::vec2 pos)
{
	Update();

	float current_time = static_cast<float>(curr_milliseconds) * 0.001f;

	if (gameState == GAME_STATE::RESULT)
	{
		InitRound(current_time);
		return;
	}

	isLeftMouseButtonPressed = true;

	TakeAim(pos);
	TryShoot(current_time);
}

void GameRoot::LMB_up(glm::vec2 pos)
{
	isLeftMouseButtonPressed = false;
}

void GameRoot::LMB_move(glm::vec2 pos)
{
	G->gl_render.Update_view_matrices();
	glm::vec2 mouse_game_coordinates = G->gl_render.To_game_coordinates(pos);

	TakeAim(mouse_game_coordinates);
}

void GameRoot::RMB_down(glm::vec2 pos)
{}

void GameRoot::RMB_up(glm::vec2 pos)
{}

void GameRoot::UpdateBubbles(float dt)
{
	for(const auto& bubble : bubbles_list)
	{
		bubble->Update(dt);
	}
}

void GameRoot::UpdateBullets(float dt)
{
	for(const auto& bullet : bullets_list)
	{
		bullet->Update(dt);
	}
}

void GameRoot::HandleBulletsLifeTime(float currentTime)
{
	std::list<Bullet*>::iterator it = bullets_list.begin();
	while (it != bullets_list.end())
	{
		Bullet* bullet = *it;

		if (bullet->IsLifetimeEnded(currentTime))
		{
			delete bullet;
			it = bullets_list.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void GameRoot::HandleBulletHits()
{
	float bubbleRadius = g_s.bubble_width * 0.5f;
	float bulletRadius = g_s.bullet_width * 0.5f;
	float hitDistanceSquared = utils::sqr(bubbleRadius + bulletRadius);

	std::list<Bullet*>::iterator it_bullet = bullets_list.begin();
	while (it_bullet != bullets_list.end())
	{
		Bullet* bullet = *it_bullet;

		bool isDeleteBullet = false;

		std::list<Bubble*>::iterator it_bubble = bubbles_list.begin();
		while (it_bubble != bubbles_list.end())
		{
			Bubble* bubble = *it_bubble;
			float distanceSquared = glm::distance2( bubble->GetPos(), bullet->GetPos());

			if (distanceSquared < hitDistanceSquared)
			{
				delete bubble;
				it_bubble = bubbles_list.erase(it_bubble);

				isDeleteBullet = true;

				break;
				// After the first bubble hit - immediately destroy the bullet.
				// One bullet can NOT destroy two bubles.
			}
			else
			{
				++it_bubble;
			}
		}

		if (isDeleteBullet)
		{
			delete bullet;
			it_bullet = bullets_list.erase(it_bullet);
		}
		else
		{
			++it_bullet;
		}
	}
}

void GameRoot::TryFinishRound(float currentTime)
{
	// Round time
	float timeRemaining = GetTimeRemaining(currentTime);

	timeLeftToPrint = ceil(timeRemaining);
	if (timeLeftToPrint < 1.0f)
	{
		timeLeftToPrint = 0.0f;
	}

	if( CanFinishRound( timeRemaining ) )
	{
		bubblesDestroyedToPrint = static_cast<unsigned int>(g_s.bubbles_number - static_cast<unsigned int>(bubbles_list.size()));
		DeleteAllBubbles();
		DeleteAllBullets();
		gameState = GAME_STATE::RESULT;
	}
}

float GameRoot::GetTimeRemaining(float currentTime)
{
	return (roundStartTime + g_s.round_time) - currentTime;
}

bool GameRoot::CanFinishRound( float timeRemaining )
{
	return ( ( timeRemaining < 0) || bubbles_list.empty() );
}

void GameRoot::Shoot(float currentTime)
{
	gun.Shoot( bullets_list, currentTime, g_s.bullet_speed, g_s.bullet_life_time );
}

void GameRoot::TakeAim(glm::vec2 aimPoint)
{
	gun.TakeAim( aimPoint );
}

void GameRoot::TryShoot( float currentTime )
{
	if(gameState != GAME_STATE::GAME)
	{
		return;
	}

	if (currentTime > lastShotTime + g_s.min_shooting_period)
	{
		lastShotTime = currentTime;
		Shoot(currentTime);
	}
	else
	{
		isShotScheduled = true;  // Schedule a shot. The gun will fire automatically, when the time (when you can fire) comes.
	}
}

void GameRoot::TryAutoShoot(float currentTime)
{
	if(gameState != GAME_STATE::GAME)
	{
		return;
	}

	// Automatic shooting
	if ((isLeftMouseButtonPressed) && (currentTime > lastShotTime + g_s.min_shooting_period))
	{
		lastShotTime = currentTime;
		isShotScheduled = false;

		Shoot(currentTime);

		return;
	}

	// Shot scheduled
	if (!(isLeftMouseButtonPressed) && (currentTime > lastShotTime + g_s.min_shooting_period))
	{
		if (isShotScheduled)
		{
			lastShotTime = currentTime;
			isShotScheduled = false;

			Shoot(currentTime);

			return;
		}
	}
}

void GameRoot::InitRound( float currentTime )
{
	DeleteAllBubbles();
	DeleteAllBullets();
	SpawnRandomBubbles();

	// SpawnRandomBullets();  // For debug

	lastShotTime = -10.0f;  // Seconds
	isLeftMouseButtonPressed = false;
	isShotScheduled = false;
	roundStartTime = currentTime;  // Seconds

	gun.SetPos( g_s.gun_pos );
	gun.SetBulletSpawnOffset( g_s.bullet_spawn_offset );

	gameState = GAME_STATE::GAME;
}

void GameRoot::DrawBackground(const GL_Render& gl_render) const
{
	const float bg_x = g_s.game_w * 0.5f;
	const float bg_y = g_s.game_h * 0.5f;

	gl_render.Draw_quad(G->RC.bg_texture, bg_x, bg_y);  // Bg
}

void GameRoot::DrawBubbles(const GL_Render& gl_render) const
{
	for (const auto& bubble : bubbles_list)
	{
		bubble->Draw(gl_render, G->RC.bubble_texture);
	}
}

void GameRoot::DrawBullets(const GL_Render& gl_render) const
{
	for (const auto& bullet : bullets_list)
	{
		bullet->Draw(gl_render, G->RC.bullet_texture);
	}
}

void GameRoot::DrawGun(const GL_Render& gl_render) const
{
	gun.Draw(gl_render, G->RC.gun_texture);
}

void GameRoot::DrawTimeLeftCounter(const GL_Render& gl_render) const
{
	unsigned int time_left = static_cast<int>(timeLeftToPrint);
	gl_render.Draw_counter(time_left, 516.0f, 730.0f);
}

void GameRoot::DrawWinLoseScreen(const GL_Render& gl_render) const
{
	// "Win" and "Lose" screens
	if (bubblesDestroyedToPrint == g_s.bubbles_number)
	{
		// Win
		gl_render.Draw_quad(G->RC.win_texture, 511.0f, 445.0f);  // "You win !!!" text
		gl_render.Draw_quad(G->RC.me_gusta_mucho_face_texture, 170.0f, 595.0f);  // Me gusta
		gl_render.Draw_quad(G->RC.awesome_face_texture, 842.0f, 592.0f);  // Awesome
	}
	else
	{
		// Lose
		gl_render.Draw_quad(G->RC.lose_texture, 511.0f, 445.0f);  // "You lose" text
	}

	// Bubbles destroyed text
	gl_render.Draw_quad(G->RC.bubbles_destroyed_texture, 511.0f, 357.0f);

	// Destroyed bubbles counter, like: "15/20"
	gl_render.Draw_counter(bubblesDestroyedToPrint, 752.0f, 392.0f);
	gl_render.Draw_counter(g_s.bubbles_number, 837.0f, 392.0f);
}

void GameRoot::DeleteAllBubbles()
{
	for (auto& bubble : bubbles_list)
	{
		delete bubble;
		bubble = nullptr;
	}

	bubbles_list.clear();
}

void GameRoot::DeleteAllBullets()
{
	for (auto& bullet : bullets_list)
	{
		delete bullet;
		bullet = nullptr;
	}

	bullets_list.clear();
}

void GameRoot::SpawnRandomBubbles()
{
	float min_x = g_s.bubble_width * 0.5f;
	float max_x = g_s.game_w - g_s.bubble_width * 0.5f;
	float min_y = g_s.bubble_height * 0.5f;
	float max_y = g_s.game_h - g_s.bubble_height * 0.5f;

	const unsigned int bubbles_number = g_s.bubbles_number;

	for (unsigned int i = 0; i < bubbles_number; ++i)
	{
		glm::vec2 pos(utils::RandomFloat(min_x, max_x), utils::RandomFloat(min_y, max_y));
		float angle = utils::RandomFloat( 0.0f, 2.0f * utils::PI);

		float speedVectorLength = utils::RandomFloat(g_s.bubble_min_speed, g_s.bubble_max_speed);
		// float speedVectorLength = 0.0f;  // For debug

		glm::vec2 speed_direction( cos(angle), sin(angle) );
		glm::vec2 speed = speed_direction * speedVectorLength;

		Bubble* bubble = new Bubble( pos, speed );

		bubbles_list.push_back( bubble );
	}
}

void GameRoot::SpawnRandomBullets()  // For debug
{
	const unsigned int bullets_number = 20;

	float min_x = g_s.bullet_width * 0.5f;
	float max_x = g_s.game_w - g_s.bullet_width * 0.5f;
	float min_y = g_s.bullet_height * 0.5f;
	float max_y = g_s.game_h - g_s.bullet_height * 0.5f;

	for (unsigned int i = 0; i < bullets_number; ++i)
	{
		glm::vec2 pos(utils::RandomFloat(min_x, max_x), utils::RandomFloat(min_y, max_y));
		float angle = utils::RandomFloat(0.0f, utils::PI * 2.0f);

		float speedVectorLength = g_s.bullet_speed;
		// float speedVectorLength = 0.0f;  // For debug

		glm::vec2 speed_direction(cos(angle), sin(angle));
		glm::vec2 speed = speed_direction * speedVectorLength;

		Bullet* bullet = new Bullet(pos, speed, 1000.0f, 0.0f);

		bullets_list.push_back(bullet);
	}
}
