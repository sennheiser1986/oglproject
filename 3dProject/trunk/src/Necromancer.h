#ifndef ARCHVILE
#define ARCHVILE


#include "Md2Model.h"
#include "Hunter.h"
#include "FlightPath.h"


class Necromancer :
	public Hunter
{
public:
	Necromancer(void);
	~Necromancer(void);
	Necromancer(float xIn, float yIn, float zIn);
	virtual void draw();
private:
	MD2Model* model;
	void init();
	clock_t previousAnimTime;
};

#endif