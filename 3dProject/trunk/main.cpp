#include <iostream>
//#include <stdlib.h>
#include <math.h>
#include <ctime>
#include <list>

#include "StaticObject.h"
#include "Bunker.h"
#include "Atom.h"
#include "SittingDuck.h"
#include "Bullet.h"
#include "Helicopter.h"
#include "Flightpath.h"

#include "imageloader.h"
#include "vec3f.h"

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define PI 3.14159265

using namespace std;

const float FLOOR_TEXTURE_SIZE = 15.0f; //The size of each floor "tile"
const float PLAYER_EYE_HEIGHT  = 18.f;  //eye height

const float HORI_SIZE = 100.0f; //static bunker size
const float VERTI_SIZE = 60.0f;

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

// some variables for gun animation
clock_t pressTime = clock();
clock_t releaseTime = clock();
bool gunOn = false;
bool spaceDown = false;

// textures base directory
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

//Game objects
list<Bullet> bulletList;
list<SittingDuck> duckList;
Bunker bunker1;
Bunker bunker2;
Atom atom1;
Atom atom2;
Atom atom3;
Atom atom4;
Helicopter heli;
SittingDuck sittingDuck1;


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


void handleKeyrelease(unsigned char key, int x1, int y1) {
	keys[(int) key] = false;
}

void handleKeypress(unsigned char key, int x1, int y1) {
	keys[(int) key] = true;
}

//does player hit stuff?
bool collides() {
	//cout << atom1.getDistance(xPos, yPos, zPos) << " " << atom1.getR() << endl;
	
	return (bunker1.getDistance(xPos, yPos, zPos) < 10) ||
	(bunker2.getDistance(xPos, yPos, zPos) < 10) ||
	(atom1.getDistance(xPos, yPos, zPos) < 10) ||
	(atom2.getDistance(xPos, yPos, zPos) < 10) ||
	(atom3.getDistance(xPos, yPos, zPos) < 10) ||
	(atom4.getDistance(xPos, yPos, zPos) < 10);
}

//allow the keyboard to rule the world
void keyboardHandler() {
	float xrotrad, yrotrad;
	float speed = 0.3;
	yrotrad = (yrot / 180 * 3.141592654f);
	xrotrad = (xrot / 180 * 3.141592654f);

	if(keys[27]){  //Escape key
		exit(0);
	}	

	if((keys['w'] || keys['s'])  && (keys['a'] || keys['d'])) {
		speed = speed / 2;
	}

	bool collision = false;
	float oldXPos = xPos;
	float oldZPos = zPos;

	if(keys['w']){  // move camera closer
		//cout << "w" << " " << endl;	
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
	
	//collision detection
	if(keys['w'] || keys['s']  || keys['a'] || keys['d']) {
		if(collides()) {
			xPos = oldXPos;
			zPos = oldZPos;
		}
	}

	if(keys['q']){  // rotate camera left
		yrot -= 1.0f * keySens; 
	}
	if(keys['e']){  // rotate camera right			
		yrot += 1.0f * keySens; 
	}
	if(keys['z']){  // move gun left
		_textureFront = _textureFGL;
		bunker1.changeTexture(_textureFront, 0);
		bunker2.changeTexture(_textureFront, 0);
	}
	if(keys['c']){  // move gun right
		_textureFront = _textureFGR;
		bunker1.changeTexture(_textureFront, 0);
		bunker2.changeTexture(_textureFront, 0);
	}
	if(keys['x']){  // center gun 
		_textureFront = _textureFGC;
		bunker1.changeTexture(_textureFront, 0);
		bunker2.changeTexture(_textureFront, 0);
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
			//cout << "up" << " " << endl;
			spaceDown = false;
			releaseTime = clock();
		}
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
	
	if (xrot > 22) {
		xrot = 22;
	}

	if (xrot < -40) {
		xrot = -40;
	}

	if (yrot < -360) {
		yrot += 360;
	}

	if (yrot > 360) {
		yrot -= 360;
	}

	//cout << "Yaw: " << xrot << " Pitch: " << yrot << endl;

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
}


void drawSkyBox() {
	glPushMatrix();
	//glTranslatef(-xPos, -yPos, -zPos);
	glRotatef(xrot, 1, 0, 0);
	glRotatef(yrot, 0, 1, 0);
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


void drawBullets() {
	list<Bullet> copyList(bulletList);
	list<Bullet>::iterator it;	
	//cout << bulletList.size() << endl;
	
	for (it = copyList.begin() ; it != copyList.end(); it++ ) {
		Bullet b = *it;

		bool addAgain = true;
		bulletList.remove(b);
		if(b.move()) {
			//cout << "removing bullet" << endl;
			addAgain = false;
			//cout << bulletList.size() << endl;
		} else {		
			//cout << sittingDuck1.getDistance(b.getX(), b.getY(), b.getZ()) << endl;
			if(sittingDuck1.getDistance(b.getX(), b.getY(), b.getZ()) <= 0) {
				sittingDuck1.hit();
				cout << "Hit target, removing bullet" << endl;
				addAgain = false;
			}			
		}
		if(addAgain) {
			//cout << "check" << endl;
			bulletList.push_back(b);
		}
	}
}

void camera() {	
	GLfloat lightPos[] = {0, 0, -5.0f, 1.0f};
	GLfloat diffuseLightColor[] = {0.4f, 0.4f, 0.4f, 1.0f};
	glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuseLightColor);
	glLightfv(GL_LIGHT2, GL_POSITION, lightPos);
	glEnable(GL_LIGHT2);
	glDisable(GL_LIGHT0);

	glPushMatrix();
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, 1.0f);
	// -1 -1  1 = bottom left
	glVertex3f(-0.01f, -0.01f, -10.0f);
	//  1 -1  1 = bottom right
	glVertex3f(0.01f, -0.01f, -10.0f);
	//  1  1  1 = top right
	glVertex3f(0.01f,  0.01f, -10.0f);
	// -1  1  1 = top left
	glVertex3f(-0.01f,  0.01f, -10.0f);	
	glEnd();
	glPopMatrix();
	
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

	scale = 0.50f;
	width = 16 * scale;
	height = 9 * scale;

	//gun
	glBindTexture(GL_TEXTURE_2D, _textureGun);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glPushMatrix();
	glTranslatef(7.0f,-5.0f,0.0f);
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

	drawSkyBox();
	float yrotrad = yrot / 180 * PI;
	float xrotrad = xrot / 180 * PI;
	gluLookAt(xPos, yPos, zPos, xPos + sin(yrotrad), yPos - sin(xrotrad), zPos - cos(yrotrad), 0, 1, 0);
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


void drawScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	glMatrixMode(GL_MODELVIEW);  	
	glLoadIdentity();
	
	keyboardHandler();
	camera();	
	drawBullets();
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
	sittingDuck1.draw();
	atom1.draw();
	atom2.draw();
	atom3.draw();
	heli.draw();
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

	heli.followFlightPath();

	glutPostRedisplay();
	glutTimerFunc(40, update, 0);
}

int main(int argc, char** argv) {
	int seed = 1268511395;
	srand(seed);
	cout << "*Using seed: " << seed << endl;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	
	//http://www.swiftless.com/tutorials/opengl/fullscreen.html
	//glutGameModeString( "1920x1080:32@75" ); //the settings for fullscreen mode
    //glutEnterGameMode(); //set glut to fullscreen using the settings in the line above
	
	glutInitWindowSize (1280,800);
    glutInitWindowPosition (100, 100);
    glutCreateWindow ("Simple Window"); 


	initRendering();

	
	string name = textureDir;
	name += "heightmap.bmp";
//	_terrain = loadTerrain(name.c_str(), 20);

	int bunkerTextures[] = {_textureFront, _textureRight, _textureBack, _textureLeft};
	bunker1 = Bunker(HORI_SIZE, VERTI_SIZE, 0, 0, -100, bunkerTextures);
	bunker2 = Bunker(HORI_SIZE, VERTI_SIZE, 0, 0, 100, bunkerTextures);
	atom1 = Atom(-100.0f, PLAYER_EYE_HEIGHT, -100.0f, 14); //Si
	atom2 = Atom(-150.0f, PLAYER_EYE_HEIGHT, -100.0f, 8);  //Oxygen
	atom3 = Atom(-200.0f, PLAYER_EYE_HEIGHT, -100.0f, 1);  //Hydrogen

	float waypoints[12] = {
		-200.0f, 4*PLAYER_EYE_HEIGHT, -200.0f,
		-200.0f, 4*PLAYER_EYE_HEIGHT, 200.0f,
		200.0f, 4*PLAYER_EYE_HEIGHT, 200.0f,
		200.0f, 4*PLAYER_EYE_HEIGHT, -200.0f
	};
	int numWaypoints = 4;
	FlightPath fp = FlightPath(waypoints, numWaypoints);

	heli = Helicopter(-200.0f, 4*PLAYER_EYE_HEIGHT, -100.0f);
	heli.setFlightPath(fp);

	sittingDuck1 = SittingDuck(100, PLAYER_EYE_HEIGHT, -100.0f, 5);
	duckList.push_back(sittingDuck1);

	glutDisplayFunc(drawScene);
	glutKeyboardFunc(handleKeypress);
	glutKeyboardUpFunc(handleKeyrelease);
	glutPassiveMotionFunc(mouseMotion);
	glutReshapeFunc(handleResize);
	glutTimerFunc(25, update, 0);
	glutMainLoop();
	return 0;
}











