// raytrace_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <cmath>
#include <iostream>
#include "Map.h"

using namespace std;

bool raytrace(int x0, int y0, int x1, int y1, bool exitOnObstruction)
{
	Map * instance = Map::getInstance();
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int x = x0;
    int y = y0;
    int n = 1 + dx + dy;
    int x_inc = (x1 > x0) ? 1 : -1;
    int y_inc = (y1 > y0) ? 1 : -1;
    int error = dx - dy;
    dx *= 2;
    dy *= 2;

	int oldX = x;
	int oldY = y;

	bool obstructed = false;

    for (; n > 0; --n)
    {
        int val = instance->getValueAt(x,y);
		if(!obstructed) {
			if(val == 9) {
				if(exitOnObstruction) {
					return false;
				}
				instance->mark(oldX,oldY,5);
				obstructed = true;
			} else {
				instance->mark(x,y,0);
			}
		} else {
			if(val != 9) {
				instance->mark(x,y,5);
				obstructed = false;
			}
		}
		cout << "x: " << x << " y: " << y <<  " error: " << error << endl;
        
		oldX = x;
		oldY = y;

		if (error > 0)
        {
            x += x_inc;
            error -= dy;
        }
        else
        {
            y += y_inc;
            error += dx;
        }
    }

	cout << "wait" << endl;
	return true;
}

bool existsLineOfSightBetween(int x0, int x1, int y0, int y1) {
	bool exitOnObstruction = true;
	raytrace(x0,y0,x1,y1,exitOnObstruction);
}



int _tmain(int argc, _TCHAR* argv[])
{
	Map::setParams(20,20,10);
	Map * instance = Map::getInstance();
	instance->mark(7,7,9);
	instance->mark(7,8,9);
	instance->mark(8,8,9);
	raytrace(0,0,10,10);
	instance->writeToFile("map.txt");
	return 0;
}


