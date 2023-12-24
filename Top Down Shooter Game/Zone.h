#pragma once
#include "Header.h"

class Zone
{
private:
	Rectangle playZone; //Kept the same
	Rectangle spawnZone; //Spawning things
	Rectangle deleteZone; //Deleting things beyond this rectangle

public:
	Zone();
	Zone(float playZoneRatio, float spawnZoneRatio, float deleteZoneRatio);

	Rectangle getPlayZone();
	Rectangle getSpawnZone();
	Rectangle getDeleteZone();

	//Get a point that is within spawnZone but beyond playZone
	Vector2 getSpawnZonePoint();

	Vector2 getCenterPos(Rectangle zone);
	void setCenterPos(Rectangle& zone, Vector2 destination);

	void syncZonesWithPos(Vector2 centerPos);

	void draw(Vector2 cameraPos, Vector2 centerPos, Color playZoneColor = GREEN, Color spawnZoneColor = RED, Color deleteZoneColor = BLACK);
};