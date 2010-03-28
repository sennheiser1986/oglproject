#include "FlightPath.h"
#include <iostream>

using namespace std;

FlightPath::FlightPath(void)
{
}

FlightPath::~FlightPath(void)
{
}

FlightPath::FlightPath(int * inWaypoints, int inNumWaypoints) {
	numWaypoints = inNumWaypoints;
	waypoints = inWaypoints;
	currWaypoint = 0;
}

void FlightPath::next() {
	currWaypoint++;
	cout << "current waypoint++: " << currWaypoint << endl;
	if(currWaypoint == numWaypoints) {
		currWaypoint = 0;
	}
}

int FlightPath::getX() {
	int x = waypoints[currWaypoint * 3 + 0];
	cout << "get x: " << x << endl;
	return x;
}

int FlightPath::getY() {
	int y = waypoints[currWaypoint * 3 + 1];
	cout << "get y: " << y << endl;
	return y;
}

int FlightPath::getZ() {
	int z = waypoints[currWaypoint * 3 + 2];
	cout << "get z: " << z << endl;
	return z;
}
