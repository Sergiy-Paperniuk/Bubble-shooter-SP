#pragma once

#include "Bullet.h"
#include "GL_Render.h"

class Gun
{
public:
	Gun();

	void TakeAim( glm::vec2 aimPoint );
	void Shoot(std::list<Bullet*>& bullets, float currentTime, float bulletSpeed, float bulletLifeTime);

	void SetAngle(float angle_init );  // Radians
	float GetAngle() const;  // Radians

	void SetPos(glm::vec2 pos_init );
	glm::vec2 GetPos() const;

	void SetBulletSpawnOffset(float bulletSpawnOffset_init);
	float GetBulletSpawnOffset() const;

	void Draw(const GL_Render& gl_render, const GL_Texture& gun_texture) const;

private:
	glm::vec2 pos;  // Pixels
	float angle;  // Radians.  0 == UP.  0.5f * PI == LEFT.
	float bullet_spawn_offset;  // Pixels
};