/* 
 *   3dProject
 *   Geert d'Hoine
 *   (c) 2010
 */ 
#ifndef WALL_H
#define WALL_H

#include "StaticObject.h"

class Wall: 
	public StaticObject {

public:
	Wall(float xIn, float yIn, float zIn, 
		float inWidth, float inLength, 
		float inHeight, int wallTexture,
		bool inMark) ;
	Wall();
	~Wall(void);
	virtual void draw();
private:
	float width;
	float height;
	float length;
	int wallTexture;
	bool mark;
};
#endif