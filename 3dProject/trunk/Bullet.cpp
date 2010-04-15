#include "Bullet.h"
#include <iostream>
#include <stdlib.h>

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define PI  3.14159265

using namespace std;

Bullet::Bullet() {}
Bullet::~Bullet() {}
Bullet::Bullet(float xIn, float yIn, float zIn, float xRotIn, float yRotIn)
: StaticObject (xIn, yIn, zIn) {
	xrotrad = xRotIn;
	yrotrad = yRotIn;
	startTime = bulletTime = clock();
	speed = 10;
	maxTime = 8;
	r = 0.1f;
}

	
//http://artis.imag.fr/~Xavier.Decoret/resources/C++/operator==.html
bool Bullet::operator==(const Bullet& other) const {
	//cout << "mine: " << x << "    other: " << other.x << endl;
	return (
		(x == other.x) &&
		(y == other.y) &&
		(z == other.z) &&
		(xrotrad == other.xrotrad) &&
		(yrotrad == other.yrotrad) &&
		(startTime == other.startTime) 
	);
}

bool Bullet::move() {
	bulletTime = clock();
	float time = (float)(bulletTime - startTime) / CLOCKS_PER_SEC;
	if(time > maxTime) {
	
	//cout << "killit" << endl;
		return true;
	} else {
		x += sin(yrotrad) * speed * (time + 0.01);
		z -= cos(yrotrad) * speed * (time + 0.01);
		y -= sin(xrotrad) * speed * (time + 0.01);
		return false;
	}

}


void Bullet::draw() {
	glPushMatrix();
	glTranslatef(x, y, z);
	glutSolidSphere(r, 32, 32);
	glPopMatrix();
}

float Bullet::getR() {
	return r;
}