#include "MyCamera.h"

MyCamera::MyCamera() : pos({ 0,0 }), speed(500.0f), locked(false) {}
MyCamera::MyCamera(Vector2 pos, float speed, bool locked)
	: pos(pos), speed(speed), locked(locked) {}

Vector2 MyCamera::getPos() { return pos; }
float MyCamera::getSpeed() { return speed; }
bool MyCamera::getLocked() { return locked; }
void MyCamera::setPos(Vector2 pos) { this->pos = pos; }
void MyCamera::setSpeed(float speed) { this->speed = speed; }
void MyCamera::setLocked(bool locked) { this->locked = locked; }

void MyCamera::takeInput()
{
	//Get The Direction.
	Vector2 direction = { 0,0 };
	if (IsKeyDown(KEY_LEFT)) direction.x--;
	if (IsKeyDown(KEY_RIGHT)) direction.x++;
	if (IsKeyDown(KEY_UP)) direction.y--;
	if (IsKeyDown(KEY_DOWN)) direction.y++;
	direction = Vector2Normalize(direction);

	//Calculate The Change Vector.
	Vector2 changeVector = Vector2Scale(direction, speed * GetFrameTime());

	//Update The Camera Position.
	pos = Vector2Add(pos, changeVector);

	//Lock Or Unlock Camera.
	if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_RIGHT)) locked = false;
	if (IsKeyPressed(KEY_L) || IsMouseButtonPressed(3)) locked = !locked;
}

void MyCamera::checkTopDownBound(Vector2 verticalBound)
{
	if (pos.y < verticalBound.x) pos.y = verticalBound.x;
	if (pos.y > verticalBound.y - SCREEN_HEIGHT) pos.y = verticalBound.y - SCREEN_HEIGHT;
}
void MyCamera::checkLeftRightBound(Vector2 horizontalBound)
{
	if (pos.x < horizontalBound.x) pos.x = horizontalBound.x;
	if (pos.x > horizontalBound.y - SCREEN_WIDTH) pos.x = horizontalBound.y - SCREEN_WIDTH;
}
void MyCamera::checkBound(Vector2 verticalBound, Vector2 horizontalBound)
{
	checkTopDownBound(verticalBound);
	checkLeftRightBound(horizontalBound);
}

