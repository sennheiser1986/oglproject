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


#define PITCH_FORWARD -45.0f
#define PITCH_LEVEL 0.0f
#define PITCH_BACKWARD 45.0f

class Helicopter :
	public StaticObject
{

public:
	Helicopter(void);
	~Helicopter(void);
	
	Helicopter(float xIn, float yIn, float zIn);
	void Helicopter::moveToPosition(float xPos, float yPos, float zPos);
	void Helicopter::draw();
	void Helicopter::rotate(float degrees);	
	void Helicopter::setFlightPath(FlightPath inFp);
	void Helicopter::followFlightPath();
private:
	bool flightPathSet;
	float pitch;
	float yaw;
	FlightPath fp;
	MD2Model* model;
	void Helicopter::init();
	void Helicopter::pitchMoveForward();
	void Helicopter::pitchMoveBackward();
	void Helicopter::pitchLevel();
	void Helicopter::pitchDegrees(float degrees);	
	bool Helicopter::hasToRotate(float degrees);
	bool Helicopter::hasToChangePitch(float degrees);
};


#endif