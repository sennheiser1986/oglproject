#ifndef PLAYER_H
#define PLAYER_H

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
	bool isHit();
	void setHit(bool inHit);

private:
	bool hit;
	float xPos;
	float yPos;
	float zPos;
	float eyeheight;
	float xRotation;
	float yRotation;
	
	Player(void);
	~Player(void);

	void fixValues();


	static Player * _instance;
};

#endif