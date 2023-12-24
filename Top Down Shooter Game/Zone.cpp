#include "Zone.h"

Zone::Zone() {}
Zone::Zone(float playZoneRatio, float spawnZoneRatio, float deleteZoneRatio)
{
	playZone = { 0,0,SCREEN_WIDTH * playZoneRatio, SCREEN_HEIGHT * playZoneRatio };
	spawnZone = { 0,0,SCREEN_WIDTH * spawnZoneRatio, SCREEN_HEIGHT * spawnZoneRatio };
	deleteZone = { 0,0,SCREEN_WIDTH * deleteZoneRatio, SCREEN_HEIGHT * deleteZoneRatio };
}

Rectangle Zone::getPlayZone() { return playZone; }
Rectangle Zone::getSpawnZone() { return spawnZone; }
Rectangle Zone::getDeleteZone() { return deleteZone; }

Vector2 Zone::getSpawnZonePoint()
{
	Vector2 randomPoint;
	do
	{
		randomPoint.x = rand() % (int)spawnZone.width + spawnZone.x;
		randomPoint.y = rand() % (int)spawnZone.height + spawnZone.y;
	} while (!CheckCollisionPointRec(randomPoint, spawnZone) || CheckCollisionPointRec(randomPoint, playZone));

	return randomPoint;
}

Vector2 Zone::getCenterPos(Rectangle zone) { return { zone.x + zone.width / 2, zone.y + zone.height / 2 }; }
void Zone::setCenterPos(Rectangle& zone, Vector2 destination)
{
	Vector2 changeVector = Vector2Subtract(destination, getCenterPos(zone));
	zone.x += changeVector.x;
	zone.y += changeVector.y;
}

void Zone::syncZonesWithPos(Vector2 centerPos)
{
	setCenterPos(playZone, centerPos);
	setCenterPos(spawnZone, centerPos);
	setCenterPos(deleteZone, centerPos);
}

void Zone::draw(Vector2 cameraPos, Vector2 centerPos, Color playZoneColor, Color spawnZoneColor, Color deleteZoneColor)
{
	syncZonesWithPos(centerPos);
	DrawRectangleLines(playZone.x - cameraPos.x, playZone.y - cameraPos.y, playZone.width, playZone.height, playZoneColor);
	DrawRectangleLines(spawnZone.x - cameraPos.x, spawnZone.y - cameraPos.y, spawnZone.width, spawnZone.height, spawnZoneColor);
	DrawRectangleLines(deleteZone.x - cameraPos.x, deleteZone.y - cameraPos.y, deleteZone.width, deleteZone.height, deleteZoneColor);
}