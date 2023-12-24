#include "Background.h"

Background::Background() {}
Background::Background(Texture2D texture, Vector2 pos, Vector2 posOffSet, float distance, float scale)
	: texture(texture), pos(pos), posOffSet(posOffSet), distance(distance), scale(scale)
{
	this->pos = Vector2Add(pos, posOffSet);
	if (distance == 0) distance = 1;
}

Vector2 Background::getScaledSize() { return { texture.width * scale, texture.height * scale }; }

void Background::setPosAboveLine(float y) { pos.y = (y - getScaledSize().y) + posOffSet.y; }
void Background::setPosBelowLine(float y) { pos.y = y + posOffSet.y; }

//Several of the same background texture is used to create the illusion that the background spans infinitely. 
//backgroundPadding determines the number of background textures before and after the main background texture.
void Background::draw(Vector2 cameraPos, bool removeVerticalShift, int backgroundPadding, Color tintColor)
{
	Vector2 distancedCameraPos = { cameraPos.x / distance, cameraPos.y / distance };
	if (removeVerticalShift) distancedCameraPos.y *= distance;
	float scaledWidth = texture.width * scale;

	//If camera position touches or passes the right end of the original background, reset background's x position
	if (distancedCameraPos.x >= scaledWidth + pos.x)
		pos.x = distancedCameraPos.x;

	//If camera position touches or passes the left end of the background that is immediately left of the original background, reset background's x position
	if (distancedCameraPos.x <= -scaledWidth + pos.x)
		pos.x = distancedCameraPos.x;

	//Draw Consecutive background textures starting with the leftmost one.
	Vector2 temp = { pos.x - backgroundPadding * scaledWidth, pos.y };
	for (int i = 0; i < backgroundPadding * 2 + 1; i++)
	{
		DrawTextureEx(texture, { temp.x - (int)distancedCameraPos.x, temp.y - (int)distancedCameraPos.y }, 0, scale, tintColor);
		temp.x += scaledWidth;
	}
}
void Background::drawThreeByThree(Vector2 cameraPos, Color tintColor)
{
	Vector2 scaledSize = getScaledSize();

	if ((cameraPos.x >= scaledSize.x + pos.x) ||
		(cameraPos.x <= -scaledSize.x + pos.x))
		pos.x = cameraPos.x;

	if ((cameraPos.y >= scaledSize.y + pos.y) ||
		(cameraPos.y <= -scaledSize.y + pos.y))
		pos.y = cameraPos.y;

	Vector2 tempPos = { pos.x - scaledSize.x, pos.y - scaledSize.y };
	for (int row = 0; row < 3; row++)
	{
		for (int column = 0; column < 3; column++)
		{
			DrawTextureEx(texture, Vector2Subtract(tempPos, cameraPos), 0, scale, tintColor);
			tempPos.x += scaledSize.x;
		}
		tempPos.x = pos.x - scaledSize.x;
		tempPos.y += scaledSize.y;
	}
}
void Background::drawSingle(Vector2 cameraPos, Color tintColor)
{
	DrawTextureEx(texture, Vector2Subtract(pos, cameraPos), 0, scale, tintColor);
}

void Background::unloadTexture() { UnloadTexture(texture); }

