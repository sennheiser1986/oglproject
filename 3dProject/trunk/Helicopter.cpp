#include <math.h>
#include "Helicopter.h"
#define PI 3.14159265

using namespace std;

Helicopter::Helicopter(void)
{

}

Helicopter::~Helicopter(void)
{

}

Helicopter::Helicopter(float xIn, float yIn, float zIn) {
	x = xIn;
	y = yIn;
	z = zIn;

	init();
}

void Helicopter::moveToPosition(float xPos, float yPos, float zPos) {
	float diffX = x - xPos;
	float diffZ = z - zPos;

	float length = sqrt(pow(diffX,2) + pow(diffZ,2));
	
	float newYrot = atan2(diffX,diffZ) * 180 / PI;
	
	if(!(rotate(newYrot))) {
		if(length > 80 || flightPathSet) {

			float xNorm = diffX / length;
			float zNorm = diffZ / length;

			x -= xNorm;
			z -= zNorm;
		}
	}
}

void Helicopter::followFlightPath() {
	float xPos = fp.getX();
	float yPos = fp.getY();
	float zPos = fp.getZ();

	float dst = sqrt(pow((xPos - x),2) + pow((yPos - y),2) + pow((zPos - z),2));
	if(dst < 1) {
		fp.next();
		xPos = fp.getX();
		yPos = fp.getY();
		zPos = fp.getZ();
	}
	moveToPosition(xPos,yPos,zPos);
}

bool Helicopter::rotate(float degrees) {
	cout << "degrees: " << degrees;
	cout << " yrotation: " << yrot << endl;
	float diffRot = yrot - degrees;
	if(abs(diffRot) > 1.0f) {
		if(yrot > degrees) {
			yrot -= 1.0f;
			return true;
		} 
		if(yrot < degrees) {
			yrot += 1.0f;
			return true;
		}
	} else {
		if(abs(diffRot) > 0.1f) {
			if(yrot > degrees) {
				yrot -= 0.1f;
				return true;
			} 
			if(yrot < degrees) {
				yrot += 0.1f;
				return true;
			}
		}
	}
	return false;
}

void Helicopter::draw() {
		glPushMatrix();
	 
		glTranslatef(x, y, z);

		glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
		glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
		glRotatef(yrot, 0.0f, 0.0f, 1.0f);
				
		glScalef(1.0f, 1.0f, 1.0f);
		model->draw();
		
		glPopMatrix();
}

void Helicopter::init() {
	   model = MD2Model::load("helicopter.md2");
	   flightPathSet = false; 
	   yrot = 0;
}

void Helicopter::setFlightPath(FlightPath inFp) {
		flightPathSet = true;
		fp = inFp;
}
