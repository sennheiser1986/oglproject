#include "Necromancer.h"

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
	model = MD2Model::load("necromancer.md2");
	model->setAnimation("run");
}

void Necromancer::draw() {
	glPushMatrix();
	glTranslatef(x, y+8.5, z);
	glRotatef(-90, 1, 0, 0);
	glRotatef(yaw + 90, 0.0f, 0.0f, 1.0f);
	glScalef(0.35f, 0.35f, 0.35f);
	model->draw();
	glPopMatrix();
}