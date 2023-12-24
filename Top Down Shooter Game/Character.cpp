#include "Character.h"

Character::Character() {}
Character::Character(Vector2 centerPos, float movementSpeed, bool facingLeft, int maxHealth, float scale, float animationSpeed)
	: pos(centerPos), movementSpeed(movementSpeed), facingLeft(facingLeft), maxHealth(maxHealth), currentHealth(maxHealth), scale(scale), animationSpeed(animationSpeed) 
{
	deathLerpTime = 5;
	timeSinceDeath = 0;
	completelyDead = false;
}

float Character::getWidth() { return current.getWidth(); }
float Character::getHeight() { return current.getHeight(); }
Character::StatusEnum Character::getStatus() { return status; }
Vector2 Character::getPos() { return pos; }
bool Character::getFacingLeft() { return facingLeft; }
int Character::getCurrentHealth() { return currentHealth; }
bool Character::getCompletelyDead() { return completelyDead; }
float Character::getScale() { return scale; }

Vector2 Character::getHitBoxCenterPos()
{
	Rectangle hitbox = getHitBox({ 0,0 });
	return { hitbox.x + hitbox.width / 2, hitbox.y + hitbox.height / 2 };
}
Vector2 Character::getSyncCameraPos()
{
	Vector2 centerPos = { getHitBoxCenterPos().x, getHitBoxCenterPos().y };
	return { centerPos.x - SCREEN_WIDTH / 2, centerPos.y - SCREEN_HEIGHT / 2 };
}

void Character::setPos(Vector2 pos) { this->pos = pos; }
void Character::setHitBoxCenterPos(Vector2 destination)
{
	Vector2 changeVector = Vector2Subtract(destination, getHitBoxCenterPos());
	this->pos = Vector2Add(this->pos, changeVector);
}

Rectangle Character::getHitBox(Vector2 cameraPos) { return { pos.x - cameraPos.x, pos.y - cameraPos.y, getWidth(), getHeight() }; }
void Character::drawHitBox(Vector2 cameraPos, Color color) { DrawRectangleRec(getHitBox(cameraPos), color); }

Circle Character::getCollisionCircle(Vector2 cameraPos) { return Circle({ getHitBoxCenterPos().x - cameraPos.x, getHitBoxCenterPos().y - cameraPos.y }, getWidth() / 4); }
void Character::drawCollisionCircle(Vector2 cameraPos, Color color) { DrawCircleV(getCollisionCircle(cameraPos).pos, getCollisionCircle(cameraPos).radius, color); }

Color Character::lerpColor(Color color1, Color color2, float ratio)
{
	Color result;
	result.r = Lerp(color1.r, color2.r, ratio);
	result.g = Lerp(color1.g, color2.g, ratio);
	result.b = Lerp(color1.b, color2.b, ratio);
	result.a = Lerp(color1.a, color2.a, ratio);

	return result;
}
void Character::drawHealthBar(Vector2 cameraPos, float size, Color lowHealthColor, Color highHealthColor, float borderSize, Color borderColor)
{
	Rectangle healthBar = { getHitBoxCenterPos().x - size / 2.0 - cameraPos.x, pos.y - size / 8.0 - cameraPos.y, size * (float)currentHealth / maxHealth, size / 8.0};
	Rectangle border = healthBar;
	border.x -= borderSize / 2;
	border.y -= borderSize / 2;
	border.width = size;
	border.width += borderSize;
	border.height += borderSize;

	DrawRectangleRec(border, borderColor);
	DrawRectangleRec(healthBar, lerpColor(lowHealthColor, highHealthColor, (float)currentHealth / maxHealth));
}
void Character::draw(Vector2 cameraPos, Color tintColor, Color hitColor)
{
	if (status == DEAD && current.finishedAnimation()) 
		tintColor.a = Lerp(255, 0, timeSinceDeath / deathLerpTime);

	current.draw(Vector2Subtract(pos, cameraPos), facingLeft, tintColor);

	if (!(status == DEAD && current.finishedAnimation())) 
		current.updateFrame();
}

bool Character::isHitByRec(Rectangle attackBox, float damage)
{
	if (currentHealth > 0)
	{
		if (CheckCollisionRecs(attackBox, getHitBox({ 0,0 })))
		{
			currentHealth -= damage;
			if (currentHealth <= 0)
			{
				currentHealth = 0;
				status = DEAD;
			}
			return true;
		}
	}
	return false;
}
bool Character::collideWith(Character* other)
{
	Circle thisCircle = getCollisionCircle({0,0});
	Circle otherCircle = other->getCollisionCircle({ 0,0 });
	if (thisCircle.collideWith(otherCircle))
	{
		Vector2 thisDirection = Vector2Normalize(Vector2Subtract(thisCircle.pos, otherCircle.pos));
		Vector2 otherDirection = Vector2Normalize(Vector2Subtract(otherCircle.pos, thisCircle.pos));

		float distance = Vector2Length(Vector2Subtract(thisCircle.pos, otherCircle.pos));
		float scale = (thisCircle.radius + otherCircle.radius) - distance;
		
		this->pos = Vector2Add(this->pos, Vector2Scale(thisDirection, scale / 2));
		other->pos = Vector2Add(other->pos, Vector2Scale(otherDirection, scale / 2));
		return true;
	}
	return false;
}
void Character::changeAnimation()
{
	if (lastStatus != status)
	{
		if (status == IDLE) current = idle;
		if (status == RUN) current = run;
		if (status == DEAD) current = dead;
	}
	lastStatus = status;
}
void Character::updatePosition()
{
	if (status == RUN)
	{
		Vector2 velocity = Vector2Scale(Vector2Normalize(direction), movementSpeed * GetFrameTime());
		pos = Vector2Add(pos, velocity);
	}
}

void Character::handleDeath()
{
	if (status == DEAD && current.finishedAnimation() && !completelyDead)
	{
		timeSinceDeath += GetFrameTime();
		if (timeSinceDeath >= deathLerpTime) completelyDead = true;
	}
}