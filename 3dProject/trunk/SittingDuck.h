#pragma once
#include "StaticObject.h"

class SittingDuck :
	public StaticObject
{
public:
	SittingDuck(void);
	~SittingDuck(void);
	SittingDuck::SittingDuck(int xIn, int yIn, int zIn, int rIn);
	void SittingDuck::hit();
	void draw();
	float getR();
private:
	float redChan;
	float greenChan;
	float blueChan;
	float r;
};
