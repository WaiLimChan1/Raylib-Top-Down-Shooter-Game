#pragma once
#include "Weapon.h"

class ShotGun : public Weapon
{
public:
	static Texture2D StaticShotGunTexture;

private:
	static std::vector<Color> colorByLevel;
	static std::vector<float> damageByLevel;
	static std::vector<float> fireSpeedByLevel;
	static std::vector<float> reloadTimeByLevel;
	static std::vector<int> clipSizeByLevel;
	static std::vector<int> bulletsPerShotByLevel;

	int bulletsPerShot;

public:
	static void LoadTextures();
	static void UnloadTextures();

	ShotGun(Vector2 centerPos, float scale);

	void fire(Vector2 cameraPos) override;
	void reload() override;
	void setLevel(int levelIndex) override;
};
