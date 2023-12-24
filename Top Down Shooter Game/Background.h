#pragma once
#include "Header.h"

class Background
{
private:
	Texture2D texture;
	Vector2 pos;
	Vector2 posOffSet;

	//The Bigger The Distance, The Further Away The Background Is.
	float distance;
	float scale;

public:
	Background();
	Background(Texture2D texutre, Vector2 pos, Vector2 posOffSet, float distance, float scale);

	Vector2 getScaledSize();

	void setPosAboveLine(float y);
	void setPosBelowLine(float y);

	void draw(Vector2 cameraPos, bool removeVerticalShift = false, int backgroundPadding = 2, Color tintColor = WHITE);
	void drawThreeByThree(Vector2 cameraPos, Color tintColor = WHITE);
	void drawSingle(Vector2 cameraPos, Color tintColor = WHITE);

	void unloadTexture();
};

