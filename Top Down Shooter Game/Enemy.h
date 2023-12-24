#pragma once
#include "Header.h"
#include "Character.h"

class Enemy : public Character
{
protected:
	static Texture2D StaticRunTexture;
	static Texture2D StaticDeadTexture;

protected:
public:
	static void LoadTextures();
	static void UnloadTextures();

	Enemy();
	Enemy(Vector2 centerPos, float movementSpeed, bool facingLeft, int maxHealth, float scale, float animationSpeed);

	void takeInput(Vector2 characterPos);
};