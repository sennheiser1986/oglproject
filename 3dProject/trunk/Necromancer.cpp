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
: Hunter(xIn, yIn, zIn, 0) {
	init();
}

void Necromancer::init() {
	model = MD2Model::load("usmc.md2");
	model->setAnimation("run");
	previousAnimTime = clock();
	speed = 1;
}

void Necromancer::draw() {
	glPushMatrix();
	glTranslatef(x, y+8.5, z);
	glRotatef(-90, 1, 0, 0);
	glRotatef(yaw + 90, 0.0f, 0.0f, 1.0f);
	glScalef(0.35f, 0.35f, 0.35f);
	clock_t currentTime = clock();
	float timeDiff = (float)(currentTime - previousAnimTime) / CLOCKS_PER_SEC;
	cout << timeDiff << endl;
	model->advance(timeDiff);
	previousAnimTime = currentTime;
	model->draw();
	glPopMatrix();
}