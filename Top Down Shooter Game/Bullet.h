#pragma once
#include"Header.h"

class Bullet
{
protected:
	static Texture2D StaticBulletTexture;

private:
	Texture2D texture;
	float scale;
	Color color;

	Vector2 pos;
	Vector2 vel;
	float maxRange;
	float currentRange;

	float damage;

public:
	static void LoadTextures();
	static void UnloadTextures();

	Bullet(float scale, Color color, Vector2 centerPos, Vector2 vel, float maxRange, float damage);

	float getWidth();
	float getHeight();
	float getDamage();
	Vector2 getHitBoxCenterPos();

	void setPos(Vector2 pos);
	void setHitBoxCenterPos(Vector2 destination);

	Rectangle getHitBox(Vector2 cameraPos);
	void drawHitBox(Vector2 cameraPos, Color color = { 230, 41, 55, 150 });

	void draw(Vector2 cameraPos);

	void updatePosition();
	bool reachMaxRange();
};