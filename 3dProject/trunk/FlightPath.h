#pragma once

class FlightPath
{
private: 
	float * waypoints;
	int numWaypoints;
	int currWaypoint;
public:
	FlightPath(void);
	~FlightPath(void);
	FlightPath(float * inWaypoints, int inNumWaypoints);
	void  FlightPath::next();
	float FlightPath::getX();
	float FlightPath::getY();
	float FlightPath::getZ();
};
