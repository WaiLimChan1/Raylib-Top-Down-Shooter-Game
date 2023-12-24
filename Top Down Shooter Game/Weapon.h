#pragma once
#include "Header.h"
#include "Bullet.h"

class Weapon
{
protected:
	Texture2D texture;
	Rectangle source;
	Rectangle dest;
	Vector2 origin;
	float textureRotation;

	float scale;
	Color color;

	Vector2 pos;

	float angle;

	std::vector<Bullet*> bullets;

	int maxLevelIndex;
	int levelIndex;

	float bulletScale;

	float damage;
	float bulletSpeed;
	float bulletRange;
	float bulletSpread; //In degrees

	float fireSpeed;
	float timeSinceLastFire;

	bool reloading;
	float reloadTime;
	float reloadProgress;

	int clipSize;
	int currentClipSize;
	int extraAmmo;

public:
	Weapon(Vector2 centerPos, float scale);

	virtual void setLevel(int levelIndex) = 0;

	float getWidth();
	float getHeight();
	float getScale();
	Vector2 getPos();
	float getAngle();
	int getLevelIndex();
	std::vector<Bullet*>& getBullets();

	Vector2 getHitBoxCenterPos();

	void setPos(Vector2 pos);
	void setHitBoxCenterPos(Vector2 destination);

	Rectangle getHitBox(Vector2 cameraPos);
	void drawHitBox(Vector2 cameraPos, Color color = { 230, 41, 55, 150 });

	void printInfo();

	void calculateDrawTextureParameters(Vector2 cameraPos);
	void draw(Vector2 cameraPos);
	void drawLineAndAngle(Vector2 cameraPos);

	Vector2 calculateDirection(Vector2 cameraPos);
	void calculateAngle(Vector2 cameraPos);
	void takeInput(Vector2 cameraPos);
	virtual void fire(Vector2 cameraPos);
	virtual void reload();
};