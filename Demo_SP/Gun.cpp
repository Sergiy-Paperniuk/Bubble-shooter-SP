#include "stdafx.h"

#include "glm/gtc/matrix_transform.hpp"  // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <list>

#include "Utils.h"
#include "Gun.h"

Gun::Gun() :
	angle(0.0f),
	pos(glm::vec2(0.0f, 0.0f)),
	bullet_spawn_offset(0.0f)
{}

void Gun::TakeAim(glm::vec2 aimPoint )
{
	glm::vec2 gunDirection = aimPoint - pos;

	if ((abs(gunDirection.x) < 1.0f) && (abs(gunDirection.y) < 1.0f))
	{
		angle = 0.0f;  // UP
	}
	else
	{
		angle = atan2(gunDirection.y, gunDirection.x);  // atan2(y, x); O_O    In radians
		angle += utils::PI * 1.5f ;  // Coordinates system correction.  Radians.  0 == UP.  0.5f * PI == LEFT.
	}
}

void Gun::Shoot( std::list<Bullet*>& bullets, float currentTime, float bulletSpeed, float bulletLifeTime )
{
	float bullet_angle = angle + utils::PI * 0.5f;
	glm::vec2 direction( cos(bullet_angle), sin(bullet_angle) );

	glm::vec2 bulletPos = pos + ( direction * bullet_spawn_offset );
	glm::vec2 bulletSpeedVector( direction * bulletSpeed );
	Bullet* bullet = new Bullet(bulletPos, bulletSpeedVector, currentTime, bulletLifeTime );
	bullets.push_back( bullet );
}

void Gun::SetAngle(float angle_init )  // Radians
{
	angle = angle_init;
}

float Gun::GetAngle() const  // Radians
{
	return angle;
}

void Gun::SetPos(glm::vec2 pos_init )
{
	pos = pos_init;
}

glm::vec2 Gun::GetPos() const
{
	return pos;
}

void Gun::SetBulletSpawnOffset(float bullet_spawn_offset_init)
{
	bullet_spawn_offset = bullet_spawn_offset_init;
}

float Gun::GetBulletSpawnOffset() const
{
	return bullet_spawn_offset;
}

void Gun::Draw(const GL_Render& gl_render, const GL_Texture& gun_texture) const
{
	glm::mat4 gun_mat = glm::translate(glm::mat4(1.0f), glm::vec3(pos, 0.0f));
	gun_mat = glm::rotate(gun_mat, angle, glm::vec3(0.0f, 0.0f, 1.0f));
	gun_mat = glm::translate(gun_mat, glm::vec3(0.0f, 42.0f, 0.0f));
	gl_render.Draw_quad(gun_texture, 0.0f, 0.0f, gun_mat);
}