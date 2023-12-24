#pragma once
#include "Header.h"
#include "AnimationPro.h"
#include "Circle.h"

class Character
{
public:
	static enum StatusEnum { IDLE, RUN, DEAD };

protected:
	AnimationPro current;

	AnimationPro idle;
	AnimationPro run;
	AnimationPro dead;

	StatusEnum lastStatus; //The last immediate status
	StatusEnum status;

	Vector2 pos;
	Vector2 direction;
	float movementSpeed;

	bool facingLeft;

	int maxHealth;
	int currentHealth;

	bool completelyDead; //Is dead and has finished death lerping.
	float deathLerpTime;
	float timeSinceDeath;

	float scale;
	float animationSpeed;

public:
	Character();
	Character(Vector2 centerPos, float movementSpeed, bool facingLeft, int maxHealth, float scale, float animationSpeed);

	float getWidth();
	float getHeight();
	StatusEnum getStatus();
	Vector2 getPos();
	bool getFacingLeft();
	int getCurrentHealth();
	bool getCompletelyDead();
	float getScale();

	Vector2 getHitBoxCenterPos();
	Vector2 getSyncCameraPos();

	void setPos(Vector2 pos);
	void setHitBoxCenterPos(Vector2 destination);

	Rectangle getHitBox(Vector2 cameraPos);
	void drawHitBox(Vector2 cameraPos, Color color = { 230, 41, 55, 150 });

	Circle getCollisionCircle(Vector2 cameraPos);
	void drawCollisionCircle(Vector2 cameraPos, Color color = { 255, 161, 0, 150 });

	Color lerpColor(Color color1, Color color2, float ratio);
	void drawHealthBar(Vector2 cameraPos, float size, Color lowHealthColor = RED, Color highHealthColor = GREEN, float borderSize = 10, Color borderColor = BLACK);
	void draw(Vector2 cameraPos, Color tintColor = WHITE, Color hitColor = RED);

	virtual void takeInput(Vector2 camerPos) = 0;

	bool isHitByRec(Rectangle attackBox, float damage);
	bool collideWith(Character* other);
	void changeAnimation();
	void updatePosition();

	void handleDeath();
};