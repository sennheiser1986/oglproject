#include "SittingDuck.h"

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

SittingDuck::SittingDuck(void)
{
}

SittingDuck::~SittingDuck(void)
{
}

SittingDuck::SittingDuck(int xIn, int yIn, int zIn, int rIn) {
	x = xIn;
	y = yIn;
	z = zIn;
	r = rIn;

	redChan = 0.0f;
	blueChan = 1.0f;
	greenChan = 1.0f;
}

void SittingDuck::draw() {
	glPushMatrix();
	glTranslatef(x, y, z);
	glEnable(GL_COLOR_MATERIAL);
	glColor3f(redChan, greenChan, blueChan);
	glutSolidSphere(r, 32, 32);
	glColor3f(1,1,1);
	glPopMatrix();
}

void SittingDuck::hit() {
	if(redChan == 0) {
		redChan = 1.0f;
		blueChan = 0.0f;
		greenChan = 0.0f;
	} else {
		redChan = 0.0f;
		blueChan = 1.0f;
		greenChan = 1.0f;
	}
}