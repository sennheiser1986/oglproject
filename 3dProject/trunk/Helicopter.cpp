#include <math.h>
#include "Helicopter.h"

Helicopter::Helicopter(void)
{

}

Helicopter::~Helicopter(void)
{

}

Helicopter::Helicopter(float xIn, float yIn, float zIn) {
	x = xIn;
	y = yIn;
	z = zIn;

	init();
}

void Helicopter::move(float playerX, float playerY, float playerZ) {
	float diffX = x - playerX;
	float diffZ = z - playerZ;

	float length = sqrt(pow(x,2) + pow(z,2));
	float xNorm = x / length;
	float zNorm = z / length;
}

void Helicopter::draw() {
		glPushMatrix();
	 
		glTranslatef(x, 0, z);

		glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
		glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);

		glRotatef(yrot, 0.0f, 0.0f, 1.0f);
		glScalef(0.1f, 0.1f, 0.1f);
		model->draw();
		glPopMatrix();
}

void Helicopter::init() {
	   model = MD2Model::load("helicopter.md2");
}
