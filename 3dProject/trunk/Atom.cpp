#include "Atom.h"
#include <ctime>
#include <stdlib.h>
#include <math.h>
#include <iostream>

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

using namespace std;

#define PI 3.14159265


void Atom::init() {
	radius = new int[7];
	speed = new int[7];
	shellRChan = new float[7];
	shellGChan = new float[7];
	shellBChan = new float[7];
	rotation = new int[numElectrons];
	phase = new int[numElectrons];
	shellNumber = new int [numElectrons];
	vecNum = new int [numElectrons];
	direction = new bool [numElectrons];
	
	bool hd = true;

	startTime = clock();
	int minRad = 10;
	int ra[] = {minRad,minRad + 4,minRad + 8,minRad + 12,minRad + 16,minRad + 20,minRad + 24};	
	int s[] = {300,300,300,300,300,300,300};
	int k = 0;
	for(k = 0; k < 7; k++) {
		radius[k] = ra[k];
		speed[k] = s[k];

		shellRChan[k] = (float)(rand() % 255)/255;
		shellGChan[k] = (float)(rand() % 255)/255;
		shellBChan[k] = (float)(rand() % 255)/255;
	}								

	cosScale = 1;
	sinScale = 0.7;
	//assign each electron to their shell
	//give them a random phase shift
	//and a random rotation
	int shellNum = 1;
	int i = 0;
	for(i = 0; i < numElectrons; i++) {
		if( (i+1) >= (2 * pow((double)shellNum, 2) + (2 * pow((double)shellNum - 1, 2)) + 1)) {
			shellNum++;
		}
		shellNumber[i] = shellNum;
		
		phase[i] = rand() % 360 + 1;
		rotation[i] = rand() % 360 + 1;
		vecNum[i] = rand() % 3;

		hd = !(hd);
		direction[i] = hd;
		
		cout <<  i << " " << shellNumber[i] << " " << rotation[i] << " " << speed[shellNumber[i]-1] << " "  << phase[i] << endl;
		cout << endl;
	}
	r = radius[shellNum-1];
}

Atom::Atom() {

}

Atom::Atom(float xIn, float yIn, float zIn, int numElec) {
	x = xIn;
	y = yIn;
	z = zIn;
	numElectrons = numElec;
	init();
}

Atom::~Atom() {

}

void Atom::draw() {
	atomTime = clock();
	double time = (double)(atomTime - startTime) / CLOCKS_PER_SEC;
	glEnable(GL_COLOR_MATERIAL);
	
	glColor3f(0.0f, 1.0f, 1.0f);
	glPushMatrix();
	glTranslatef(x, y, z);
	glutSolidSphere(1.0,32,32);
	glPopMatrix();			
	
	
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
		glColor3f(shellRChan[shellNumber[i]-1], shellGChan[shellNumber[i]-1], shellBChan[shellNumber[i]-1]);
		glutSolidSphere(0.5,32,32);
		glPopMatrix();
	}
	glColor3f(1.0f, 1.0f, 1.0f);
	glDisable(GL_COLOR_MATERIAL);
}
