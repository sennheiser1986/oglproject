/* 
 *   3dProject
 *   Geert d'Hoine
 *   (c) 2010
 */ 
#ifndef STATIC_OBJECT
#define STATIC_OBJECT

class StaticObject
{
protected:
	//coordinates and radius
	float x;
	float y;
	float z;
public:
	StaticObject(void);
	~StaticObject(void);
	StaticObject(float xIn, float yIn, float zIn);

	virtual void draw() = 0;
	float getX();
	float getY();
	float getZ();
	void setX(float xIn);
	void setY(float yIn);
	void setZ(float zIn);
	float getDistance(float xIn, float yIn, float zIn);
	float getHorizontalDistance(float xIn, float yIn, float zIn);
	float getVerticalDistance(float xIn, float yIn, float zIn) ;
};

#endif