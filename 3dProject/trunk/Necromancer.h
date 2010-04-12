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
	
	void draw();
private:
	MD2Model* model;
	void init();
};

#endif