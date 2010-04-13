#ifndef HUNTER
#define HUNTER
#include "StaticObject.h"
#include "FlightPath.h"
#include "PathFind.h"
#include <ctime>

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define PI 3.14159265

class Hunter :
	public StaticObject
{
public:
	Hunter(void);
	~Hunter(void);
	Hunter(float xIn, float yIn, float zIn);
	virtual void draw();
	void moveToPosition(float xIn, float yIn, float zIn);
	void followPath();
	void moveToPlayer();
	bool collidesWith(float xIn, float yIn, float zIn, float rIn);
	void setHeight(float inHeight);
	void setWidth(float inWidth);
	virtual void setHit();
	bool isHit();
	float getWidth();
	float getHeight();


private:
	FlightPath * flightPath;
	PathFind pf;
	void calculatePath();
	bool hasToRotate(float degrees);
	void rotate(float degrees);
	void init();	
	void resetPath();
	bool reachedEndOfPath;
	clock_t waypointTime;
	clock_t vanishTime;
	bool playerVisible;
	bool playerVanished;
	bool playerCaught;
	bool useWaypoints;
	float playerLastSeenX;
	float playerLastSeenY;
	float playerLastSeenZ;
	float width;
	float height;
	bool hit;
protected:
	float yaw;
	float speed;

};

#endif