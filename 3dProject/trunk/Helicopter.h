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




class Helicopter :
	public StaticObject
{

public:
	Helicopter(void);
	~Helicopter(void);
	
	Helicopter(float xIn, float yIn, float zIn);
	void Helicopter::moveToPosition(float xPos, float yPos, float zPos);
	void Helicopter::draw();
	bool Helicopter::rotate(float degrees);	
	void Helicopter::setFlightPath(FlightPath inFp);
	void Helicopter::followFlightPath();
private:
	bool flightPathSet;
	float xrot;
	float yrot;
	FlightPath fp;
	MD2Model* model;
	void Helicopter::init();
};


#endif