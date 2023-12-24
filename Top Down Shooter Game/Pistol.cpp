#include "Pistol.h"

Texture2D Pistol::StaticPistolTexture;
void Pistol::LoadTextures() { StaticPistolTexture = LoadTexture("Textures/Weapons/weaponR2.png"); }
void Pistol::UnloadTextures() { UnloadTexture(StaticPistolTexture); }

std::vector<Color> Pistol::colorByLevel = { GRAY, GREEN, BLUE, PURPLE, GOLD };
std::vector<float> Pistol::damageByLevel = { 30, 45, 60, 80, 100 };
std::vector<float> Pistol::fireSpeedByLevel = { 0.5, 0.4, 0.3, 0.2, 0.1 };
std::vector<float> Pistol::reloadTimeByLevel = { 1, 0.9, 0.8, 0.7, 0.6};
std::vector<int> Pistol::clipSizeByLevel = { 10, 10, 15, 15, 20 };

Pistol::Pistol(Vector2 centerPos, float scale)
	: Weapon(centerPos, scale)
{
	texture = StaticPistolTexture;
	setHitBoxCenterPos(centerPos);

	levelIndex = 0;

	bulletScale = 0.01;

	bulletSpeed = 1000;
	bulletRange = 1000;
	bulletSpread = 0;

	timeSinceLastFire = fireSpeedByLevel.at(levelIndex);

	reloading = false;
	reloadProgress = 0;

	currentClipSize = clipSizeByLevel.at(levelIndex);
	extraAmmo = 0;
	setLevel(levelIndex);
}

void Pistol::setLevel(int level)
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