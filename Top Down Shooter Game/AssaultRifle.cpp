#include "AssaultRifle.h"

Texture2D AssaultRifle::StaticAssaultRifleTexture;
void AssaultRifle::LoadTextures() { StaticAssaultRifleTexture = LoadTexture("Textures/Weapons/weaponR1.png"); }
void AssaultRifle::UnloadTextures() { UnloadTexture(StaticAssaultRifleTexture); }

std::vector<Color> AssaultRifle::colorByLevel = { GRAY, GREEN, BLUE, PURPLE, GOLD };
std::vector<float> AssaultRifle::damageByLevel = { 50, 75, 100, 150, 200 };
std::vector<float> AssaultRifle::fireSpeedByLevel = { 0.05, 0.04, 0.03, 0.02, 0.01 };
std::vector<float> AssaultRifle::reloadTimeByLevel = { 3, 2.5, 2, 1.5, 1 };
std::vector<int> AssaultRifle::clipSizeByLevel = { 30, 60, 90, 150, 200 };

AssaultRifle::AssaultRifle(Vector2 centerPos, float scale)
	: Weapon(centerPos, scale)
{
	texture = StaticAssaultRifleTexture;
	setHitBoxCenterPos(centerPos);

	levelIndex = 0;

	bulletScale = 0.02;

	bulletSpeed = 1500;
	bulletRange = 1500;
	bulletSpread = 10;

	timeSinceLastFire = fireSpeed;

	reloading = false;
	reloadProgress = 0;

	currentClipSize = clipSizeByLevel.at(levelIndex);
	extraAmmo = 0;
	setLevel(levelIndex);
}

void AssaultRifle::setLevel(int level)
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
}