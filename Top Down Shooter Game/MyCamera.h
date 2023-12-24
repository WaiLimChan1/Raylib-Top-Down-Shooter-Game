#pragma once
#include "Header.h"

class MyCamera
{
private:
	Vector2 pos;
	float speed;
	bool locked;

public:
	MyCamera();
	MyCamera(Vector2 pos, float speed, bool locked);

	Vector2 getPos();
	float getSpeed();
	bool getLocked();
	void setPos(Vector2 pos);
	void setSpeed(float speed);
	void setLocked(bool locked);

	void takeInput();
	void checkTopDownBound(Vector2 verticalBound);
	void checkLeftRightBound(Vector2 horizontalBound);
	void checkBound(Vector2 verticalBound, Vector2 horizontalBound);
};

