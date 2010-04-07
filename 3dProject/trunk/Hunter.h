#pragma once
#include "StaticObject.h"
#include "FlightPath.h"

#define PI 3.14159265

class Hunter :
	public StaticObject
{
public:
	Hunter(void);
	~Hunter(void);
	Hunter(float xIn, float yIn, float zIn);
	void draw();
	void Hunter::moveToPosition(float xIn, float yIn, float zIn);
	void Hunter::followPath();
	void Hunter::moveToPlayer();
	


private:
	FlightPath flightPath;
	void Hunter::calculatePath();
	bool Hunter::hasToRotate(float degrees);
	void Hunter::rotate(float degrees);
	float x;
	float y;
	float z;
	float yaw;
	float speed;

};
