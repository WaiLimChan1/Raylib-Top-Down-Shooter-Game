#include "Player.h"

Texture2D Player::StaticIdleTexture;
Texture2D Player::StaticRunTexture;

void Player::LoadTextures()
{
	StaticIdleTexture = LoadTexture("Textures/Characters/Player/idle.png");
	StaticRunTexture = LoadTexture("Textures/Characters/Player/run.png");
}
void Player::UnloadTextures()
{
	UnloadTexture(StaticIdleTexture);
	UnloadTexture(StaticRunTexture);
}

Player::Player() {}
Player::Player(Vector2 centerPos, float movementSpeed, bool facingLeft, int maxHealth, float scale, float animationSpeed)
	: Character(centerPos, movementSpeed, facingLeft, maxHealth, scale, animationSpeed)
{
	lastStatus = IDLE;
	status = IDLE;

	direction = { 0,0 };

	completelyDead = false;

	idle = AnimationPro(StaticIdleTexture, scale, animationSpeed, 4);
	run = AnimationPro(StaticRunTexture, scale, animationSpeed, 7);

	current = idle;

	setHitBoxCenterPos(centerPos);

	float weaponScale = 0.07;

	pistol = new Pistol(getHitBoxCenterPos(), weaponScale);
	assaultRifle = new AssaultRifle(getHitBoxCenterPos(), weaponScale);
	shotGun = new ShotGun(getHitBoxCenterPos(), weaponScale);

	currentWeapon = pistol;
}

std::vector<Bullet*>& Player::getBullets() { return currentWeapon->getBullets(); }

void Player::drawWeapon(Vector2 cameraPos)
{
	currentWeapon->printInfo();
	currentWeapon->draw(cameraPos);
	//currentWeapon->drawLineAndAngle(camera.getPos());
	//currentWeapon->drawHitBox(camera.getPos());
}

void Player::takeInput(Vector2 cameraPos)
{
	if (currentHealth > 0)
	{
		//Movement direction input
		direction = { 0,0 };

		if (IsKeyDown(KEY_W)) direction.y--;
		if (IsKeyDown(KEY_S)) direction.y++;
		if (IsKeyDown(KEY_A)) direction.x--;
		if (IsKeyDown(KEY_D)) direction.x++;

		if (direction.x != 0 || direction.y != 0) status = RUN;
		else status = IDLE;

		//Character direction depending on position of mouse
		Vector2 mousePos = Vector2Add(GetMousePosition(), cameraPos);
		if (mousePos.x < getHitBoxCenterPos().x) facingLeft = true;
		else if (mousePos.x > getHitBoxCenterPos().x) facingLeft = false;

		//Weapon Input
		if (IsKeyPressed(KEY_ONE)) currentWeapon = pistol;
		if (IsKeyPressed(KEY_TWO)) currentWeapon = assaultRifle;
		if (IsKeyPressed(KEY_THREE)) currentWeapon = shotGun;

		//Testing
		if (IsKeyDown(KEY_Q)) currentHealth--;
	}
}

void Player::WeaponLogic(Vector2 cameraPos)
{
	if (currentHealth > 0)
	{
		currentWeapon->setHitBoxCenterPos(getHitBoxCenterPos());
		currentWeapon->calculateAngle(cameraPos);
		currentWeapon->takeInput(cameraPos);
	}
}
void Player::upgradeWeapon(Collectable::CollectableType collectableType)
{
	if (collectableType == Collectable::PISTOL_COLLECTABLE) pistol->setLevel(pistol->getLevelIndex() + 1);
	else if (collectableType == Collectable::ASSAULT_RIFLE_COLLECTABLE) assaultRifle->setLevel(assaultRifle->getLevelIndex() + 1);
	else if (collectableType == Collectable::SHOT_GUN_COLLECTABLE) shotGun->setLevel(shotGun->getLevelIndex() + 1);
}