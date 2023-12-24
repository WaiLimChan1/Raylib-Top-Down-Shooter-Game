#include "Weapon.h"

Weapon::Weapon(Vector2 centerPos, float scale)
	: pos(centerPos), scale(scale) { maxLevelIndex = 4; }

float Weapon::getWidth() { return texture.width * scale; }
float Weapon::getHeight() { return texture.height * scale; }
float Weapon::getScale() { return scale; }
Vector2 Weapon::getPos() { return pos; }
float Weapon::getAngle() { return angle; }
int Weapon::getLevelIndex() { return levelIndex; }
std::vector<Bullet*>& Weapon::getBullets() { return bullets; }

Vector2 Weapon::getHitBoxCenterPos()
{
	Rectangle hitbox = getHitBox({ 0,0 });
	return { hitbox.x + hitbox.width / 2, hitbox.y + hitbox.height / 2 };
}

void Weapon::setPos(Vector2 pos) { this->pos = pos; }
void Weapon::setHitBoxCenterPos(Vector2 destination)
{
	Vector2 changeVector = Vector2Subtract(destination, getHitBoxCenterPos());
	this->pos = Vector2Add(this->pos, changeVector);
}

Rectangle Weapon::getHitBox(Vector2 cameraPos) { return { pos.x - cameraPos.x, pos.y - cameraPos.y, getWidth(), getHeight() }; }
void Weapon::drawHitBox(Vector2 cameraPos, Color color) { DrawRectangleRec(getHitBox(cameraPos), color); }

void Weapon::printInfo()
{
	char message[100];
	float fontSize = 30;
	Color textColor = BLACK;
	int posX = 10;
	int posY = 10;

	strcpy_s(message, "Clip: ");
	strcat_s(message, std::to_string(currentClipSize).c_str());
	strcat_s(message, "/");
	strcat_s(message, std::to_string(clipSize).c_str());
	DrawText(message, posX, posY, fontSize, textColor);
	posY += fontSize;

	strcpy_s(message, "Ammo: ");
	strcat_s(message, std::to_string(extraAmmo).c_str());
	DrawText(message, posX, posY, fontSize, textColor);
	posY += fontSize;
	posY += fontSize;

	if (reloading)
	{
		strcpy_s(message, "Reloading... ");
		DrawText(message, posX, posY, fontSize, textColor);
		posY += fontSize;

		float circleRadius = 30;
		DrawCircleSector({ posX + circleRadius, posY + circleRadius}, circleRadius, 0, Lerp(0, 360, reloadProgress / reloadTime), 90, GRAY);
	}
}

void Weapon::calculateDrawTextureParameters(Vector2 cameraPos)
{
	source = { 0,0,(float)texture.width, (float)texture.height };

	Vector2 texturePos = Vector2Subtract(getHitBoxCenterPos(), cameraPos);
	dest = { texturePos.x,texturePos.y,getWidth(), getHeight() };

	origin = { getWidth() / 2, getHeight() * 3 / 4 };

	textureRotation = angle * -1;
	if (angle > 90 && angle < 270)
	{
		source.height *= -1;
		origin = { getWidth() / 2, getHeight() * 1 / 4 };
	}
}
void Weapon::draw(Vector2 cameraPos)
{
	calculateDrawTextureParameters(cameraPos);
	DrawTexturePro(texture, source, dest, origin, textureRotation, color);
	//DrawRectanglePro(dest, origin, textureRotation, { 230, 41, 55, 150 });
}
void Weapon::drawLineAndAngle(Vector2 cameraPos)
{
	Vector2 centerPos = Vector2Subtract(getHitBoxCenterPos(), cameraPos);
	DrawLineV(centerPos, GetMousePosition(), PURPLE);
	DrawCircleSector(centerPos, 25, 90, 90 + angle, 90, GREEN);
}

Vector2 Weapon::calculateDirection(Vector2 cameraPos)
{
	Vector2 centerPos = Vector2Subtract(getHitBoxCenterPos(), cameraPos);
	Vector2 direction = Vector2Normalize(Vector2Subtract(GetMousePosition(), centerPos));
	return direction;
}
void Weapon::calculateAngle(Vector2 cameraPos)
{
	Vector2 direction = calculateDirection(cameraPos);

	direction.y *= -1;
	angle = atan2(direction.y, direction.x) * 180 / PI;
	if (direction.y <= 0) angle += 360;
}

void Weapon::takeInput(Vector2 cameraPos)
{
	float dt = GetFrameTime();
	timeSinceLastFire += dt;
	if (timeSinceLastFire > fireSpeed) timeSinceLastFire = fireSpeed;

	if (IsMouseButtonDown(0) && timeSinceLastFire >= fireSpeed && currentClipSize > 0)
	{
		if (reloading)
		{
			reloading = false;
			reloadProgress = 0;
		}
		fire(cameraPos);
	}
	if (IsMouseButtonDown(0) && currentClipSize == 0 && extraAmmo > 0 && !reloading) reloading = true;
	if (IsKeyPressed(KEY_R) && currentClipSize < clipSize && extraAmmo > 0 && !reloading) reloading = true;
	if (reloading)
	{
		reloadProgress += dt;
		if (reloadProgress >= reloadTime) reload();
	}
}

void Weapon::fire(Vector2 cameraPos)
{
	float angleSpread;
	if ((int) bulletSpread == 0) angleSpread = angle;
	else angleSpread = angle + (rand() % (int)bulletSpread - bulletSpread / 2);

	Vector2 direction = { cos(angleSpread * PI / 180), -sin(angleSpread * PI / 180) };
	Vector2 bulletVelocity = Vector2Scale(direction, bulletSpeed);
	bullets.push_back(new Bullet(bulletScale, color, getHitBoxCenterPos(), bulletVelocity, bulletRange, damage));
	currentClipSize--;
	timeSinceLastFire = 0;
}

void Weapon::reload()
{
	extraAmmo += currentClipSize;
	currentClipSize = 0;

	if (extraAmmo >= clipSize)
	{
		extraAmmo -= clipSize;
		currentClipSize = clipSize;
	}
	else
	{
		currentClipSize = extraAmmo;
		extraAmmo = 0;
	}

	reloading = false;
	reloadProgress = 0;
}