#ifndef HELICOPTER
#define HELICOPTER

#include "StaticObject.h"
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
	void Helicopter::move(float playerX, float playerY, float playerZ);
	void Helicopter::draw();
private:
	float xrot;
	float yrot;
	MD2Model *model;
	void Helicopter::init();
};


#endif