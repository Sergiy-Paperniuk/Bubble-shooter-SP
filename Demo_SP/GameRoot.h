#pragma once

#include <list>

#include "GL_Render.h"
#include "GameSettings.h"
#include "Bubble.h"
#include "Bullet.h"
#include "Gun.h"

enum GAME_STATE 
{
	NONE = 0,
	GAME = 1,
	RESULT = 2
};

class GameRoot
{
public:  // Code
	GameRoot();
	~GameRoot();

	void Init();
	void Update();
	void Draw(const GL_Render& gl_render) const;

	// Mouse
	void LMB_down(glm::vec2 pos);
	void LMB_up(glm::vec2 pos);
	void LMB_move(glm::vec2 pos);

	void RMB_down(glm::vec2 pos);
	void RMB_up(glm::vec2 pos);

	void TakeAim(glm::vec2 aimPoint);
	void TryShoot( float currentTime );

	void InitRound( float currentTime );

private:  // Code
	void SpawnRandomBubbles();
	void SpawnRandomBullets();  // For debug

	void TryAutoShoot(float currentTime);

	void UpdateBubbles(float dt);
	void UpdateBullets(float dt);
	void HandleBulletsLifeTime(float currentTime);
	void HandleBulletHits();

	void Shoot(float currentTime);

	void DrawBackground(const GL_Render& gl_render) const;
	void DrawBubbles(const GL_Render& gl_render) const;
	void DrawBullets(const GL_Render& gl_render) const;
	void DrawGun(const GL_Render& gl_render) const;
	void DrawTimeLeftCounter(const GL_Render& gl_render) const;
	void DrawWinLoseScreen(const GL_Render& gl_render) const;

	void DeleteAllBubbles();
	void DeleteAllBullets();

	float GetTimeRemaining(float currentTime);
	bool CanFinishRound(float timeRemaining);
	void TryFinishRound(float currentTime);

public:  // Data
	GameSettings g_s;

private:  // Data
	int curr_milliseconds = 0;
	int prev_milliseconds = 0;

	float timeLeftToPrint = 0.0f;
	unsigned int bubblesDestroyedToPrint = 0;

	float roundStartTime = 0.0f;  // Seconds
	bool isLeftMouseButtonPressed = false;
	float lastShotTime = -10.0f;  // Seconds
	bool isShotScheduled = false;

	int gameState;

	std::list<Bubble*> bubbles_list;
	std::list<Bullet*> bullets_list;
	Gun gun;
};