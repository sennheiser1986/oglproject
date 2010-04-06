#pragma once
#include "StaticObject.h"

class Hunter :
	public StaticObject
{
public:
	Hunter(void);
	~Hunter(void);
	Hunter(float xIn, float yIn, float zIn);
	void draw();
	void Hunter::move();
};
