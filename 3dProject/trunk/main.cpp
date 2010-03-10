/* Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above notice and this permission notice shall be included in all copies
 * or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR otherWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
/* File for "Mipmapping" lesson of the OpenGL tutorial on
 * www.videotutorialsrock.com
 */



#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <ctime>

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "imageloader.h"
#include "vec3f.h"

using namespace std;

const float FLOOR_TEXTURE_SIZE = 15.0f; //The size of each floor "tile"
const float PLAYER_EYE_HEIGHT  = 18.f;
const float HORI_SIZE = 100.0f;
const float VERTI_SIZE = 60.0f;

const int BOUNCE_SIZE = 256;

float _angle = 0.0f;
int _textureFGL, _textureFGC, _textureFGR, _textureLeft, _textureRight,
_textureBack, _textureTop, _textureFront, _textureFloor, _textureShooting,
_textureGunOff, _textureGun, _textureArtesis,
mouseX, mouseY;
int _skyTex[6];

//The forward position relative to the floor
float _pos = 0;
float xPos = 0.0f;
float yPos = PLAYER_EYE_HEIGHT;
float zPos = 0.0f;

// rotation
float xrot = 0.0f;
float yrot = 0.0f;

int keySens = 5; // relative to player's movement speed

// stores which keys are depressed
bool keys[256];

int w1;
int h1;

// some variables for gun animation
clock_t pressTime = clock();
clock_t releaseTime = clock();
bool gunOn = false;
bool spaceDown = false;

const string textureDir = "./textures/";

/**
 * On Lights: 
 * GL_LIGHT0 = sun (diffuse)
 * GL_LIGHT1 = sun (ambient)
 * GL_LIGHT2 = HUD LIGHT
 *
 *
 * GL_LIGHT2 should only be used on statically lit HUD items,
 * such as the logo and the crosshairs, GL_LIGHT0 should be OFF,
 * because it is 'dynamic' (it changes intensity on player rotation)
 *
 */

//weirdthing variables
bool incVert = true; //true to increment, false to decrement
bool incHor = false;  //''
float vertiOff = 0.0f;
float horiOff = 0.0f;
float weirdAngle = 0.0f;


class BoundingBox {
	private:
		float w; //Width
		float h; //Height
		float x; //x
		float y; //y
		float z; //z
	public:
		BoundingBox() {

		}

		BoundingBox(float wIn, float hIn, float xIn, float yIn, float zIn) {
			w = wIn;
			h = hIn;
			x = xIn;
			y = yIn;
			z = zIn;
		}
		
		~BoundingBox() {
		}

		bool collidesWith(BoundingBox otherBB) {
			float otherX = otherBB.getX();
			float otherY = otherBB.getY();
			float otherZ = otherBB.getZ();
			float otherW = otherBB.getWidth();
			float otherH = otherBB.getHeight();

			
			float otherFront[4][3];
			float otherLeft[4][3];
			float otherRight[4][3];
			float otherBack[4][3];

			float myFront[4][3];
			float myLeft[4][3];
			float myRight[4][3];
			float myBack[4][3];
	
			//1 = 'linksboven', 2 = 'rechtsboven', ...
			float myTop[4][3] = {
				{x - w/2, y + h/2, z + w/2},
				{x + w/2, y + h/2, z + w/2},
				{x + w/2, y + h/2, z - w/2},
				{x - w/2, y + h/2, z - w/2}
			};
			
			//cout << x << " "  << y << " " << z << endl; 
			
			float myBottom[4][3] = {
				{x - w/2, y - h/2, z + w/2},
				{x + w/2, y - h/2, z + w/2},
				{x + w/2, y - h/2, z - w/2},
				{x - w/2, y - h/2, z - w/2}
			};

			float otherTop[4][3] = {
				{otherX - otherW/2, otherY + otherH/2, otherZ + otherW/2},
				{otherX + otherW/2, otherY + otherH/2, otherZ + otherW/2},
				{otherX + otherW/2, otherY + otherH/2, otherZ - otherW/2},
				{otherX - otherW/2, otherY + otherH/2, otherZ - otherW/2}
			};
			cout << otherTop[0][1] << " " << otherTop[0][2] << " " << otherTop[0][3] <<  endl;

			float otherBottom[4][3] = {
				{otherX - otherW/2, otherY - otherH/2, otherZ + otherW/2},
				{otherX + otherW/2, otherY - otherH/2, otherZ + otherW/2},
				{otherX + otherW/2, otherY - otherH/2, otherZ - otherW/2},
				{otherX - otherW/2, otherY - otherH/2, otherZ - otherW/2}
			};

			*otherFront[0] = *otherTop[3];
			*otherFront[1] = *otherTop[2];
			*otherFront[2] = *otherBottom[3];
			*otherFront[3] = *otherBottom[2];
			
			*myFront[0] = *myTop[3];
			*myFront[1] = *myTop[2];
			*myFront[2] = *myBottom[3];
			*myFront[3] = *myBottom[2];

			*otherLeft[0] = *otherTop[0]; 
			*otherLeft[1] = *otherTop[3];
			*otherLeft[2] = *otherBottom[2];
			*otherLeft[3] = *otherBottom[0];

			*myLeft[0] = *myTop[0]; 
			*myLeft[1] = *myTop[3];
			*myLeft[2] = *myBottom[2];
			*myLeft[3] = *myBottom[0];

			*otherBack[0] = *otherTop[1]; 
			*otherBack[1] = *otherTop[0];
			*otherBack[2] = *otherBottom[0];
			*otherBack[3] = *otherBottom[1];

			*myBack[0] = *myTop[1]; 
			*myBack[1] = *myTop[0];
			*myBack[2] = *myBottom[0];
			*myBack[3] = *myBottom[1];

			*otherRight[0] = *otherTop[2]; 
			*otherRight[1] = *otherTop[1];
			*otherRight[2] = *otherBottom[1];
			*otherRight[3] = *otherBottom[3];

			*myRight[0] = *myTop[2]; 
			*myRight[1] = *myTop[1];
			*myRight[2] = *myBottom[1];
			*myRight[3] = *myBottom[3];

			int i;
			bool topPlaneIntersect[] = {false,false,false,false};
			for(i = 0; i < 4; i++) {
					topPlaneIntersect[i] = 
					((otherTop[0][0] < myTop[0][0]) && 
					(otherTop[i][2] > myTop[0][2]) && 
					(otherTop[i][0] > myTop[1][0]) &&
					(otherTop[i][2] > myTop[1][2]) &&
					(otherTop[i][0] < myTop[2][0]) &&
					(otherTop[i][2] > myTop[2][2]) &&
					(otherTop[i][0] > myTop[3][0]) &&
					(otherTop[i][2] > myTop[3][2]));			
					//cout << topPlaneIntersect[i] << " "; 
			}		
			//cout << endl;

			bool frontPlaneIntersect[] = {false,false,false,false};
			for(i = 0; i < 4; i++) {
					frontPlaneIntersect[i] = 
					((otherFront[i][0] < myFront[0][0]) && 
					(otherFront[i][1] > myFront[0][1]) && 
					(otherFront[i][0] > myFront[1][0]) &&
					(otherFront[i][1] > myFront[1][1]) &&
					(otherFront[i][0] < myFront[2][0]) &&
					(otherFront[i][1] > myFront[2][1]) &&
					(otherFront[i][0] > myFront[3][0]) &&
					(otherFront[i][1] > myFront[3][1]));	
					//cout << frontPlaneIntersect[i]<< " "; 
			}		
			//cout << endl;

			return false;
		}
		
		float getWidth() {
			return w;
		}
		
		float getHeight() {
			return h;
		}

		float getX() {
			return x;
		}

		float getY() {
			return y;
		}

		float getZ() {
			return z;
		}		

		void setX(float xIn) {
			x = xIn;
		}

		void setY(float yIn) {
			y = yIn;
		}

		void setZ(float zIn) {
			z = zIn;
		}

};

class Bunker {
	private:
		float w; //Width
		float h; //Height
		float x; //Xpos
		float y; //Ypos
		float z; //Zpos
		BoundingBox bb;
	public:
		Bunker() {

		}

		Bunker(float wIn, float hIn, float xIn, float yIn, float zIn) {
			w = wIn;
			h = hIn;
			x = xIn;
			y = yIn;
			z = zIn;
			bb = BoundingBox(w,h,x,y,z);
		}
		
		~Bunker() {
		}

		bool collidesWithPC(BoundingBox otherBox) {
			return bb.collidesWith(otherBox);
		}
		
		float getWidth() {
			return w;
		}
		
		float getHeight() {
			return h;
		}

		float getX() {
			return x;
		}

		float getY() {
			return y;
		}

		float getZ() {
			return z;
		}

		void draw() {
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

			glNormal3f(-1.0, 0.0f, 0.0f);
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

			glNormal3f(1.0, 0.0f, 0.0f);
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
			
			
			//Front face
			glEnd();

			glBindTexture(GL_TEXTURE_2D, _textureFront);		
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glBegin(GL_QUADS);

			glNormal3f(0.0, 0.0f, -1.0f);
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
};

class Cylinder {
	private:
		float r; //radius
		float h; //height
		float x; //Xpos
		float y; //Ypos
		float z; //Zpos
		BoundingBox bb;
	public:

		Cylinder() {

		}

		Cylinder(float rIn, float hIn, float xIn, float yIn, float zIn) {
			r = rIn;
			h = hIn;
			x = xIn;
			y = yIn;
			z = zIn;
			bb = BoundingBox(r*2,h,x,y,z);
		}
		
		~Cylinder() {
		
		}

		bool collidesWithPC(BoundingBox otherBox) {
			return bb.collidesWith(otherBox);
		}
		
		float getRadius() {
			return r;
		}
		
		float getHeight() {
			return h;
		}

		float getX() {
			return x;
		}

		float getY() {
			return y;
		}

		float getZ() {
			return z;
		}

		void draw() {
			glPushMatrix();
			glTranslatef(x, y + (h / 2), z);
			//http://nehe.gamedev.net/data/lessons/lesson.asp?lesson=18
			GLUquadricObj *quadratic; 
			quadratic = gluNewQuadric();			// Create A Pointer To The Quadric Object ( NEW )
			gluQuadricNormals(quadratic, GLU_SMOOTH);	// Create Smooth Normals ( NEW )
			gluQuadricTexture(quadratic, GL_TRUE);		// Create Texture Coords ( NEW )
			gluCylinder(quadratic,r,r,h,32,32);

			glPopMatrix();
		}
};

Bunker bunker1 ;
Bunker bunker2 ;
Cylinder cylinder1;
BoundingBox pcBB = BoundingBox(10.0f,PLAYER_EYE_HEIGHT,xPos,yPos,zPos);

void orthogonalStart() {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, w1, 0, h1);
    glScalef(1, -1, 1);
    glTranslatef(0, -h1, 0);
    glMatrixMode(GL_MODELVIEW);
}

// extra byte toevoegen (RGBA ipv RGB) => alpha channel;
// 0xFF = transparant, 0x00 = niet doorzichtig
void toRGBA(unsigned char *dst, unsigned char *src, unsigned int width, unsigned int height) {
  unsigned int a, size = width*height;
  cout << "\t\t\tGenerating alpha of size " << size << endl;
  clock_t startTime = clock();

  for(a=0; a<size; ++a) {
    //de eerste drie channels over kopieren
    *(dst++) = src[0]; 
    *(dst++) = src[1]; 
	*(dst++) = src[2]; 
	
	// we willen alle witte pixels in de BMP transparant zetten
	if(src[0] > 0xEE && src[1] > 0xEE && src[2] > 0xEE) {
		*(dst++) = 0x00;
		//beetje verbose
		//cout << ".";
	}	else {
      *(dst++) = 0xFF;
	  //beetje verbose
	  //cout << "-";
	}	
	// pointer met 3 verzetten
    src += 3;
  }
  clock_t endTime = clock();
  float diff = (float)(endTime - startTime)/CLOCKS_PER_SEC;
  cout << "\t\t\tTime to generate Alpha: " << diff << endl;
  cout << "\t\t\tPixels/Sec: " << size / diff << endl;
}



void orthogonalEnd() {
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void handleKeyrelease(unsigned char key, int x1, int y1) {
	keys[(int) key] = false;
}

void handleKeypress(unsigned char key, int x1, int y1) {
	keys[(int) key] = true;
}

void keyboardHandler() {
	float xrotrad, yrotrad;
	float speed = 0.3;

	if(keys[27]){  //Escape key
		exit(0);
	}	

	if(gunOn) {
		_textureGun = _textureShooting;
		clock_t time = clock();
		double diff = (time - pressTime) / (double)CLOCKS_PER_SEC;
		if(diff > 0.1) {
			gunOn = !(gunOn);
		}
		cout << pressTime << " " << releaseTime << endl;
	} else {
		_textureGun = _textureGunOff;
	}

	if(keys[' ']) {		
		if(!spaceDown) {
			cout << "down" << " "  << endl;
			spaceDown = true;
			pressTime = clock();
			gunOn = true;
		}
	} else {		
		if(spaceDown) {
			cout << "up" << " " << endl;
			spaceDown = false;
			releaseTime = clock();
		}
	}

	if((keys['w'] || keys['s'])  && (keys['a'] || keys['d'])) {
		speed = speed / 2;
	}
	if(keys['w']){  // move camera closer
		yrotrad = (yrot / 180 * 3.141592654f);
		xrotrad = (xrot / 180 * 3.141592654f);
		xPos += float(sin(yrotrad)) * speed * keySens;
		zPos -= float(cos(yrotrad)) * speed * keySens;
	}
	if(keys['s']){  // move camera farther
		yrotrad = (yrot / 180 * 3.141592654f);
		xrotrad = (xrot / 180 * 3.141592654f);
		xPos -= float(sin(yrotrad)) * speed * keySens;
		zPos += float(cos(yrotrad)) * speed * keySens;
	}
	if(keys['a']){  // move camera left
		yrotrad = (yrot / 180 * 3.141592654f);
		xPos -= float(cos(yrotrad)) * speed * keySens;
		zPos -= float(sin(yrotrad)) * speed * keySens;
	}
	if(keys['d']){  // move camera right			
		yrotrad = (yrot / 180 * 3.141592654f);
		xPos += float(cos(yrotrad)) * speed * keySens;
		zPos += float(sin(yrotrad)) * speed * keySens;
	}
	if(keys['q']){  // rotate camera left
		yrot -= 1.0f * keySens; 
	}
	if(keys['e']){  // rotate camera right			
		yrot += 1.0f * keySens; 
	}
	if(keys['z']){  // move gun left
		_textureFront = _textureFGL;
	}
	if(keys['c']){  // move gun right
		_textureFront = _textureFGR;
	}
	if(keys['x']){  // center gun 
		_textureFront = _textureFGC;
	}

	if (xrot > 360) {
		xrot -= 360;
	}

	if (xrot < -360) {
		xrot += 360;
	}

	if (yrot < -360) {
		yrot += 360;
	}

	if (yrot > 360) {
		yrot -= 360;
	}	

	if((keys['w'] || keys['s'])  || (keys['a'] || keys['d'])) {
		cout << xPos << " " << yPos << " " << zPos << endl;
		pcBB.setX(xPos);
		pcBB.setY(yPos);
		pcBB.setZ(zPos);
		bunker1.collidesWithPC(pcBB);
		//cout << pcBB.getX() << " " << pcBB.getY() << " " << pcBB.getZ() << endl;
	}
 
}

void mouseMotion(int x, int y) {
	int diffx = x - mouseX;
	int diffy = y - mouseY;

	xrot += (float) diffy;
	yrot += (float) diffx;
	
	if (xrot > 75) {
		xrot = 75;
	}

	if (xrot < -75) {
		xrot = -75;
	}

	if (yrot < -360) {
		yrot += 360;
	}

	if (yrot > 360) {
		yrot -= 360;
	}
	cout << "rotation" << " " << xrot << " " << yrot << endl;

	mouseY = y;
	mouseX = x;
}

//Makes the image into a mipmapped texture, and returns the id of the texture
GLuint loadMipmappedTexture_RGBA(Image *image) {
	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	unsigned char* rgbaPix = new unsigned char[image->width * image->height * 4];
	toRGBA((unsigned char*) rgbaPix, (unsigned char*) image->pixels, image->width, image->height);
	gluBuild2DMipmaps(GL_TEXTURE_2D,
					  GL_RGBA,
					  image->width, image->height,
					  GL_RGBA,
					  GL_UNSIGNED_BYTE,
					  rgbaPix);
	return textureId;
}

GLuint loadTexture_RGBA(Image* image) {
	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	unsigned char* rgbaPix = new unsigned char[image->width * image->height * 4];
	toRGBA((unsigned char*) rgbaPix, (unsigned char*) image->pixels, image->width, image->height);
	glTexImage2D(GL_TEXTURE_2D,
				 0,
				 GL_RGBA,
				 image->width, image->height,
				 0,
				 GL_RGBA,
				 GL_UNSIGNED_BYTE,
				 rgbaPix);
	return textureId;
}

GLuint loadMipmappedTexture(Image *image) {
	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	gluBuild2DMipmaps(GL_TEXTURE_2D,
					  GL_RGB,
					  image->width, image->height,
					  GL_RGB,
					  GL_UNSIGNED_BYTE,
					  image->pixels);
	return textureId;
}

void initRendering() {
	cout << "*Init rendering" << endl;

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	
	cout << "\tLoading images" << endl;

	string name = textureDir;
	name += "front_gun_left.bmp";
	Image* image = loadBMP(name.c_str());
	_textureFGL = loadTexture_RGBA(image);

	name = textureDir;
	name += "front_gun_center.bmp";
	image = loadBMP(name.c_str());
	_textureFGC = loadTexture_RGBA(image);
	_textureFront = _textureFGC;

	name = textureDir;
	name += "front_gun_right.bmp";
	image = loadBMP(name.c_str());
	_textureFGR = loadTexture_RGBA(image);

	name = textureDir;
	name += "left.bmp";
	image = loadBMP(name.c_str());
	_textureLeft = loadTexture_RGBA(image);	
	
	name = textureDir;
	name += "right.bmp";
	image = loadBMP(name.c_str());
	_textureRight = loadTexture_RGBA(image);

	name = textureDir;
	name += "back.bmp";
	image = loadBMP(name.c_str());
	_textureBack = loadTexture_RGBA(image);

	name = textureDir;
	name += "top.bmp";
	image = loadBMP(name.c_str());
	_textureTop = loadTexture_RGBA(image);

	name = textureDir;
	name += "checkerboard.bmp";
	image = loadBMP(name.c_str());
	_textureFloor = loadMipmappedTexture(image);

	name = textureDir;
	name += "artesis.bmp";
	image = loadBMP(name.c_str());
	_textureArtesis = loadTexture_RGBA(image);

	name = textureDir;
	name += "gun_2_layers_shooting_16w-9h.bmp";
	image = loadBMP(name.c_str());
	_textureShooting = loadTexture_RGBA(image);

	name = textureDir;
	name += "gun_2_layers_16w-9h.bmp";
	image = loadBMP(name.c_str());
	_textureGunOff = loadTexture_RGBA(image);
	_textureGun = _textureGunOff;
	delete image;

	string skyName[6];

	skyName[0] = textureDir;
    skyName[0] += "hotdesert_positive_z.bmp"; //front
	skyName[1] = textureDir;
    skyName[1] += "hotdesert_positive_x.bmp"; //right
	skyName[2] = textureDir;
    skyName[2] += "hotdesert_negative_z.bmp"; //back
	skyName[3] = textureDir;
    skyName[3] += "hotdesert_negative_x.bmp"; //left
	skyName[4] = textureDir;
    skyName[4] += "hotdesert_positive_y.bmp"; //top
	skyName[5] = textureDir;
    skyName[5] += "hotdesert_negative_y.bmp"; //bottom

	int i;
	for (i = 0; i < 6; i++) {
		image = loadBMP(skyName[i].c_str());
		_skyTex[i] = loadMipmappedTexture(image);
		delete image;
	}
	
	cout << "\tImages loaded" << endl;
}


void handleResize(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (float)w / (float)h, 1.0, 1000.0);
	h1 = h;
	w1 = w;
}


void drawSkyBox() {
	glPushMatrix();
	glTranslatef(xPos, yPos, zPos);
	float side = 1000.0f;

	GLfloat diffuseLightColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat ambientLightColor[] = {0.4f, 0.4f, 0.4f, 1.0f};
	GLfloat lightPos[] = {0, 0, -side/2 + 10, 1.0f};
	GLfloat lightDir[] = {0.0f, -1.0f, -1.0f};
	
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLightColor);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, lightDir);
	glEnable(GL_LIGHT0);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLightColor);
	glLightfv(GL_LIGHT1, GL_AMBIENT, ambientLightColor);
	glEnable(GL_LIGHT1);

	glEnable(GL_TEXTURE_2D);

	//front
	glBindTexture(GL_TEXTURE_2D, _skyTex[0]);		
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBegin(GL_QUADS);
	glNormal3f(0.0, 0.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f);
	// -1 -1  1 = bottom left
	glVertex3f(-side / 2, -side / 2, side / 2);
	glTexCoord2f(1.0f, 0.0f);
	//  1 -1  1 = bottom right
	glVertex3f(side / 2, -side / 2, side / 2);
	glTexCoord2f(1.0f, 1.0f);
	//  1  1  1 = top right
	glVertex3f(side / 2,  side / 2, side / 2);
	glTexCoord2f(0.0f, 1.0f);
	// -1  1  1 = top left
	glVertex3f(-side / 2,  side / 2, side / 2);		
	glEnd();

	//right face
	glBindTexture(GL_TEXTURE_2D, _skyTex[1]);		
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBegin(GL_QUADS);
	glNormal3f(1.0, 0.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	// x  y  z
	// 1 -1 -1  = bottom right
	glVertex3f(side / 2, -side / 2, -side / 2);
	glTexCoord2f(1.0f, 1.0f);
	// 1  1 -1  = top right
	glVertex3f(side / 2, side / 2, -side / 2);
	glTexCoord2f(0.0f, 1.0f);
	// 1  1  1  = top left
	glVertex3f(side / 2, side / 2, side / 2);
	glTexCoord2f(0.0f, 0.0f);
	// 1 -1  1  = bottom left
	glVertex3f(side / 2, -side / 2, side / 2);
	glEnd();
	
	//Back face
	glBindTexture(GL_TEXTURE_2D, _skyTex[2]);		
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBegin(GL_QUADS);
	glNormal3f(0.0, 0.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f);
	// -1 -1 -1
	glVertex3f(-side / 2, -side / 2, -side / 2);
	glTexCoord2f(1.0f, 1.0f);
	// -1  1 -1
	glVertex3f(-side / 2, side / 2, -side / 2);
	glTexCoord2f(0.0f, 1.0f);
	//  1  1 -1
	glVertex3f(side / 2, side / 2, -side / 2);
	glTexCoord2f(0.0f, 0.0f);
	//  1 -1 -1
	glVertex3f(side / 2, -side / 2, -side / 2);
	glEnd();

	//Left face
	glBindTexture(GL_TEXTURE_2D, _skyTex[3]);		
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	
	glBegin(GL_QUADS);
	glNormal3f(-1.0, 0.0f, 0.0f);
	//  x  y  z
	// -1 -1 -1 = bottom left
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-side / 2, -side / 2, -side / 2);
	// -1 -1  1 = bottom right
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-side / 2, -side / 2, side / 2);
	// -1  1  1 = top right
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-side / 2, side / 2, side / 2);
	// -1  1 -1 = top left
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-side / 2, side / 2, -side / 2);	
	glEnd();
	
	//Top face	
	glBindTexture(GL_TEXTURE_2D, _skyTex[4]);		
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBegin(GL_QUADS);
	glNormal3f(0.0, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f);
	// -1  1 -1 = top left
	glVertex3f(-side / 2, side / 2, -side / 2);
	glTexCoord2f(0.0f, 0.0f);
	// -1  1  1 = bottom left
	glVertex3f(-side / 2, side / 2, side / 2);
	glTexCoord2f(1.0f, 0.0f);
	//  1  1  1 = bottom right
	glVertex3f(side / 2, side / 2, side / 2);
	glTexCoord2f(1.0f, 1.0f);
	//  1  1 -1 = top right
	glVertex3f(side / 2, side / 2, -side / 2);	
	glEnd();
	
	//Bottom face
	glBegin(GL_QUADS);
	glNormal3f(0.0, -1.0f, 0.0f);
	glVertex3f(-side / 2, -side / 2, -side / 2);
	glVertex3f(side / 2, -side / 2, -side / 2);
	glVertex3f(side / 2, -side / 2, side / 2);
	glVertex3f(-side / 2, -side / 2, side / 2);	
	glEnd();
	
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void camera() {	
	//look straight ahead
	gluLookAt(0.0, 0.0, 0.0, 0.0, 0.0, -5.0, 0.0, 1.0, 0.0);

	
	GLfloat lightPos[] = {0, 0, -5.0f, 1.0f};
	GLfloat diffuseLightColor[] = {0.4f, 0.4f, 0.4f, 1.0f};
	glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuseLightColor);
	glLightfv(GL_LIGHT2, GL_POSITION, lightPos);
	glEnable(GL_LIGHT2);


	//crosshairs
	glDisable(GL_LIGHT0);
	float crossSize = 0.05f;
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, 1.0f);
	// -1 -1  1 = bottom left
	glVertex3f(-crossSize / 2 , -crossSize / 2, -10.0f);
	//  1 -1  1 = bottom right
	glVertex3f(crossSize / 2, -crossSize / 2, -10.0f);
	//  1  1  1 = top right
	glVertex3f(crossSize / 2,  crossSize / 2, -10.0f);
	// -1  1  1 = top left
	glVertex3f(-crossSize / 2,  crossSize / 2, -10.0f);	
	glEnd();

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);  
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER,0.1f); 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	

	float scale = 0.20f;
	float height = 5 * scale;
	float width = 5 * scale;

	//artesis logo
	glBindTexture(GL_TEXTURE_2D, _textureArtesis);	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glPushMatrix();
	glTranslatef(-5.5f,2.8f,0.0f);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, -1.0f);
	// -1 -1  1 = bottom left
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-width, -height, -10.0f);
	//  1 -1  1 = bottom right
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(width, -height, -10.0f);
	//  1  1  1 = top right
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(width,  height, -10.0f);
	// -1  1  1 = top left
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-width,  height, -10.0f);	
	glEnd();
	glPopMatrix();

	glDisable(GL_LIGHT2);
	glEnable(GL_LIGHT0);

	scale = 0.38f;
	width = 16 * scale;
	height = 9 * scale;

	//gun
	glBindTexture(GL_TEXTURE_2D, _textureGun);	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glPushMatrix();
	glTranslatef(7.0f,-4.0f,0.0f);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, 1.0f);
	// -1 -1  1 = bottom left
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-width, -height, -10.0f);
	//  1 -1  1 = bottom right
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(width, -height, -10.0f);
	//  1  1  1 = top right
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(width,  height, -10.0f);
	// -1  1  1 = top left
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-width,  height, -10.0f);	
	glEnd();
	glPopMatrix();

	
	

	glDisable(GL_ALPHA_TEST);
	glDisable(GL_BLEND); 
	glRotatef(xrot,1.0,0.0,0.0);  //rotate our camera
    glRotatef(yrot,0.0,1.0,0.0); 
	glTranslatef(-xPos, -yPos, -zPos);
}

void drawWeirdthing() {
	float centerX = -100.0f;
	float centerY = PLAYER_EYE_HEIGHT;
	float centerZ = -100.0f;
	glEnable(GL_COLOR_MATERIAL);
	glColor3f(1.0f, 0.3f, 0.3f);

	glPushMatrix();
	glTranslatef(centerX, centerY, -100.0f);
	glutSolidSphere(1.0,32,32);
	glPopMatrix();
	
	glColor3f(0.1f, 0.1f, 0.8f);

	glPushMatrix();
	glTranslatef(centerX, centerY, -100.0f);	
	glRotatef(45.0f, 0.0f, 1.0f, 0.0f);
	glTranslatef(0, sin(weirdAngle + 123) * 8.0f, cos(weirdAngle + 85) * 10.0f);	
	glutSolidSphere(0.5,32,32);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(centerX, centerY, -100.0f);	
	glRotatef(60.0f, 1.0f, 0.0f, 1.0f);
	glTranslatef(0, sin(-weirdAngle + 35) * 12.0f, cos(-weirdAngle + 63) * 10.0f);	
	glutSolidSphere(0.5,32,32);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(centerX, centerY, -100.0f);	
	glRotatef(60.0f, 1.0f, 0.0f, 1.0f);
	glTranslatef(0, sin(-weirdAngle + 25) * 10.0f, cos(weirdAngle + 70) * 8.0f);	
	glutSolidSphere(0.5,32,32);
	glPopMatrix();
	
	float newCx = centerX + cos(weirdAngle) * 10.0f;
	float newCy = centerY + sin(weirdAngle) * 8.0f;
	glPushMatrix();
	glTranslatef(newCx, newCy, -100.0f);
	glutSolidSphere(0.5,32,32);
	glPopMatrix();

	glColor3f(1.0f, 1.0f, 1.0f);

	glPushMatrix();
	glTranslatef(newCx - horiOff, newCy, -100.0f);
	glutSolidSphere(0.25,32,32);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(newCx + horiOff, newCy + vertiOff, -100.0f);
	glutSolidSphere(0.25,32,32);
	glPopMatrix();
	
}

void drawFloor(float y) {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _skyTex[5]);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,
					GL_TEXTURE_MIN_FILTER,
					GL_LINEAR_MIPMAP_LINEAR);


	glBegin(GL_QUADS);
	
	glNormal3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(2000 / FLOOR_TEXTURE_SIZE, _pos / FLOOR_TEXTURE_SIZE);
	glVertex3f(-1000.0f, y, -1000.0f);
	glTexCoord2f(2000 / FLOOR_TEXTURE_SIZE, (2000 + _pos) / FLOOR_TEXTURE_SIZE);
	glVertex3f(-1000.0f, y, 1000.0f);
	glTexCoord2f(0.0f, (2000 + _pos) / FLOOR_TEXTURE_SIZE);
	glVertex3f(1000.0f, y, 1000.0f);
	glTexCoord2f(0.0f, _pos / FLOOR_TEXTURE_SIZE);
	glVertex3f(1000.0f, y, -1000.0f);
	
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void drawLight() {
	glPushMatrix();
	
	glPopMatrix();
}

void drawScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	glMatrixMode(GL_MODELVIEW);  	
	glLoadIdentity();
	
	keyboardHandler();
	camera();
	drawLight();
	drawSkyBox();
	//orthogonalStart();
	//glBegin(GL_QUADS);	
	//glVertex2f(300, 300);
	//glVertex2f(300, 800);
	//glVertex2f(800, 800);
	//glVertex2f(800, 300);
	//glEnd();
	//orthogonalEnd();

	//GLfloat ambientLight[] = {0.3f, 0.3f, 0.3f, 1.0f};
	//glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
	
	
	
	//glRotatef(-_angle, 1.0f, 1.0f, 0.0f);

	drawFloor(0.0f);
	//drawTerrain();
	bunker1.draw();	
	bunker2.draw();	
	cylinder1.draw();	
	drawWeirdthing();

	glutSwapBuffers();
}

void update(int value) {
	//	_angle += 1.0f;
	//if (_angle > 360) {
	//	_angle -= 360;
	//}

	weirdAngle += 0.1f;
	if (weirdAngle > 360) {
		weirdAngle -= 360;
	}

	if(horiOff == 8.0f) {
		incHor = false;
	}
	if(horiOff == -8.0f) {
		incHor = true;
	}

	if(vertiOff == 8.0f) {
		incVert = false;
	}

	if(vertiOff == -8.0f) {
		incVert = true;
	}

	if(incHor) {		
		horiOff += 0.5f;
	} else {
		horiOff -= 0.5f;
	}
	if(incVert) {
		vertiOff += 0.5f;
	} else {
		vertiOff -= 0.5f;
	}

	glutPostRedisplay();
	glutTimerFunc(40, update, 0);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	
	//http://www.swiftless.com/tutorials/opengl/fullscreen.html
	//glutGameModeString( "1920x1080:32@75" ); //the settings for fullscreen mode
    //glutEnterGameMode(); //set glut to fullscreen using the settings in the line above
	
	glutInitWindowSize (800,450);
    glutInitWindowPosition (100, 100);
    glutCreateWindow ("Simple Window"); 


	initRendering();

	
	string name = textureDir;
	name += "heightmap.bmp";
//	_terrain = loadTerrain(name.c_str(), 20);

	bunker1 = Bunker(HORI_SIZE, VERTI_SIZE, 0, 0, -100);
	bunker2 = Bunker(HORI_SIZE, VERTI_SIZE, 120, 0, -100);
	cylinder1 = Cylinder(10.0f, 1.0f, 350, 0, -100);

	glutDisplayFunc(drawScene);
	glutKeyboardFunc(handleKeypress);
	glutKeyboardUpFunc(handleKeyrelease);
	glutPassiveMotionFunc(mouseMotion);
	glutReshapeFunc(handleResize);
	glutTimerFunc(25, update, 0);
	glutMainLoop();
	return 0;
}











