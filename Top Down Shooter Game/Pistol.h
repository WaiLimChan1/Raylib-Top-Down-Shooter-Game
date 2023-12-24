#pragma once
#include "Weapon.h"

class Pistol : public Weapon
{
public:
	static Texture2D StaticPistolTexture;

private:
	static std::vector<Color> colorByLevel;
	static std::vector<float> damageByLevel;
	static std::vector<float> fireSpeedByLevel;
	static std::vector<float> reloadTimeByLevel;
	static std::vector<int> clipSizeByLevel;

public:
	static void LoadTextures();
	static void UnloadTextures();

	Pistol(Vector2 centerPos, float scale);
	void setLevel(int levelIndex) override;
};