#include "AnimationPro.h"

AnimationPro::AnimationPro() {}
AnimationPro::AnimationPro(Texture2D texture, float scale, float updateTime, int totalFrames)
	: texture(texture), scale(scale), updateTime(updateTime), totalFrames(totalFrames)
{
	float frameWidth = texture.width / totalFrames;
	float frameHeight = texture.height;
	source = { 0, 0, frameWidth, frameHeight };

	width = scale * frameWidth;
	height = scale * frameHeight;

	runningTime = 0.0f;
	frame = 0;

	currentFrameJustStarted = true;
}

Texture2D AnimationPro::getTexture() { return texture; }
float AnimationPro::getWidth() { return width; }
float AnimationPro::getHeight() { return height; }
int AnimationPro::getFrame() { return frame; }
int AnimationPro::getTotalFrames() { return totalFrames; }

void AnimationPro::setFrame(int frame) { this->frame = frame; }

void AnimationPro::reScale(float newScale)
{
	this->scale = newScale;

	float frameWidth = texture.width / totalFrames;
	float frameHeight = texture.height;
	width = scale * frameWidth;
	height = scale * frameHeight;
}

void AnimationPro::draw(Vector2 pos, bool flip, Color color)
{
	Rectangle tempSource = source;
	if (flip) tempSource.width *= -1;
	DrawTexturePro(texture, tempSource, { pos.x, pos.y, width, height }, { 0,0 }, 0, color);
}

void AnimationPro::updateFrame()
{
	if (runningTime >= updateTime)
	{
		currentFrameJustStarted = true;

		runningTime = 0;
		if (frame == totalFrames) frame = 0;
		source.x = frame * source.width;
		frame++;
	}
	else
		currentFrameJustStarted = false;
	runningTime += GetFrameTime();
}

bool AnimationPro::startedCurrentFrame() { return currentFrameJustStarted; }
bool AnimationPro::finishedCurrentFrame() { return runningTime >= updateTime; }
bool AnimationPro::finishedAnimation() { return frame == totalFrames && finishedCurrentFrame(); }

void AnimationPro::unloadTexture() { UnloadTexture(texture); }


