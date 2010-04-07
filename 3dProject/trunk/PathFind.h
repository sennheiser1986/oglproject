#include <list>

class PathFind {

public:	
	PathFind();
	~PathFind();
	bool existStraightPath(int x0, int y0, int x1, int y1);
	bool calculateStraightPath(int x0, int y0, int x1, int y1);
	bool calculatePath(int x0, int y0, int x1, int y1);
	void getCalculatedPath(std::list<int*>& out);
private:
	int beforeObstacleX;
	int beforeObstacleY;
	int afterObstacleX;
	int afterObstacleY;

	void astarSearch(int x0, int y0, int x1, int y1, std::list<int *>& out);
	bool raytrace(int x0, int y0, int x1, int y1, bool exitOnObstruction);

	std::list<int *> path;
};