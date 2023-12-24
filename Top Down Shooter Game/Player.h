#pragma once
#include "Header.h"
#include "Character.h"
#include "Pistol.h"
#include "AssaultRifle.h"
#include "ShotGun.h"
#include "Bullet.h"
#include "Collectable.h"

class Player : public Character
{
protected:
	static Texture2D StaticIdleTexture;
	static Texture2D StaticRunTexture;

protected:
	Pistol* pistol;
	AssaultRifle* assaultRifle;
	ShotGun* shotGun;

	Weapon* currentWeapon;

public:
	static void LoadTextures();
	static void UnloadTextures();

	Player();
	Player(Vector2 centerPos, float movementSpeed, bool facingLeft, int maxHealth, float scale, float animationSpeed);

	std::vector<Bullet*>& getBullets();

	void drawWeapon(Vector2 cameraPos);

	void takeInput(Vector2 cameraPos);

	void WeaponLogic(Vector2 cameraPos);
	void upgradeWeapon(Collectable::CollectableType collectableType);
};