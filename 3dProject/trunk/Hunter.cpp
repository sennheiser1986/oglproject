#include <iostream>
#include <math.h>
#include <list>
#include "PathFind.h"
#include "Player.h"
#include "Map.h"
#include "Hunter.h"

using namespace std;

Hunter::Hunter(void)
{
	
}

Hunter::~Hunter(void)
{
}

Hunter::Hunter(float xIn, float yIn, float zIn, float rIn): 
	StaticObject(xIn, yIn, zIn, rIn) {
	init();
}

void Hunter::moveToPosition(float xPos, float yPos, float zPos) {
	float diffX = x - xPos;
	float diffY = y - yPos;
	float diffZ = z - zPos;

	float length = sqrt(pow(diffX,2) + pow(diffY,2) + pow(diffZ,2));
	
	float newyaw = atan2(diffX,diffZ) * 180 / PI;
	
	//if(hasToRotate(newyaw)) {
	//		rotate(newyaw);
	//} else {
		float xNorm = diffX / length;
		float zNorm = diffZ / length;
		float yNorm = diffY / length;

		x -= xNorm * speed;
		z -= zNorm * speed;
		y -= yNorm * speed;
	//}	
}

void Hunter::init() {
	   yaw = 0;	  
	   speed = 2;
	   reachedEndOfPath = false;
}

void Hunter::draw() {
	glPushMatrix();
	glTranslatef(x, y, z);
	glutSolidSphere(r, 32, 32);
	glPopMatrix();
}

void Hunter::followPath() {
	if(flightPath.hasWaypoints()) {
		int xPos = flightPath.getX();
		int yPos = flightPath.getY();
		int zPos = flightPath.getZ();

		bool go = true;
		float dst = sqrt(pow((xPos - x),2) + pow((yPos - y),2) + pow((zPos - z),2));
		if(dst <= speed) {
			go = flightPath.next();
			if(! go) {
				reachedEndOfPath = true;
			}
			xPos = flightPath.getX();
			yPos = flightPath.getY();
			zPos = flightPath.getZ();
		}

		if(go) {
			moveToPosition(xPos,yPos,zPos);
		} else {
			Player * playerInstance = Player::getInstance();
			int playX = playerInstance->getX();
			int playY = playerInstance->getY();
			int playZ = playerInstance->getZ();
			if(getDistance(playX, playY, playZ) >= r) {
				flightPath.clear();
				calculatePath();
				followPath();
			}

		}
	} else {
		flightPath.clear();
		calculatePath();
		followPath();
	}
}

void Hunter::calculatePath() {
	PathFind pf = PathFind();
	Player * playerInstance = Player::getInstance();
	Map * mapInstance = Map::getInstance();

	int * playerCoords = playerInstance->getGridCoords();
	int playerRow = playerCoords[0];
	int playerCol = playerCoords[1];

	int * coords = mapInstance->convertWorldCoordToMapCoord(x, z);
	int hunterRow = coords[0];
	int hunterCol = coords[1];

	list<int *> pathList;
	pf.calculatePath(hunterRow, hunterCol, playerRow, playerCol);
	pf.getCalculatedPath(pathList);

	int numWaypoints = pathList.size();
	int * waypoints = new int[3*numWaypoints];
	
	list<int *>::iterator it;	
	int i = 0;
	for (it = pathList.begin() ; it != pathList.end(); it++ ) {
		int * temp = new int[2];
		temp = *it;
		int row = temp[0];
		int col = temp[1];
		int * temp2 = mapInstance->convertMapCoordToWorldCoord(row, col);
		int tempX = temp2[0];
		int tempZ = temp2[1];

		mapInstance->debugMark(row, col, 8);
		int tempY = playerInstance->getY();
		
		waypoints[3 * i + 0] = tempX;
		waypoints[3 * i + 1] = tempY;
		waypoints[3 * i + 2] = tempZ;

		temp2 = mapInstance->convertWorldCoordToMapCoord(tempX, tempZ);
		cout << temp2[0] << " " << temp2[1] << endl;

		i++;
	}

	for(int i = 0; i < numWaypoints; i++) {
		cout << waypoints[i * 3 + 0] << " ";
		cout << waypoints[i * 3 + 1] << " ";
		cout << waypoints[i * 3 + 2] << endl;
	}

	flightPath = FlightPath(waypoints, numWaypoints, false);
	flightPath.printWaypoints();
	mapInstance->writeToFile("kaka.txt");
	*waypoints;
	waypointTime = clock();
}

bool Hunter::hasToRotate(float degrees) {
	float diffRot = yaw - degrees;
	return (abs(diffRot) > 0.1f);
}

void Hunter::rotate(float degrees) {
	float diffRot = yaw - degrees;
	float rotSpeed = speed / 3;

	// ensure shortest rotation
	if(abs(diffRot) > 180) {
		if(yaw > 0) {
			yaw = yaw - 360;
		}
	}

	if(abs(diffRot) > rotSpeed) {		
		if(yaw > degrees) {
			yaw -= rotSpeed;
		} 
		if(yaw < degrees) {
			yaw += rotSpeed;
		}
	} else {
		if(abs(diffRot) > 0.1f * rotSpeed) {
			if(yaw > degrees) {
				yaw -= 0.1f * rotSpeed;
			} 
			if(yaw < degrees) {
				yaw += 0.1f * rotSpeed;
			}
		} else {
			if(abs(diffRot) > 0.1f) {
				if(yaw > degrees) {
					yaw -= 0.1f;
				} 
				if(yaw < degrees) {
					yaw += 0.1f;
				}
			}
		}
	}
}
