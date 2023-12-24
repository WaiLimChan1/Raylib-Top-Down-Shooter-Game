#pragma once
#include "Header.h"
#include "Pistol.h"
#include "AssaultRifle.h"
#include "ShotGun.h"

class Collectable
{
public:
	static enum CollectableType {PISTOL_COLLECTABLE, ASSAULT_RIFLE_COLLECTABLE, SHOT_GUN_COLLECTABLE};

private:
	CollectableType collectableType;

	Texture2D texture;
	float scale;
	Vector2 pos;

public:
	Collectable();
	Collectable(CollectableType collectableType, float scale, Vector2 centerPos);

	CollectableType getCollectableType();
	Vector2 getHitBoxCenterPos();

	void setPos(Vector2 pos);
	void setHitBoxCenterPos(Vector2 destination);

	Rectangle getHitBox(Vector2 cameraPos);
	void drawHitBox(Vector2 cameraPos, Color color = { 230, 41, 55, 150 });

	void draw(Vector2 cameraPos, Color colorTint = WHITE);
};