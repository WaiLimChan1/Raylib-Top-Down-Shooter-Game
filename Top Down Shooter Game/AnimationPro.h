#pragma once
#include "Header.h"

class AnimationPro
{
private:
	Texture2D texture;
	Rectangle source;

	float scale;
	float width;
	float height;

	float updateTime;
	float runningTime;

	int frame;
	int totalFrames;

	bool currentFrameJustStarted;

public:
	AnimationPro();
	AnimationPro(Texture2D texture, float scale, float updateTime, int totalFrames);

	Texture2D getTexture();
	float getWidth();
	float getHeight();
	int getFrame();
	int getTotalFrames();

	void setFrame(int frame);

	void reScale(float newScale);

	void draw(Vector2 pos, bool flip, Color color = WHITE);
	void updateFrame();

	bool startedCurrentFrame();
	bool finishedCurrentFrame();
	bool finishedAnimation();

	void unloadTexture();
};

