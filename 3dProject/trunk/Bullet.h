#include "StaticObject.h"
#include <ctime>
#include <math.h>

class Bullet: public StaticObject {
	private: 
		float speed;
		float xrotrad;
		float yrotrad;
		float maxTime;
		clock_t startTime;
		clock_t bulletTime;

	public:
		Bullet();
		~Bullet();

		Bullet(float xIn, float yIn, float zIn, float xRotIn, float yRotIn);
		bool Bullet::operator==(const Bullet& other) const;
		bool move();
		virtual void draw();
};
