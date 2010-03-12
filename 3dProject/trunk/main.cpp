#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <ctime>
#include <list>

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "imageloader.h"
#include "vec3f.h"
#define PI 3.14159265

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
		float r; //Sphere radius
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
			r = sqrt(2* pow(w/2,2));
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

		float getDistance(float xIn, float yIn, float zIn) {
			// (distance to center) - radius
			//cout << r << endl;
			return sqrt(pow((xIn - x),2) + pow((yIn - y),2) + pow((zIn - z),2)) - r;
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

class Atom {
	private:
			float x;
			float y;
			float z;
			float r;
			int numElectrons;
			// electrons per shell <=  2 * [(shellNumber) ^ 2]
			int *radius;
			int *speed;
			int *shellNumber;
			int *phase;
			int *rotation;
			int *vecNum;
			bool *direction;
			float cosScale;
			float sinScale;
			clock_t startTime;
			clock_t atomTime;

			void init() {
				radius = new int[7];
				speed = new int[7];
				rotation = new int[numElectrons];
				phase = new int[numElectrons];
				shellNumber = new int [numElectrons];
				vecNum = new int [numElectrons];
				direction = new bool [numElectrons];
				
				bool hd = true;

				startTime = clock();
				int minRad = 10;
				int ra[] = {minRad,minRad + 4,minRad + 8,minRad + 12,minRad + 16,minRad + 20,minRad + 24};	
				int s[] = {200,190,180,170,160,150,140};
				int k = 0;
				for(k = 0; k < 7; k++) {
					radius[k] = ra[k];
					speed[k] = s[k];
				}								

				cosScale = 1;
				sinScale = 0.7;
				//assign each electron to their shell
				//give them a random phase shift
				//and a random rotation
				int shellNum = 1;
				int i = 0;
				for(i = 0; i < numElectrons; i++) {
					if( i >= (2 * pow((double)shellNum, 2) + (2 * pow((double)shellNum - 1, 2)) + 1)) {
						shellNum++;
					}
					shellNumber[i] = shellNum;
					phase[i] = rand() % 360 + 1;
					rotation[i] = rand() % 360 + 1;
					vecNum[i] = rand() % 3;

					hd = !(hd);
					direction[i] = hd;
					
					//cout <<  i << " " << shellNumber[i] << " " << rotation[i] << " " << speed[shellNumber[i]-1] << " "  << phase[i] << endl;
					//cout << endl;
				}
				r = radius[shellNum-1];
			}
	public:	
		Atom() {

		}

		Atom(float xIn, float yIn, float zIn, int numElec) {
			x = xIn;
			y = yIn;
			z = zIn;
			numElectrons = numElec;
			init();
		}

		~Atom() {

		}

		float getDistance(float xIn, float yIn, float zIn) {
			// (distance to center) - radius
			return sqrt(pow((xIn - x),2) + pow((yIn - y),2) + pow((zIn - z),2)) - r;
		}

		void draw() {
			atomTime = clock();
			double time = (double)(atomTime - startTime) / CLOCKS_PER_SEC;
			glEnable(GL_COLOR_MATERIAL);
			
			glColor3f(0.0f, 1.0f, 1.0f);
			glPushMatrix();
			glTranslatef(x, y, z);
			glutSolidSphere(1.0,32,32);
			glPopMatrix();			
			
			glColor3f(1.0f, 0.0f, 1.0f);
			//Scout << sin(speed[shellNumber[0]-1] * angle + phase[0]) << " " << cos(speed[shellNumber[0]-1] * angle + phase[0]) << endl;
			int i;
			for(i = 0; i < numElectrons; i++) {
				float amplitude = radius[shellNumber[i]-1];
				float thisPhase = phase[i] * PI / 180;
				float angFreq = speed[shellNumber[i]-1] * PI / 180;
				if(direction[i]) {
					angFreq = angFreq * -1;
				}
				//cout << time << " " << angFreq << " " << angFreq * time << endl;
				//cout << amplitude << " " << amplitude * sin(angFreq * time + thisPhase) << " " << amplitude * cos(angFreq * time + thisPhase) << endl;
				glPushMatrix();
				glTranslatef(x, y, z);	

				if(vecNum[i] == 0) {
					glRotatef(rotation[i], 1.0f, 0.0f, 0.0f);
				}
				if(vecNum[i] == 1) {
					glRotatef(rotation[i], 0.0f, 1.0f, 0.0f);
				}
				if(vecNum[i] == 2) {
					glRotatef(rotation[i], 1.0f, 1.0f, 0.0f);
				}
				//glTranslatef(0, sin(speed[shellNumber[i]-1] * angle + phase[i]) * radius[shellNumber[i]-1] * sinScale,
				//	cos(speed[shellNumber[i]-1] * angle + phase[i]) * radius[shellNumber[i]-1] * cosScale);	
				glTranslatef(0, amplitude * sin(angFreq * time + thisPhase) * sinScale,
					amplitude * cos(angFreq * time + thisPhase) * cosScale);	
				glutSolidSphere(0.5,32,32);
				glPopMatrix();
			}
			glColor3f(1.0f, 1.0f, 1.0f);
			glDisable(GL_COLOR_MATERIAL);
		}
};

class Bullet {
	private:
		float x;
		float y;
		float z;
		float speed;
		float xrotrad;
		float yrotrad;
		float maxTime;
		clock_t startTime;
		clock_t bulletTime;
		list<Bullet> theList;

	public:
		Bullet() {

		}

		~Bullet() {
		}

		Bullet(float xIn, float yIn, float zIn, float xRotIn, float yRotIn) {
			x = xIn;
			y = yIn;
			z = zIn;
			xrotrad = xRotIn;
			yrotrad = yRotIn;
			startTime = bulletTime = clock();
			speed = 100;
			maxTime = 5;
		}

			
		//http://artis.imag.fr/~Xavier.Decoret/resources/C++/operator==.html
		bool Bullet::operator==(const Bullet& other) const {
			//cout << "mine: " << x << "    other: " << other.x << endl;
			return (
				(x == other.x) &&
				(y == other.y) &&
				(z == other.z) &&
				(xrotrad == other.xrotrad) &&
				(yrotrad == other.yrotrad) &&
				(startTime == other.startTime) 
			);
		}

		bool move() {
			bulletTime = clock();
			float time = (float)(bulletTime - startTime) / CLOCKS_PER_SEC;
			if(time > maxTime) {
				cout << "killit" << endl;
				return true;
			} else {
				x += float(sin(yrotrad)) * speed * time;
				z -= float(cos(yrotrad)) * speed * time;
				y -= float(sin(xrotrad)) * speed * time * 1.5;
				cout << float(sin(xrotrad)) * 180 / 3.141592654f << endl;
				glPushMatrix();
				glTranslatef(x,y,z);
				glutSolidSphere(0.1,32,32);
				glPopMatrix();
				//cout << time << " " << x << " " << y << " " << z << endl;

				return false;
			}
		}
};

list<Bullet> bulletList;

Bunker bunker1 ;
Bunker bunker2 ;
Cylinder cylinder1;
Atom atom1;
Atom atom2;
Atom atom3;
Atom atom4;
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

bool collides() {
	return (bunker1.getDistance(xPos, yPos, zPos) < 10) ||
	(bunker2.getDistance(xPos, yPos, zPos) < 10) ||
	(atom1.getDistance(xPos, yPos, zPos) < 10) ||
	(atom2.getDistance(xPos, yPos, zPos) < 10) ||
	(atom3.getDistance(xPos, yPos, zPos) < 10) ||
	(atom4.getDistance(xPos, yPos, zPos) < 10);
}

void keyboardHandler() {
	float xrotrad, yrotrad;
	float speed = 0.3;
	yrotrad = (yrot / 180 * 3.141592654f);
	xrotrad = (xrot / 180 * 3.141592654f);

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
		//cout << pressTime << " " << releaseTime << endl;
	} else {
		_textureGun = _textureGunOff;
	}

	if(keys[' ']) {		
		if(!spaceDown) {
			//cout << "down" << " "  << endl;
			spaceDown = true;
			pressTime = clock();
			gunOn = true;
			
			Bullet bullet = Bullet(xPos, yPos, zPos, xrotrad, yrotrad);
			bulletList.push_back(bullet);
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

	bool collision = false;
	float oldXPos = xPos;
	float oldZPos = zPos;

	if(keys['w']){  // move camera closer
		cout << "w" << " " << endl;	
		xPos += float(sin(yrotrad)) * speed * keySens;
		zPos -= float(cos(yrotrad)) * speed * keySens;		
	}
	if(keys['s']){  // move camera farther
		xPos -= float(sin(yrotrad)) * speed * keySens;
		zPos += float(cos(yrotrad)) * speed * keySens;
	}
	if(keys['a']){  // move camera left
		xPos -= float(cos(yrotrad)) * speed * keySens;
		zPos -= float(sin(yrotrad)) * speed * keySens;
	}
	if(keys['d']){  // move camera right	
		xPos += float(cos(yrotrad)) * speed * keySens;
		zPos += float(sin(yrotrad)) * speed * keySens;
	}

	if(collides()) {
		xPos = oldXPos;
		zPos = oldZPos;
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
	//float centerX = -100.0f;
	//float centerY = PLAYER_EYE_HEIGHT;
	//float centerZ = -100.0f;
	//const int numElectrons = 14; // = Si
	//// electrons per shell <=  2 * [(shellNumber) ^ 2]
	//float radius[] = {5.0f,7.0f,9.0f,11.0f,13.0f,15.0f,17.0f};
	//float speed[] = {1.7f,1.6f,1.5f,1.4f,1.3f,1.2f,1.1f,1.0f};
	//int shellNumber [numElectrons];
	//int phase [numElectrons];
	//int rotation [numElectrons];
	//float cosScale = 1.0f;
	//float sinScale = 0.65f;

	////assign each electron to their shell
	////give them a random phase shift
	////and a random rotation
	//int shellNum = 1;
	//int i = 1;
	//for(i = 1; i < numElectrons; i++) {
	//	if( i >= (2 * pow((double)shellNum, 2) + (2 * pow((double)shellNum - 1, 2)) + 1)) {
	//		shellNum++;
	//	}
	//	shellNumber[i] = shellNum;
	//	phase[i] = rand() % 360 + 1;
	//	rotation[i] = rand() % 360 + 1;
	//}
	
	//glEnable(GL_COLOR_MATERIAL);
	//glColor3f(1.0f, 0.3f, 0.3f);

	//for(i = i; i < numElectrons; i++) {
	//	glPushMatrix();
	//	glTranslatef(centerX, centerY, -100.0f);	
	//	glRotatef(rotation[i], 0.0f, 1.0f, 0.0f);
	//	glTranslatef(0, sin(speed[shellNumber[i]-1] * weirdAngle + phase[i]) * radius[shellNumber[i]-1] * sinScale,
	//		cos(speed[shellNumber[i]-1] * weirdAngle + phase[i]) * radius[shellNumber[i]-1] * cosScale);	
	//	glutSolidSphere(0.5,32,32);
	//	glPopMatrix();
	//}
	//glPushMatrix();
	//glTranslatef(centerX, centerY, -100.0f);
	//glutSolidSphere(1.0,32,32);
	//glPopMatrix();
	//
	//glColor3f(0.1f, 0.1f, 0.8f);

	//glPushMatrix();
	//glTranslatef(centerX, centerY, -100.0f);	
	//glRotatef(45.0f, 0.0f, 1.0f, 0.0f);
	//glTranslatef(0, sin(weirdAngle + 123) * energyLevel[0], cos(weirdAngle + 85) * energyLevel[0]);	
	//glutSolidSphere(0.5,32,32);
	//glPopMatrix();

	//glPushMatrix();
	//glTranslatef(centerX, centerY, -100.0f);	
	//glRotatef(60.0f, 1.0f, 0.0f, 1.0f);
	//glTranslatef(0, sin(-weirdAngle + 35) * energyLevel[0], cos(-weirdAngle + 63) * energyLevel[0]);	
	//glutSolidSphere(0.5,32,32);
	//glPopMatrix();

	//glPushMatrix();
	//glTranslatef(centerX, centerY, -100.0f);	
	//glRotatef(100.0f, 1.0f, 0.0f, 1.0f);
	//glTranslatef(0, sin(-weirdAngle + 25) * energyLevel[2], cos(weirdAngle + 70) * energyLevel[2]);	
	//glutSolidSphere(0.5,32,32);
	//glPopMatrix();

	//glPushMatrix();
	//glTranslatef(centerX, centerY, -100.0f);	
	//glRotatef(75.0f, 1.0f, 0.0f, 1.0f);
	//glTranslatef(0, sin(-weirdAngle - 25) * energyLevel[2], cos(weirdAngle + 15) * energyLevel[2]);	
	//glutSolidSphere(0.5,32,32);
	//glPopMatrix();
	//
	//float newCx = centerX + cos(weirdAngle) * energyLevel[4];
	//float newCy = centerY + sin(weirdAngle) * energyLevel[4];
	//glPushMatrix();
	//glTranslatef(newCx, newCy, -100.0f);
	//glutSolidSphere(0.5,32,32);
	//glPopMatrix();

	//glColor3f(1.0f, 1.0f, 1.0f);

	//glPushMatrix();
	//glTranslatef(newCx - horiOff, newCy, -100.0f);
	//glutSolidSphere(0.25,32,32);
	//glPopMatrix();
	//glPushMatrix();
	//glTranslatef(newCx + horiOff, newCy + vertiOff, -100.0f);
	//glutSolidSphere(0.25,32,32);
	//glPopMatrix();
	//
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
	atom1.draw();
	atom2.draw();
	atom3.draw();
	 
	list<Bullet> copyList(bulletList);

	list<Bullet>::iterator it;
	
	cout << bulletList.size() << endl;
	
	for (it = copyList.begin() ; it != copyList.end(); it++ ) {
		Bullet b = *it;
		if(b.move()) {
			cout << "removing bullet" << endl;
			bulletList.remove(b);
			cout << bulletList.size() << endl;
		}
	}

	//drawWeirdthing();

	glutSwapBuffers();
}

void update(int value) {
	//	_angle += 1.0f;
	//if (_angle > 360) {
	//	_angle -= 360;
	//}

	weirdAngle += 0.5f;
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
	bunker2 = Bunker(HORI_SIZE, VERTI_SIZE, 0, 0, 100);
	cylinder1 = Cylinder(10.0f, 1.0f, 350, 0, -100);
	atom1 = Atom(-100.0f, PLAYER_EYE_HEIGHT, -100.0f, 14); //Si
	atom2 = Atom(-150.0f, PLAYER_EYE_HEIGHT, -100.0f, 8);  //Oxygen
	atom3 = Atom(-200.0f, PLAYER_EYE_HEIGHT, -100.0f, 1);  //Hydrogen

	glutDisplayFunc(drawScene);
	glutKeyboardFunc(handleKeypress);
	glutKeyboardUpFunc(handleKeyrelease);
	glutPassiveMotionFunc(mouseMotion);
	glutReshapeFunc(handleResize);
	glutTimerFunc(25, update, 0);
	glutMainLoop();
	return 0;
}











