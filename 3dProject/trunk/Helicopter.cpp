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
	
	float newyaw = atan2(diffX,diffZ) * 180 / PI;
	
	if(hasToRotate(newyaw)) {
		if(hasToChangePitch(PITCH_LEVEL)) {
			pitchLevel();
		} else {
			rotate(newyaw);
		}
	} else {
		if(length > 80 || flightPathSet) {
			if(hasToChangePitch(PITCH_FORWARD)) {
				pitchMoveForward();
			} else {
				float xNorm = diffX / length;
				float zNorm = diffZ / length;

				x -= xNorm;
				z -= zNorm;
			}
		} else {
			if(hasToChangePitch(PITCH_LEVEL)) {
				pitchLevel();
			}
		}
	}
}

void Helicopter::pitchDegrees(float degrees) {
	if(pitch < degrees - 1.0f) {
		pitch++;
	}

	if(pitch < degrees - 0.1f) {
		pitch += 0.1f;
	}

	if(pitch > degrees + 1.0f) {
		pitch--;
	}
	if(pitch > degrees + 0.1f) {
		pitch -= 0.1f;
	}
}

bool Helicopter::hasToChangePitch(float degrees) {
	float diff = pitch - degrees;
	return (abs(diff) > 0.1f);
}

void Helicopter::pitchLevel() {
	pitchDegrees(PITCH_LEVEL);
}

void Helicopter::pitchMoveForward() {
	pitchDegrees(PITCH_FORWARD);
}

void Helicopter::pitchMoveBackward() {
	pitchDegrees(PITCH_BACKWARD);
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

bool Helicopter::hasToRotate(float degrees) {
	float diffRot = yaw - degrees;
	return (abs(diffRot) > 0.1f);
}

void Helicopter::rotate(float degrees) {
	float diffRot = yaw - degrees;

	// ensure shortest rotation
	if(abs(diffRot) > 180) {
		if(yaw > 0) {
			yaw = yaw - 360;
		}
	}

	if(abs(diffRot) > 1.0f) {		
		if(yaw > degrees) {
			yaw -= 1.0f;
		} 
		if(yaw < degrees) {
			yaw += 1.0f;
		}
	} else {
		if(abs(diffRot) > 0.1f) {
			if(yaw > degrees) {
				yaw -= 0.1f;
			} 
			if(yaw < degrees) {
				yaw += 0.1f;
			}
		}
	}
}

void Helicopter::draw() {
		cout << x << " " << y << " " << z << " " << yaw  << " " << pitch << " " << endl;
		glPushMatrix();
		float yawrad = yaw / 180 * PI;

		glTranslatef(x, y, z);
		glRotatef(pitch, cos(yawrad), 0.0f, -sin(yawrad));
		glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
		glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
		glRotatef(yaw, 0.0f, 0.0f, 1.0f);
				
		glScalef(1.0f, 1.0f, 1.0f);
		model->draw();
		
		glPopMatrix();
}

void Helicopter::init() {
	   model = MD2Model::load("helicopter.md2");
	   flightPathSet = false; 
	   yaw = 0;
	   pitch = 0;
}

void Helicopter::setFlightPath(FlightPath inFp) {
		flightPathSet = true;
		fp = inFp;
}
