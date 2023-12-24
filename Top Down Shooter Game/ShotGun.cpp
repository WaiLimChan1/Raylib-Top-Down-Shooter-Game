#include "ShotGun.h"

Texture2D ShotGun::StaticShotGunTexture;
void ShotGun::LoadTextures() { StaticShotGunTexture = LoadTexture("Textures/Weapons/weaponR3.png"); }
void ShotGun::UnloadTextures() { UnloadTexture(StaticShotGunTexture); }

std::vector<Color> ShotGun::colorByLevel = { GRAY, GREEN, BLUE, PURPLE, GOLD };
std::vector<float> ShotGun::damageByLevel = { 25, 50, 75, 100, 150 };
std::vector<float> ShotGun::fireSpeedByLevel = { 0.3, 0.2, 0.1, 0.08, 0.05 };
std::vector<float> ShotGun::reloadTimeByLevel = { 1.3, 1, 0.8, 0.5, 0.2 };
std::vector<int> ShotGun::clipSizeByLevel = { 7, 7, 14, 14, 28 };
std::vector<int> ShotGun::bulletsPerShotByLevel = { 10, 25, 50, 75, 100 };

ShotGun::ShotGun(Vector2 centerPos, float scale)
	: Weapon(centerPos, scale)
{
	texture = StaticShotGunTexture;
	setHitBoxCenterPos(centerPos);

	levelIndex = 0;

	bulletScale = 0.02;

	bulletSpeed = 2000;
	bulletRange = 1000;
	bulletSpread = 30;

	timeSinceLastFire = fireSpeed;

	reloading = false;
	reloadProgress = 0;

	currentClipSize = clipSizeByLevel.at(levelIndex);
	extraAmmo = 0;
	setLevel(levelIndex);
}

void ShotGun::fire(Vector2 cameraPos)
{
	for (int i = 0; i < bulletsPerShot; i++)
	{
		float angleSpread;
		if ((int)bulletSpread == 0) angleSpread = angle;
		else angleSpread = angle + (rand() % (int)bulletSpread - bulletSpread / 2);

		float bulletSpeedSpread = bulletSpeed + (rand() % (int)bulletSpeed/3 - bulletSpeed / 6);

		Vector2 direction = { cos(angleSpread * PI / 180), -sin(angleSpread * PI / 180) };
		Vector2 bulletVelocity = Vector2Scale(direction, bulletSpeedSpread);
		bullets.push_back(new Bullet(bulletScale, color, getHitBoxCenterPos(), bulletVelocity, bulletRange, damage));
	}

	currentClipSize--;
	timeSinceLastFire = 0;
}

void ShotGun::reload()
{

	if (extraAmmo >= 1)
	{
		extraAmmo -= 1;
		currentClipSize += 1;
	}

	if (currentClipSize >= clipSize) reloading = false;
	else reloading = true;

	reloadProgress = 0;
}

void ShotGun::setLevel(int level)
{
	levelIndex = level;
	if (level <= 0) levelIndex = 0;
	if (level > maxLevelIndex) levelIndex = maxLevelIndex;

	color = colorByLevel.at(levelIndex);
	damage = damageByLevel.at(levelIndex);
	fireSpeed = fireSpeedByLevel.at(levelIndex);
	reloadTime = reloadTimeByLevel.at(levelIndex);
	clipSize = clipSizeByLevel.at(levelIndex);
	extraAmmo += 5 * clipSizeByLevel.at(levelIndex);
	bulletsPerShot = bulletsPerShotByLevel.at(levelIndex);
}