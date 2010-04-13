#include "Wall.h"
#include "Map.h"

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

Wall::Wall() {

}

Wall::Wall(float xIn, float yIn, float zIn, float inWidth, float inLength, float inHeight, int inTexture, bool inMark) 
	: StaticObject(xIn, yIn, zIn)
{
	width = inWidth;
	length = inLength;
	height = inHeight;
	wallTexture = inTexture;
	mark = inMark;

	if(mark) {
		Map * instance = Map::getInstance();
		int cellSide = instance->getCellSide();

		int * minMapCoord = instance->convertWorldCoordToMapCoord(x-width/2-cellSide,z-length/2-cellSide);
		int minRow = minMapCoord[0];
		int minCol = minMapCoord[1];

		int * maxMapCoord = instance->convertWorldCoordToMapCoord(x+width/2+cellSide,z+length/2+cellSide);
		int maxRow = maxMapCoord[0];
		int maxCol = maxMapCoord[1];

		int markWidth = maxCol - minCol;
		int markHeight = maxRow - minRow;

		if(length > width) {
			int f = 0 +  1;
		}

		instance->markBlock(minRow, minCol, markHeight, markWidth, 9);
	}
}

Wall::~Wall(void)
{
}

void Wall::draw() {
	glPushMatrix();
	glTranslatef(x, y + height/2, z);

	bool drawfront = true;
	bool drawback = true;
	bool drawleft = true;
	bool drawright = true;
	bool drawtop = true;
	bool drawbottom = true;

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, wallTexture);		
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	
	
	float w = width/2;
	float h = height/2;
	float l = length/2;

	if(drawfront) {
		//front
		glBegin(GL_QUADS);
		glNormal3f(0.0f, 0.0f, 1.0f);		

		glTexCoord2f(0.0f,0.0f); glVertex3f(-w, -h,  l); //bottom left		
		glTexCoord2f(1.0f,0.0f); glVertex3f( w, -h,  l); //bottom right
		glTexCoord2f(1.0f,1.0f); glVertex3f( w,  h,  l); //top right
		glTexCoord2f(0.0f,1.0f); glVertex3f(-w,  h,  l); //top left		
		glEnd();
	}
    

	if(drawleft) {
		//left
		glBegin(GL_QUADS);
		glNormal3f(-1.0f, 0.0f, 0.0f);
		glTexCoord2f(0,0); glVertex3f(-width/2, -height/2, -length/2); //bottom left		
		glTexCoord2f(1,0); glVertex3f(-width/2, -height/2,  length/2); //bottom right
		glTexCoord2f(1,1); glVertex3f(-width/2,  height/2,  length/2); //top right
		glTexCoord2f(0,1); glVertex3f(-width/2,  height/2, -length/2); //top left		
		glEnd();
	}

	if(drawright) {
		//right
		glBegin(GL_QUADS);
		glNormal3f(1.0f, 0.0f, 0.0f);
		glTexCoord2f(1,0); glVertex3f(width/2, -height/2, -length/2); //BR
		glTexCoord2f(1,1); glVertex3f(width/2,  height/2, -length/2); //TR
		glTexCoord2f(0,1); glVertex3f(width/2,  height/2,  length/2); //TL
		glTexCoord2f(0,0); glVertex3f(width/2, -height/2,  length/2); //BL
		glEnd();
	}

	if(drawback) {
		//back
		glBegin(GL_QUADS);
		glNormal3f(0.0f, 0.0f, -1.0f);
		glTexCoord2f(0,1);
		glVertex3f( width/2,  height/2,  -length/2);
		glTexCoord2f(1,1);
		glVertex3f(-width/2,  height/2,  -length/2);
		glTexCoord2f(1,0);
		glVertex3f(-width/2, -height/2,  -length/2);
		glTexCoord2f(0,0);
		glVertex3f( width/2, -height/2,  -length/2);
		glEnd();
	}

	//top
	if(drawtop) {
		glBegin(GL_QUADS);
		glNormal3f(0.0f, 1.0f, 0.0f);
		glTexCoord2f(0,1);
		glVertex3f(-width/2,  height/2, -length/2);
		glTexCoord2f(1,1);
		glVertex3f( width/2,  height/2, -length/2);
		glTexCoord2f(1,0);
		glVertex3f( width/2,  height/2,  length/2);
		glTexCoord2f(0,0);
		glVertex3f(-width/2,  height/2,  length/2);
		glEnd();
	}

	if(drawbottom) {
		//bottom
		glBegin(GL_QUADS);
		glNormal3f(0.0f, -1.0f, 0.0f);
		glTexCoord2f(0,1);
		glVertex3f( width/2,  -height/2, length/2);
		glTexCoord2f(1,1);
		glVertex3f(-width/2,  -height/2, length/2);
		glTexCoord2f(1,0);
		glVertex3f(-width/2,  -height/2, -length/2);
		glTexCoord2f(0,0);
		glVertex3f( width/2,  -height/2, -length/2);
		glEnd();
	}

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}
