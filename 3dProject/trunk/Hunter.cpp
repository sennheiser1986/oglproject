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

Hunter::Hunter(float xIn, float yIn, float zIn) {
	x = xIn;
	y = yIn;
	z = zIn;

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
	   speed = 0.5;
}

void Hunter::draw() {
	glPushMatrix();
	glTranslatef(x, y, z);
	glutSolidSphere(5.0f, 32, 32);
	glPopMatrix();
}

void Hunter::followPath() {
	if(flightPath.hasWaypoints()) {
		int xPos = flightPath.getX();
		int yPos = flightPath.getY();
		int zPos = flightPath.getZ();

		float dst = sqrt(pow((xPos - x),2) + pow((yPos - y),2) + pow((zPos - z),2));
		if(dst <= speed) {
			flightPath.next();
			xPos = flightPath.getX();
			yPos = flightPath.getY();
			zPos = flightPath.getZ();
		}
		
		moveToPosition(xPos,yPos,zPos);
	
	}	else {
		calculatePath();
		followPath();
	}	
}

void Hunter::calculatePath() {
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

	int numWaypoints = pathList.size();
	int * waypoints = new int[3*numWaypoints];
	int * waypointRef = waypoints; 
	
	list<int *>::iterator it;	
	for (it = pathList.begin() ; it != pathList.end(); it++ ) {
		int * temp = new int[2];
		temp = *it;
		int col = temp[1];
		int row = temp[0];
		mapInstance->mark(row, col,0);
		int * temp2 = mapInstance->convertMapCoordToWorldCoord(row, col);
		int tempX = temp2[0];
		int tempZ = temp2[1];

		
		int tempY = playerInstance->getY();
		
		*waypointRef = tempX;
		//cout << waypointRef << " " << *waypointRef << endl;
		waypointRef++;
		*waypointRef = tempY;
		//cout << waypointRef << " " << *waypointRef << endl;
		waypointRef++;
		*waypointRef = tempZ;		
		//cout << waypointRef << " " << *waypointRef << endl;
		//cout << endl;
		waypointRef++;
	}

	//for(int i = 0; i < numWaypoints; i++) {
		//cout << waypoints[3*i]  << ","
		//	<< waypoints[3*i+1] << ","
		//	<< waypoints[3*i+2] << endl;		
	//}

	mapInstance->writeToFile("hunterr.txt");
	flightPath = FlightPath(waypoints, numWaypoints);

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
