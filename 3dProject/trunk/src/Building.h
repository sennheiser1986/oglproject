#ifndef BUILDING_H
#define BUILDING_H

#include "StaticObject.h"
#include "Wall.h"
#include <list>

class Building :
	public StaticObject
{
public:
	Building(float xIn, float yIn, float zIn, float inWidth, float inLength, float inHeight, int inTexture);
	~Building(void);
	Building();

	virtual void draw();
private:

	void init();

	float length;
	float width;
	float height;
	int texture;

	std::list<Wall> walls;
};

#endif