#include "Wall.h"

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

Wall::Wall() {

}

Wall::Wall(float xIn, float yIn, float zIn, float inWidth, float inLength, float inHeight) 
	: StaticObject(xIn, yIn, zIn)
{
	width = inWidth;
	length = inLength;
	height = inHeight;
}

Wall::~Wall(void)
{
}

void Wall::draw() {
	glPushMatrix();
	glTranslatef(x, y + height/2, z);
	glScalef(width / 10, height / 10, length / 10);
	glutSolidCube(10);
	glPopMatrix();
}
