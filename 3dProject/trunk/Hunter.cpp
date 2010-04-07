#include "Hunter.h"
#include "Player.h"
#include "Map.h"
#include <iostream>
#include <math.h>
#include <list>
#include "PathFind.h"

using namespace std;

Hunter::Hunter(void)
{
}

Hunter::~Hunter(void)
{
}

Hunter::Hunter(float xIn, float yIn, float zIn) {
	x = xIn;
	y = yIn;
	z = zIn;
}

void Hunter::draw() {

}

void Hunter::move() {
	PathFind pf = PathFind();
	Player * playerInstance = Player::getInstance();
	Map * mapInstance = Map::getInstance();

	int * playerCoords = playerInstance->getGridCoords();
	int playerY = playerCoords[0];
	int playerX = playerCoords[1];

	int * coords = mapInstance->convertWorldCoordToMapCoord(x, z);
	int hunterX = coords[0];
	int hunterY = coords[1];

	list<int *> pathList;
	pf.calculatePath(hunterX, hunterY, playerX, playerY);
	pf.getCalculatedPath(pathList);
}
