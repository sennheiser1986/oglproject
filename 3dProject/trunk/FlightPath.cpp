#include "FlightPath.h"

FlightPath::FlightPath(void)
{
}

FlightPath::~FlightPath(void)
{
}

FlightPath::FlightPath(float * inWaypoints, int inNumWaypoints) {
	numWaypoints = inNumWaypoints;
	waypoints = inWaypoints;
	currWaypoint = 0;
}

void FlightPath::next() {
	currWaypoint++;
	if(currWaypoint == numWaypoints) {
		currWaypoint = 0;
	}
}

float FlightPath::getX() {
	float x = waypoints[currWaypoint * 3 + 0];
	return x;
}

float FlightPath::getY() {
	float y = waypoints[currWaypoint * 3 + 1];
	return y;
}

float FlightPath::getZ() {
	float z = waypoints[currWaypoint * 3 + 2];
	return z;
}
