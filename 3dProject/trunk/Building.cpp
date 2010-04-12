#include "Building.h"

using namespace std;
Building::Building(float xIn, float yIn, float zIn, float inWidth, float inLength, float inHeight) 
	: StaticObject(xIn, yIn, zIn)
{
		width = inWidth;
		length = inLength;
		height = inHeight;
		init();
}

Building::Building() {

}

Building::~Building(void)
{
}

void Building::init() {
	float hUnits = width / 20;
	float vUnits = length / 10;

	float x1 = x - width / 2 + 0.5 * hUnits; 
	float x2 = x - width / 2 + 4 * hUnits;
	float x3 = x;
	float x4 = x + width / 2 - 4 * hUnits;
	float x5 = x + width / 2 - 0.5 * hUnits;

	float z1 = z - length / 2 + 0.5 * vUnits;
	float z2 = z;
	float z3 = z + length / 2 - 0.5 * vUnits;

	float width2 = 4 * hUnits;
	float width1 = (width - width2) / 2;
	float width3 = 1 * hUnits;
	float width4 = width;

	float length1 = length / 10;
	float length2 = length - (length / 5);

	Wall wall1 = Wall(x2, y, z1, width1, length1, height);
	Wall wall2 = Wall(x3, y + height/2, z1, width2, length1, height/2);
	Wall wall3 = Wall(x4, y, z1, width1, length1, height);
	Wall wall4 = Wall(x1, y, z2, width3, length2, height);
	Wall wall5 = Wall(x5, y, z2, width3, length2, height);
	Wall wall6 = Wall(x3, y, z3, width4, length1, height);

	walls.push_back(wall1);
	walls.push_back(wall2);
	walls.push_back(wall3);
	walls.push_back(wall4);
	walls.push_back(wall5);
	walls.push_back(wall6);
}

void Building::draw() {
	list<Wall>::iterator it;	
	
	for (it = walls.begin() ; it != walls.end(); it++ ) {
		Wall w = *it;
		w.draw();
	}
}