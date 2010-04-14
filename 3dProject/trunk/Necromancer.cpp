#include <iostream>
#include "Necromancer.h"

using namespace std;

Necromancer::Necromancer(void)
{
}

Necromancer::~Necromancer(void)
{
}

Necromancer::Necromancer(float xIn, float yIn, float zIn) 
: Hunter(xIn, yIn, zIn) {
	init();
}

void Necromancer::init() {
	model = MD2Model::load("usmc.md2");
	model->setAnimation("run");
	previousAnimTime = clock();
	speed = 2;
	setHeight(15);
	setWidth(10);
}

void Necromancer::draw() {
	glPushMatrix();
	glTranslatef(x, y+8.5, z);

	if(isHit()) {
		glEnable(GL_COLOR_MATERIAL);
		glColor3f(1.0f, 0.0f, 0.0f);
		glutSolidSphere(getWidth(), 32,32);
		glColor3f(1.0f, 1.0f, 1.0f);
		glDisable(GL_COLOR_MATERIAL);
	} else {

		glRotatef(-90, 1, 0, 0);
		glRotatef(yaw + 90, 0.0f, 0.0f, 1.0f);
		glScalef(0.35f, 0.35f, 0.35f);
		
		clock_t currentTime = clock();
		float timeDiff = (float)(currentTime - previousAnimTime) / CLOCKS_PER_SEC;
		model->advance(timeDiff);
		previousAnimTime = currentTime;
		
		model->draw();
	}
	glPopMatrix();
}
