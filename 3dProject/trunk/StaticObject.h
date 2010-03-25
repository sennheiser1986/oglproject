#ifndef STATIC_OBJECT
#define STATIC_OBJECT

class StaticObject
{
protected:
	//coordinates and radius
	float x;
	float y;
	float z;
	float r;
public:
	StaticObject(void);
	~StaticObject(void);
	StaticObject(float xIn, float yIn, float zIn);

	void draw();
	float getX();
	float getY();
	float getZ();
	float getR();
	void setX(float xIn);
	void setY(float yIn);
	void setZ(float zIn);
	void setR(float rIn);
	float getDistance(float xIn, float yIn, float zIn);
};

#endif