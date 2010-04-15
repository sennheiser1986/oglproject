#ifndef HELICOPTER
#define HELICOPTER

#include "StaticObject.h"
#include "FlightPath.h"
#include "Md2Model.h"

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


#define PITCH_FORWARD -20.0f
#define PITCH_LEVEL 0.0f
#define PITCH_BACKWARD 20.0f

class Helicopter :
	public StaticObject
{

public:
	Helicopter(void);
	~Helicopter(void);
	
	Helicopter(float xIn, float yIn, float zIn);
	void moveToPosition(float xPos, float yPos, float zPos);
	void draw();
	void rotate(float degrees);	
	void setFlightPath(FlightPath inFp);
	void followFlightPath();
private:
	bool flightPathSet;
	float pitch;
	float yaw;
	int speed;
	FlightPath fp;
	MD2Model* model;
	void init();
	void pitchMoveForward();
	void pitchMoveBackward();
	void pitchLevel();
	void pitchDegrees(float degrees);	
	bool hasToRotate(float degrees);
	bool hasToChangePitch(float degrees);
};


#endif