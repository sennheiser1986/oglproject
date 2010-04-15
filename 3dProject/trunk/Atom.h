#pragma once
#include "StaticObject.h"
#include <ctime>

class Atom :
	public StaticObject {
private:
	int numElectrons;
	// electrons per shell <=  2 * [(shellNumber) ^ 2]
	int *radius;
	int *speed;
	float *shellRChan;
	float *shellGChan;
	float *shellBChan;
	int *shellNumber;
	int *phase;
	int *rotation;
	int *vecNum;
	bool *direction;
	float cosScale;
	float sinScale;
	clock_t startTime;
	clock_t atomTime;
	float r;
	
	void Atom::init();

public:	
	Atom();
	Atom(float xIn, float yIn, float zIn, int numElec);
	~Atom();
	virtual void draw();
};