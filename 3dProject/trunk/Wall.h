#pragma once
#include "StaticObject.h"

class Wall: 
	public StaticObject {

public:
	Wall(float xIn, float yIn, float zIn, float inWidth, float inLength, float inHeight) ;
	Wall();
	~Wall(void);
	virtual void draw();
private:
	float width;
	float height;
	float length;
};
