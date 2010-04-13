#include <iostream>
#include <math.h>
#include <list>
#include "Player.h"
#include "Map.h"
#include "Hunter.h"
#include "vec3f.h"

using namespace std;

Hunter::Hunter(void)
{
	
}

Hunter::~Hunter(void)
{
}

Hunter::Hunter(float xIn, float yIn, float zIn): 
	StaticObject(xIn, yIn, zIn) {
	init();
}

void Hunter::moveToPlayer() {
	Player * playerInstance = Player::getInstance();
	float xPos = playerInstance->getX();
	float yPos = playerInstance->getY();
	float zPos = playerInstance->getZ();
	moveToPosition(xPos, yPos, zPos);
}

void Hunter::moveToPosition(float xPos, float yPos, float zPos) {
	float diffX = x - xPos;
	float diffY = y - yPos;
	float diffZ = z - zPos;

	float length = sqrt(pow(diffX,2) + pow(diffY,2) + pow(diffZ,2));
	
	float newyaw = atan2(diffX,diffZ) * 180 / PI;
	
	if(hasToRotate(newyaw)) {
			rotate(newyaw);
	} //else {
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
	   pf = PathFind();
	   useWaypoints = true;
	   playerVanished = false;
	   playerVisible = false;
	   playerCaught = false;
	   flightPath =  new FlightPath();
	   hit = false;
}

void Hunter::draw() {
	glPushMatrix();
	glTranslatef(x, y, z);
	float r = 10;
	glScalef(r,2*r,r);
	//glutSolidSphere(r, 32, 32);
	glEnable(GL_COLOR_MATERIAL);
	glColor3f(1.0f, 0.1f, 0.1f);

	glBegin(GL_QUADS);
	glVertex3f(-1,0,-1);
	glVertex3f(1,0,-1);
	glVertex3f(1,0,1);
	glVertex3f(-1,0,1);
	glEnd();

	
	//Vec3f vecCenter = Vec3f(0, 1, 0);
	//Vec3f vecUL = Vec3f(-1, 0, -1);
	//Vec3f vecUR = Vec3f(1, 0, -1);
	//Vec3f vecLR = Vec3f(1, 0, 1);
	//Vec3f vecLL = Vec3f(-1, 0, 1);

	//Vec3f diff1 = vecUL - vecCenter;
	//Vec3f diff2 = vecUR - vecCenter;
	//Vec3f diff3 = vecLR - vecCenter;
	//Vec3f diff4 = vecLR - vecCenter;

	//Vec3f cross1 = diff1.cross(diff2);
	//Vec3f cross2 = diff2.cross(diff3);
	//Vec3f cross3 = diff3.cross(diff4);
	//Vec3f cross4 = diff4.cross(diff1);


	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0,1,0);
	//glNormal3f(cross1[0], cross1[1], cross1[2]);
	glVertex3f(-1,0,-1);
	//glNormal3f(cross2[0], cross2[1], cross2[2]);
	glVertex3f(1,0,-1);
	//glNormal3f(cross3[0], cross3[1], cross3[2]);
	glVertex3f(1,0,1);
	//glNormal3f(cross4[0], cross4[1], cross4[2]);
	glVertex3f(-1,0,1);
	glEnd();

	glPopMatrix();

	glColor3f(1.0f, 1.0f, 1.0f);
	glDisable(GL_COLOR_MATERIAL);
}

float Hunter::getHeight() {
	return height;
}

float Hunter::getWidth() {
	return width;
}

void Hunter::followPath() {
	if(!hit) {
		Player * playerInstance = Player::getInstance();
		Map * mapInstance = Map::getInstance();
		if(getDistance(playerInstance->getX(), playerInstance->getY(), playerInstance->getZ()) <= speed) {
			playerCaught = true;
		}
		if(!playerCaught) {
			bool reset = false;			
			
			int * playerCoords = playerInstance->getGridCoords();
			int playerRow = playerCoords[0];
			int playerCol = playerCoords[1];

			int * coords = mapInstance->convertWorldCoordToMapCoord(x, z);
			int hunterRow = coords[0];
			int hunterCol = coords[1];

			if(pf.existStraightPath(hunterRow, hunterCol, playerRow, playerCol)) {
				playerVisible = true;
				useWaypoints  = false;
				playerLastSeenX = playerInstance->getX();
				playerLastSeenY = playerInstance->getY();
				playerLastSeenZ = playerInstance->getZ();
				moveToPlayer();
			} else {
				if(playerVisible) {
					playerVanished = true;
				}
				playerVisible = false;
			}
			if(playerVanished) {
				if(getDistance(playerLastSeenX, playerLastSeenY, playerLastSeenZ) <= speed) {
					useWaypoints = true;
					reset = true;
				} else {
					moveToPosition(playerLastSeenX, playerLastSeenY, playerLastSeenZ);
				}
			}
			if(useWaypoints) {
				cout << flightPath->hasWaypoints() << " " << reset << endl;
				if(flightPath->hasWaypoints() && (!reset)) {
					int xPos = flightPath->getX();
					int yPos = flightPath->getY();
					int zPos = flightPath->getZ();

					bool go = true;
					float dst = sqrt(pow((xPos - x),2) + pow((yPos - y),2) + pow((zPos - z),2));
					if(dst <= speed) {
						go = flightPath->next();
						if(! go) {
							reachedEndOfPath = true;
						}
						xPos = flightPath->getX();
						yPos = flightPath->getY();
						zPos = flightPath->getZ();
					}

					if(go) {
						moveToPosition(xPos,yPos,zPos);
					} else {
						int playX = playerInstance->getX();
						int playY = playerInstance->getY();
						int playZ = playerInstance->getZ();
						if(getDistance(playX, playY, playZ) >= speed) {
							reset = true;
						}

					}
				} else {
					reset = false;
					playerVanished = false;
					resetPath();
					followPath();
				}
			}
		}
	}
}

void Hunter::resetPath() {
	flightPath->clear();
	calculatePath();
}

void Hunter::calculatePath() {
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

		int tempY = playerInstance->getY();
		
		waypoints[3 * i + 0] = tempX;
		waypoints[3 * i + 1] = tempY;
		waypoints[3 * i + 2] = tempZ;

		i++;
	}

	flightPath = new FlightPath(waypoints, numWaypoints, false);
	waypointTime = clock();
}

bool Hunter::hasToRotate(float degrees) {
	float diffRot = yaw - degrees;
	return (abs(diffRot) > 0.1f);
}

void Hunter::rotate(float degrees) {
	float diffRot = yaw - degrees;
	float rotSpeed = speed * 3;

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

bool Hunter::collidesWith(float xIn, float yIn, float zIn, float rIn) {
	bool collision = false;
	if(width > 0 && height > 0) {
		float horiDist = getHorizontalDistance(xIn, yIn, zIn) - rIn - width/2;
		float vertiDist = getVerticalDistance(xIn, yIn, zIn) - rIn - height;
		if(horiDist < 0 && vertiDist < 0)  {
			collision = true;
		}
	}
	return collision;
}

void Hunter::setWidth(float inWidth) {
	width = inWidth;
}

void Hunter::setHeight(float inHeight) {
	height = inHeight;
}

void Hunter::setHit() {
	hit = true;
}

bool Hunter::isHit() {
	return hit;
}