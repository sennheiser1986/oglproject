#pragma once

class Player
{
public:
	void setX(float xIn);
	void setY(float yIn);
	void setZ(float zIn);
	void setHeight(float height);
	float getX();
	float getY();
	float getZ();
	float getXrot();
	float getYrot();
	void move(float xIn, float yIn, float zIn);
	void rotate(float xrotIn, float yrotIn);
	int * getGridCoords();
	float getEyeheight();
	static Player * getInstance();

private:
	float xPos;
	float yPos;
	float zPos;
	float eyeheight;
	float xRotation;
	float yRotation;
	
	Player(void);
	~Player(void);

	void Player::fixValues();

	Player(const Player &){};
	Player & operator=(const Player &){};

	static Player * _instance;
};
