#include <math.h>
#include "StaticObject.h"

StaticObject::StaticObject(void)
{
}

StaticObject::~StaticObject(void)
{
}

StaticObject::StaticObject(float xIn, float yIn, float zIn) {
	x = xIn;
	y = yIn;
	z = zIn;
}

float StaticObject::getX() {
	return x;
}

float StaticObject::getY() {
	return y;
}

float StaticObject::getZ() {
	return z;
}

float StaticObject::getR() {
	return r;
}

void StaticObject::setX(float xIn) {
	x = xIn;
}

void StaticObject::setY(float yIn) {
	y = yIn;
}

void StaticObject::setZ(float zIn) {
	z = zIn;
}

void StaticObject::setR(float rIn) {
	r = rIn;
}



float StaticObject::getDistance(float xIn, float yIn, float zIn) {
	return sqrt(pow((xIn - x),2) + pow((yIn - y),2) + pow((zIn - z),2)) - r;	
}