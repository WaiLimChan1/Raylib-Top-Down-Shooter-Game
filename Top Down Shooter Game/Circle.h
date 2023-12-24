#pragma once
#include "Header.h"

struct Circle
{
public:
	Vector2 pos;
	float radius;

	Circle() {}
	Circle(Vector2 pos, float radius) : pos(pos), radius(radius) {}

	bool collideWith(Circle other)
	{
		float distance = Vector2Length(Vector2Subtract(this->pos, other.pos));
		return distance < (this->radius + other.radius);
	}
};