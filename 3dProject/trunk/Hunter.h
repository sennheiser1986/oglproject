#ifndef HUNTER
#define HUNTER
#include "StaticObject.h"
#include "FlightPath.h"

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
	void draw();
	void moveToPosition(float xIn, float yIn, float zIn);
	void followPath();
	void moveToPlayer();
	


private:
	FlightPath flightPath;
	void calculatePath();
	bool hasToRotate(float degrees);
	void rotate(float degrees);
	void init();
	float x;
	float y;
	float z;
	float yaw;
	float speed;

};

#endif