/* 
 *   3dProject
 *   Geert d'Hoine
 *   (c) 2010
 */ 
#include <cstdlib>
#include <cmath>
#include "Helicopter.h"
#define PI 3.14159265


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
	float diffY = y - yPos;
	float diffZ = z - zPos;

	float length = sqrt(pow(diffX,2) + pow(diffY,2) + pow(diffZ,2));
	
	float newyaw = atan2(diffX,diffZ) * 180 / PI;
	
	if(hasToRotate(newyaw)) { //rotate before moving
		if(hasToChangePitch(PITCH_LEVEL)) {
			pitchLevel();
		} else {
			rotate(newyaw);
		}
	} else {
		if(length > 80 || flightPathSet) { // only move if waypoint is far enough
			if(hasToChangePitch(PITCH_FORWARD)) { //make nose tilt forward
				pitchMoveForward();
			} else {
				float xNorm = diffX / length; //do the actual move
				float zNorm = diffZ / length;
				float yNorm = diffY / length;

				x -= xNorm * speed;
				z -= zNorm * speed;
				y -= yNorm * speed;
			}
		} else {
			if(hasToChangePitch(PITCH_LEVEL)) { //we arent moving,
				pitchLevel();                   //so we level the nose
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
	if(dst <= speed) {
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
	float rotSpeed = speed / 3;

	// ensure shortest rotation
	if(abs(diffRot) > 180) {
		if(yaw > 0) {
			yaw = yaw - 360;
		}
	}

	if(abs(diffRot) > rotSpeed) {		
		if(yaw > degrees) {
			yaw -= rotSpeed;
		} 
		if(yaw < degrees) {
			yaw += rotSpeed;
		}
	} else {
		if(abs(diffRot) > 0.1f * rotSpeed) {
			if(yaw > degrees) {
				yaw -= 0.1f * rotSpeed;
			} 
			if(yaw < degrees) {
				yaw += 0.1f * rotSpeed;
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
}

void Helicopter::draw() {
		glPushMatrix();
		float yawrad = yaw / 180 * PI;

		glTranslatef(x, y, z);
		glRotatef(pitch, cos(yawrad), 0.0f, -sin(yawrad));
		glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
		glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
		glRotatef(yaw, 0.0f, 0.0f, 1.0f);
		glScalef(1.0f, 1.0f, 1.0f);
		model->draw();
		
		glBegin(GL_QUADS);


		glTranslatef(0.0f, 0.0f, 10.0f);

		float height = 20.0f;
		float width = 20.0f;
		float length = 60.0f;

		glScalef(length, width, height);
		glutSolidCube(1.0);

		glPopMatrix();
}

void Helicopter::init() {
	   model = MD2Model::load("helicopter.md2");
	   flightPathSet = false; 
	   yaw = 0;
	   pitch = 0;
	   speed = 10;
}

void Helicopter::setFlightPath(FlightPath inFp) {
		flightPathSet = true;
		fp = inFp;
}
