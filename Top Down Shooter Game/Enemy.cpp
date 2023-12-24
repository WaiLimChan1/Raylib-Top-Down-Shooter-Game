#include "Enemy.h"

Texture2D Enemy::StaticRunTexture;
Texture2D Enemy::StaticDeadTexture;

void Enemy::LoadTextures()
{
	StaticRunTexture = LoadTexture("Textures/Characters/Enemy/run.png");
	StaticDeadTexture = LoadTexture("Textures/Characters/Enemy/dead.png");
}
void Enemy::UnloadTextures()
{
	UnloadTexture(StaticRunTexture);
	UnloadTexture(StaticDeadTexture);
}

Enemy::Enemy() {}
Enemy::Enemy(Vector2 centerPos, float movementSpeed, bool facingLeft, int maxHealth, float scale, float animationSpeed)
	: Character(centerPos, movementSpeed, facingLeft, maxHealth, scale, animationSpeed)
{
	lastStatus = RUN;
	status = RUN;

	direction = { 0,0 };

	completelyDead = false;

	run = AnimationPro(StaticRunTexture, scale, animationSpeed, 7);
	dead = AnimationPro(StaticDeadTexture, scale, animationSpeed, 1);

	current = run;

	setHitBoxCenterPos(centerPos);
}

void Enemy::takeInput(Vector2 characterPos)
{
	if (currentHealth > 0)
	{
		direction = Vector2Normalize(Vector2Subtract(characterPos, getHitBoxCenterPos()));
		if (direction.x < 0) facingLeft = true;
		else if (direction.x > 0) facingLeft = false;
	}
}