#include "StaticObject.h"
#include "Bunker.h"
#include "Map.h"
#include <math.h>
#include <iostream>

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

using namespace std;

Bunker::Bunker(float wIn, float hIn, float xIn, float yIn, float zIn, int* inTex) {
	w = wIn;
	h = hIn;
	x = xIn;
	y = yIn;
	z = zIn;
	r = sqrt(2* pow(w/2,2));
	textures = inTex;

	cout << x << " " << z << " " << x-w/2 << " " << x+w/2 <<  " " << z-w/2 << " " << z+w/2;

	Map * instance = Map::getInstance();
	
	int * minMapCoord = instance->convertWorldCoordToMapCoord(x-w/2,z-w/2);
	int minRow = minMapCoord[0];
	int minCol = minMapCoord[1];

	int * maxMapCoord = instance->convertWorldCoordToMapCoord(x+w/2,z+w/2);
	int maxRow = maxMapCoord[0];
	int maxCol = maxMapCoord[1];

	int width = maxCol - minCol;
	int height = maxRow - minRow;

	instance->markBlock(minRow, minCol, height, width, 9);

}

float Bunker::getWidth() {
	return w;
}

float Bunker::getHeight() {
	return h;
}

void Bunker::changeTexture(int texture, int index) {
	textures[index] = texture;
}

Bunker::Bunker() {};
Bunker::~Bunker() {};

void Bunker::draw() {
	// index: 0 => front, 1 => right, 2 => back, 3 => left,

	int _textureFront, _textureRight, _textureBack, _textureLeft;
	_textureFront = textures[0];
	_textureRight = textures[1];
	_textureBack = textures[2];
	_textureLeft = textures[3];

	glPushMatrix();
	glTranslatef(x, y + (h / 2), z);
	//glRotatef(0.0f, 1.0f, 0.0f, 0.0f);
	//glRotatef(0.0f, 0.0f, 1.0f, 0.0f);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);  
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER,0.1f); 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	
	//Top face	
	//glBindTexture(GL_TEXTURE_2D, _textureTop);		
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glBegin(GL_QUADS);
	//glNormal3f(0.0, 1.0f, 0.0f);
	//glTexCoord2f(0.0f, 1.0f);
	//glVertex3f(-HORI_SIZE / 2, VERTI_SIZE / 2, -HORI_SIZE / 2);
	//glTexCoord2f(0.0f, 0.0f);
	//glVertex3f(-HORI_SIZE / 2, VERTI_SIZE / 2, HORI_SIZE / 2);
	//glTexCoord2f(1.0f, 1.0f);
	//glVertex3f(HORI_SIZE / 2, VERTI_SIZE / 2, HORI_SIZE / 2);
	//glTexCoord2f(1.0f, 1.0f);
	//glVertex3f(HORI_SIZE / 2, VERTI_SIZE / 2, -HORI_SIZE / 2);
	
	//Bottom face
	glNormal3f(0.0, -1.0f, 0.0f);
	glVertex3f(-w / 2, -h / 2, -w / 2);
	glVertex3f(w / 2, -h / 2, -w / 2);
	glVertex3f(w / 2, -h / 2, w / 2);
	glVertex3f(-w / 2, -h / 2, w / 2);
	
	//Left face
	glEnd();
	glBindTexture(GL_TEXTURE_2D, _textureLeft);		
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	
	glBegin(GL_QUADS);

	glNormal3f(1.0, 0.0f, 0.0f);
	//  x  y  z
	// -1 -1 -1 = bottom left
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-w / 2, -h / 2, -w / 2);
	// -1 -1  1 = bottom right
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-w / 2, -h / 2, w / 2);
	// -1  1  1 = top right
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-w / 2, h / 2, w / 2);
	// -1  1 -1 = top left
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-w / 2, h / 2, -w / 2);
	
	//Right face
	glEnd();
	glBindTexture(GL_TEXTURE_2D, _textureRight);		
	//glTexParameteri(GL_TEzxxXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBegin(GL_QUADS);

	glNormal3f(-1.0, 0.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	// x  y  z
	// 1 -1 -1  = bottom right
	glVertex3f(w / 2, -h / 2, -w / 2);
	glTexCoord2f(1.0f, 1.0f);
	// 1  1 -1  = top right
	glVertex3f(w / 2, h / 2, -w / 2);
	glTexCoord2f(0.0f, 1.0f);
	// 1  1  1  = top left
	glVertex3f(w / 2, h / 2, w / 2);
	glTexCoord2f(0.0f, 0.0f);
	// 1 -1  1  = bottom left
	glVertex3f(w / 2, -h / 2, w / 2);
		
	glEnd();
	//Front face
	glBindTexture(GL_TEXTURE_2D, _textureFront);		
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBegin(GL_QUADS);

	glNormal3f(0.0, 0.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f);
	// -1 -1  1 = bottom left
	glVertex3f(-w / 2, -h / 2, w / 2);
	glTexCoord2f(1.0f, 0.0f);
	//  1 -1  1 = bottom right
	glVertex3f(w / 2, -h / 2, w / 2);
	glTexCoord2f(1.0f, 1.0f);
	//  1  1  1 = top right
	glVertex3f(w / 2,  h / 2, w / 2);
	glTexCoord2f(0.0f, 1.0f);
	// -1  1  1 = top left
	glVertex3f(-w / 2,  h / 2, w / 2);
	
	
	glEnd();
	
	//Back face
	glBindTexture(GL_TEXTURE_2D, _textureBack);		
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	///glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBegin(GL_QUADS);

	glNormal3f(0.0, 0.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f);
	// -1 -1 -1
	glVertex3f(-w / 2, -h / 2, -w / 2);
	glTexCoord2f(1.0f, 1.0f);
	// -1  1 -1
	glVertex3f(-w / 2, h / 2, -w / 2);
	glTexCoord2f(0.0f, 1.0f);
	//  1  1 -1
	glVertex3f(w / 2, h / 2, -w / 2);
	glTexCoord2f(0.0f, 0.0f);
	//  1 -1 -1
	glVertex3f(w / 2, -h / 2, -w / 2);
	
	glEnd();

	

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}
