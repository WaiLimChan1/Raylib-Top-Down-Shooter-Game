#include "Header.h"

#include "MyCamera.h"
#include "Background.h"
#include "Zone.h"
#include "Player.h"
#include "Enemy.h"
#include "Collectable.h"

//Global Variables----------------------------------------------------------------------
MyCamera camera;
Background background;
Zone zone;
Texture2D crossHair;

Player player;
std::vector<Bullet*> bullets;

std::vector<Character*> enemies;
int enemyDamage;

float enemySpawnRateOriginal; //Slowest spawn rate at the start of game
float enemySpawnRateFinal; //Fastest spawn rate later on
float timeToReachFinalSpawnRate; //Time to reach fastest spawn rate
float timeSinceLastSpawn;

std::vector<Character*> deadBodies;

std::vector<Collectable*> collectables;
float collectableSpawnRate;
float collectableLastSpawnTime;

float timeSurvived;
int killCount;
//----------------------------------------------------------------------

void LoadTextures()
{
	Player::LoadTextures();
	Enemy::LoadTextures();

	Pistol::LoadTextures();
	AssaultRifle::LoadTextures();
	ShotGun::LoadTextures();
	Bullet::LoadTextures();
}

void SetUpEnemies()
{
	enemyDamage = 1;
	enemySpawnRateOriginal = 1;
	enemySpawnRateFinal = 0.1;
	timeToReachFinalSpawnRate = 60;
	timeSinceLastSpawn = 0;
	enemies.clear();
	deadBodies.clear();
}

void SetUpCollectables()
{
	collectableSpawnRate = 10;
	collectableLastSpawnTime = 0;
	collectables.clear();
}

void Restart()
{
	player = Player({ 0,0 }, 500, false, 1000, 3, 1.0 / 12.0);
	SetUpEnemies();

	timeSurvived = 0;
	killCount = 0;

	SetUpCollectables();
}

void SetUp()
{
	LoadTextures();
	camera = MyCamera({ 0,0 }, 500, true);
	background = Background(LoadTexture("Textures/Environment/ground.png"), { 0,0 }, { 0,0 }, 1, 1);
	zone = Zone(1, 1.5, 2);
	crossHair = LoadTexture("Textures/Extras/crosshair.png");

	Restart();
}

void PrintTimeSurvived(float fontSize = 50, Color textColor = BLACK)
{
	if (player.getCurrentHealth() > 0) timeSurvived += GetFrameTime();
	int time = (int)timeSurvived;

	char message[100];
	float textWidth;
	int posX;
	int posY = fontSize;

	strcpy_s(message, std::to_string(time).c_str());
	textWidth = MeasureText(message, fontSize);
	posX = (SCREEN_WIDTH - textWidth) / 2;
	DrawText(message, posX, posY, fontSize, textColor);
}

void PrintKillCount(float fontSize = 50, Color textColor = BLACK)
{
	char message[100];
	float textWidth;
	int posX;
	int posY = fontSize;

	strcpy_s(message, "Kills: ");
	strcat_s(message, std::to_string(killCount).c_str());
	textWidth = MeasureText(message, fontSize);
	posX = (SCREEN_WIDTH - textWidth) - fontSize;
	DrawText(message, posX, posY, fontSize, textColor);
}


void PrintGameOver(float fontSize = 100, Color textColor = RED)
{
	char message[100];
	float textWidth;
	int posX;
	int posY = (SCREEN_HEIGHT / 2) - fontSize * 3;

	strcpy_s(message, "Game Over");
	textWidth = MeasureText(message, fontSize);
	posX = (SCREEN_WIDTH - textWidth) / 2;
	DrawText(message, posX, posY, fontSize, textColor);
	posY += fontSize;

	strcpy_s(message, "Time Survived: ");
	strcat_s(message, std::to_string((int)timeSurvived).c_str());
	strcat_s(message," Seconds");
	textWidth = MeasureText(message, fontSize);
	posX = (SCREEN_WIDTH - textWidth) / 2;
	DrawText(message, posX, posY, fontSize, textColor);
	posY += fontSize;

	strcpy_s(message, "Kill Count: ");
	strcat_s(message, std::to_string(killCount).c_str());
	textWidth = MeasureText(message, fontSize);
	posX = (SCREEN_WIDTH - textWidth) / 2;
	DrawText(message, posX, posY, fontSize, textColor);
	posY += fontSize;

	posY += fontSize;
	strcpy_s(message, "Press Space To Play Again");
	textWidth = MeasureText(message, fontSize);
	posX = (SCREEN_WIDTH - textWidth) / 2;
	DrawText(message, posX, posY, fontSize, textColor);
}

void Draw()
{
	BeginDrawing();

	ClearBackground(GRAY);

	background.drawThreeByThree(camera.getPos());
	zone.draw(camera.getPos(), player.getHitBoxCenterPos());

	for (auto& deadBodies : deadBodies) deadBodies->draw(camera.getPos());

	for (auto& enemy : enemies) enemy->draw(camera.getPos());
	for (auto& enemy : enemies)
	{
		enemy->drawHealthBar(camera.getPos(), 100);
		//enemy->drawHitBox(camera.getPos());
		//enemy->drawCollisionCircle(camera.getPos());
	}

	for (auto& collectable : collectables)
	{
		collectable->draw(camera.getPos());
		//collectable->drawHitBox(camera.getPos());
	}

	player.draw(camera.getPos());
	player.drawHealthBar(camera.getPos(), 100);
	player.drawWeapon(camera.getPos());
	//player.drawHitBox(camera.getPos());
	//player.drawCollisionCircle(camera.getPos());

	for (auto& bullet : bullets)
	{
		bullet->draw(camera.getPos());
		//bullet->drawHitBox(camera.getPos());
	}

	if (player.getCurrentHealth() > 0) 
	{
		PrintTimeSurvived();
		PrintKillCount();
	}
	else
		PrintGameOver();

	DrawTextureEx(crossHair, Vector2Subtract(GetMousePosition(), {(float)(crossHair.width / 2.0), (float)(crossHair.height / 2.0)}), 0, 1, BLACK);
	EndDrawing();
}

void Input()
{
	player.takeInput(camera.getPos());
	if (player.getCurrentHealth() <= 0) if (IsKeyPressed(KEY_SPACE)) Restart();
	for (auto& enemy : enemies) { enemy->takeInput(player.getHitBoxCenterPos()); }
	camera.takeInput();
}

void EnemyLogic()
{
	//Spawn Enemies
	float dt = GetFrameTime();
	timeSinceLastSpawn += dt;

	float spawnRateTime = Lerp(enemySpawnRateOriginal, enemySpawnRateFinal, timeSurvived / timeToReachFinalSpawnRate);
	if (spawnRateTime < enemySpawnRateFinal) spawnRateTime = enemySpawnRateFinal;

	if (timeSinceLastSpawn >= spawnRateTime)
	{
		Vector2 pos = zone.getSpawnZonePoint();
		float movementSpeed = 250;
		bool facingLeft = false;
		int maxHealth = 100;
		float scale = 3;
		float animationSpeed = 1.0 / 12.0;
		enemies.push_back(new Enemy(pos, movementSpeed, facingLeft, maxHealth, scale, animationSpeed));

		timeSinceLastSpawn = 0;
	}

	//Update enemy
	for (auto& enemy : enemies)
	{
		enemy->changeAnimation();
		enemy->updatePosition();
	}

	//Check player collision by enemy, take damage
	for (auto& enemy : enemies)
	{
		if (enemy->getCurrentHealth() > 0)
			//if (player.isHitByRec(enemy->getHitBox({ 0,0 }), enemyDamage));
			if (player.getCollisionCircle({ 0,0 }).collideWith(enemy->getCollisionCircle({ 0,0 })))
				player.isHitByRec(player.getHitBox({ 0,0 }), enemyDamage);
	}

	//Delete enemy that surpassed the deletion zone
	for (int enemyIndex = 0; enemyIndex < enemies.size(); enemyIndex++)
	{
		if (!CheckCollisionPointRec(enemies.at(enemyIndex)->getHitBoxCenterPos(), zone.getDeleteZone()))
		{
			delete enemies.at(enemyIndex);
			enemies.erase(enemies.begin() + enemyIndex);
			enemyIndex--;
		}
	}
}

void BulletLogic()
{
	//Get the bullets into the bullets vector
	for (auto& bullet : player.getBullets()) bullets.push_back(bullet);
	player.getBullets().clear();

	//Check bullet collision with enemies and remove bullets that struck enemies
	for (int bulletIndex = 0; bulletIndex < bullets.size(); bulletIndex++)
	{
		for (int enemyIndex = 0; enemyIndex < enemies.size(); enemyIndex++)
		{
			if (enemies.at(enemyIndex)->isHitByRec(bullets.at(bulletIndex)->getHitBox({ 0,0 }), bullets.at(bulletIndex)->getDamage()))
			{
				delete bullets.at(bulletIndex);
				bullets.erase(bullets.begin() + bulletIndex);
				bulletIndex--;
				break;
			}
		}
	}

	//Move bullet. Delete it if it reached max range or if bullet has surpassed the delete zone
	for (int bulletIndex = 0; bulletIndex < bullets.size(); bulletIndex++)
	{
		bullets.at(bulletIndex)->updatePosition();
		if (bullets.at(bulletIndex)->reachMaxRange() || //Delete bullet if bullet reached max range
			!CheckCollisionPointRec(bullets.at(bulletIndex)->getHitBoxCenterPos(), zone.getDeleteZone())) //Delete bullet if bullet surpassed the delete zone
		{
			delete bullets.at(bulletIndex);
			bullets.erase(bullets.begin() + bulletIndex);
			bulletIndex--;
		}
	}
}

void DeadBodyLogic()
{
	//Add dead enemies to deadBodies vector
	for (int enemyIndex = 0; enemyIndex < enemies.size(); enemyIndex++)
	{
		if (enemies.at(enemyIndex)->getCurrentHealth() <= 0)
		{
			enemies.at(enemyIndex)->changeAnimation(); //Make sure they are in the dead animation
			killCount++;
			deadBodies.push_back(enemies.at(enemyIndex));
			enemies.erase(enemies.begin() + enemyIndex);
			enemyIndex--;
		}
	}

	//Make dead bodies fade away
	for (auto& deadBody : deadBodies) deadBody->handleDeath();

	//Remove completely-faded-away-bodies and out-of-bound-bodies
	for (int i = 0; i < deadBodies.size(); i++)
	{
		if (deadBodies.at(i)->getCompletelyDead() || //Delete dead body if it faded away
			!CheckCollisionPointRec(deadBodies.at(i)->getHitBoxCenterPos(), zone.getDeleteZone())) //Delete dead body if dead body surpassed the delete zone
		{
			delete deadBodies.at(i);
			deadBodies.erase(deadBodies.begin() + i);
			i--;
		}
	}
}

void checkBlockingCollision()
{
	std::vector<Character*> allCharacters;
	allCharacters = enemies;
	if (player.getCurrentHealth() > 0) allCharacters.push_back(&player);

	for (int i = 0; i < allCharacters.size() - 1; i++)
		for (int j = i + 1; j < allCharacters.size(); j++)
			allCharacters.at(i)->collideWith(allCharacters.at(j));

	allCharacters.clear();
}

void CollectableLogic()
{
	collectableLastSpawnTime += GetFrameTime();
	if (collectableLastSpawnTime >= collectableSpawnRate)
	{
		Vector2 pos = zone.getSpawnZonePoint();
		float scale = 0.07;
		int randNum = rand() % 3;

		if (randNum == 0) collectables.push_back(new Collectable(Collectable::PISTOL_COLLECTABLE, scale, pos));
		if (randNum == 1) collectables.push_back(new Collectable(Collectable::ASSAULT_RIFLE_COLLECTABLE, scale, pos));
		if (randNum == 2) collectables.push_back(new Collectable(Collectable::SHOT_GUN_COLLECTABLE, scale, pos));
		collectableLastSpawnTime = 0;
	}

	//Collide with player, player collects the collectable and then delete the collectable.
	for (int i = 0; i < collectables.size(); i++)
	{
		if (CheckCollisionPointRec(player.getHitBoxCenterPos(), collectables.at(i)->getHitBox({0,0}))) //Hitting player
		{
			player.upgradeWeapon(collectables.at(i)->getCollectableType());
			delete collectables.at(i);
			collectables.erase(collectables.begin() + i);
			i--;
		}
	}

	//Remove out of bound collectables
	for (int i = 0; i < collectables.size(); i++)
	{
		if (!CheckCollisionPointRec(collectables.at(i)->getHitBoxCenterPos(), zone.getDeleteZone())) //Delete out of bound collectables
		{
			delete collectables.at(i);
			collectables.erase(collectables.begin() + i);
			i--;
		}
	}
}

void Logic()
{
	player.changeAnimation();
	player.updatePosition();
	player.WeaponLogic(camera.getPos());

	EnemyLogic();
	BulletLogic();
	DeadBodyLogic();
	checkBlockingCollision();

	CollectableLogic();

	if (camera.getLocked()) camera.setPos(player.getSyncCameraPos());
}

void ReleaseResources()
{
	background.unloadTexture();
	UnloadTexture(crossHair);

	Player::UnloadTextures();
	Enemy::UnloadTextures();

	Pistol::UnloadTextures();
	AssaultRifle::UnloadTextures();
	ShotGun::UnloadTextures();
	Bullet::UnloadTextures();
}

int main()
{
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Top Down Shooter Game");
	SetWindowState(FLAG_VSYNC_HINT);
	srand(time(0));

	SetUp();
	while (!WindowShouldClose())
	{
		Draw();
		Input();
		if (player.getCurrentHealth() > 0)
			Logic();
	}
	ReleaseResources();

	return 0;
}