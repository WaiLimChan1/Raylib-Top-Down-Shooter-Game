#include "Collectable.h"

Collectable::Collectable() {}
Collectable::Collectable(CollectableType collectableType, float scale, Vector2 centerPos)
	: collectableType(collectableType), scale(scale), pos(centerPos)
{
	if (collectableType == PISTOL_COLLECTABLE) texture = Pistol::StaticPistolTexture;
	else if (collectableType == ASSAULT_RIFLE_COLLECTABLE) texture = AssaultRifle::StaticAssaultRifleTexture;
	else if (collectableType == SHOT_GUN_COLLECTABLE) texture = ShotGun::StaticShotGunTexture;
	setHitBoxCenterPos(centerPos);
}

Collectable::CollectableType Collectable::getCollectableType() { return collectableType; }
Vector2 Collectable::getHitBoxCenterPos()
{
	Rectangle hitbox = getHitBox({ 0,0 });
	return { hitbox.x + hitbox.width / 2, hitbox.y + hitbox.height / 2 };
}

void Collectable::setPos(Vector2 pos) { this->pos = pos; }
void Collectable::setHitBoxCenterPos(Vector2 destination)
{
	Vector2 changeVector = Vector2Subtract(destination, getHitBoxCenterPos());
	this->pos = Vector2Add(this->pos, changeVector);
}

Rectangle Collectable::getHitBox(Vector2 cameraPos) { return { pos.x - cameraPos.x, pos.y - cameraPos.y, texture.width * scale, texture.height * scale }; }
void Collectable::drawHitBox(Vector2 cameraPos, Color color) { DrawRectangleRec(getHitBox(cameraPos), color); }

void Collectable::draw(Vector2 cameraPos, Color colorTint)
{
	DrawTextureEx(texture, Vector2Subtract(pos, cameraPos), 0, scale, colorTint);
}