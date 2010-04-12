#include "Player.h"
#include "Map.h"
#include <iostream>

using namespace std;

Player * Player::_instance = 0;

Player::Player(void)
{

}

Player::~Player(void)
{
}

float Player::getEyeheight() {
	return eyeheight + yPos;
}

void Player::setHeight(float height) {
	eyeheight = height;
}


void Player::setX(float xIn) {
	xPos = xIn;
}

void Player::setY(float yIn) {
	yPos = yIn;
}

void Player::setZ(float zIn) {
	zPos = zIn;
}

float Player::getX() {
	return xPos;
}

float Player::getY() {
	return yPos;
}

float Player::getZ() {
	return zPos;
}

int * Player::getGridCoords() {
	Map * instance = Map::getInstance();
	return instance->convertWorldCoordToMapCoord(xPos, zPos);
}

float Player::getYrot() {
	return yRotation;
}

float Player::getXrot() {
	return xRotation;
}

void Player::rotate(float xrotIn, float yrotIn) {
	xRotation += xrotIn;
	yRotation += yrotIn;
	fixValues();
}

void Player::fixValues() {
	if (xRotation > 22) {
		xRotation = 22;
	}

	if (xRotation < -40) {
		xRotation = -40;
	}

	if (yRotation < -360) {
		yRotation += 360;
	}

	if (yRotation > 360) {
		yRotation -= 360;
	}
}



void Player::move(float xIn, float yIn, float zIn) {
	xPos += xIn;
	yPos += yIn;
	zPos += zIn;
}

Player * Player::getInstance() {
	if(!_instance) {
		_instance = new Player;
	}
	return _instance;
}
