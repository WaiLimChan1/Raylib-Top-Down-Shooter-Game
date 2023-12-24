#include "Bullet.h"

Texture2D Bullet::StaticBulletTexture;
void Bullet::LoadTextures() { StaticBulletTexture = LoadTexture("Textures/Extras/bullet.png"); }
void Bullet::UnloadTextures() { UnloadTexture(StaticBulletTexture); }

Bullet::Bullet(float scale, Color color, Vector2 centerPos, Vector2 vel, float maxRange, float damage)
	: scale(scale), color(color), pos(centerPos), vel(vel), maxRange(maxRange), damage(damage)
{
	texture = StaticBulletTexture;
	setHitBoxCenterPos(centerPos);
	currentRange = 0;
}

float Bullet::getWidth() { return texture.width * scale; }
float Bullet::getHeight() { return texture.height * scale; }
float Bullet::getDamage() { return damage; }
Vector2 Bullet::getHitBoxCenterPos()
{
	Rectangle hitbox = getHitBox({0,0});
	return { hitbox.x + hitbox.width / 2, hitbox.y + hitbox.height / 2 };
}

void Bullet::setPos(Vector2 pos) { this->pos = pos; }
void Bullet::setHitBoxCenterPos(Vector2 destination)
{
	Vector2 changeVector = Vector2Subtract(destination, getHitBoxCenterPos());
	this->pos = Vector2Add(this->pos, changeVector);
}

Rectangle Bullet::getHitBox(Vector2 cameraPos) { return { pos.x - cameraPos.x, pos.y - cameraPos.y, getWidth(), getHeight() }; }
void Bullet::drawHitBox(Vector2 cameraPos, Color color) { DrawRectangleRec(getHitBox(cameraPos), color); }

void Bullet::draw(Vector2 cameraPos)
{
	DrawTextureEx(texture, Vector2Subtract(pos, cameraPos), 0, scale, color);
}

void Bullet::updatePosition()
{
	Vector2 changeVector = Vector2Scale(vel, GetFrameTime());
	pos = Vector2Add(pos, changeVector);
	currentRange += Vector2Length(changeVector);
}
bool Bullet::reachMaxRange() { return currentRange >= maxRange; }