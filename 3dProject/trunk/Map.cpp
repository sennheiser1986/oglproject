#include "Map.h"
#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

Map * Map::_instance = 0;
int Map::width = 500;
int Map::height = 500;
int Map::cellSide = 10;

void Map::setParams(int inWidth, int inHeight, int inCellSide) {
	width = inWidth;
	height = inHeight;
	cellSide = inCellSide;
}

Map * Map::getInstance() {
	if(!_instance) {
		_instance = new Map;
	}
	return _instance;
}

Map::Map() {
	grid = new int * [height];
	for (int i = 0; i < height; i++) {
		grid[i] = new int[width];
	}

	for (int i = 0; i < height; i++) {
		for(int j = 0; j < width; j++) {
			grid[i][j] = 3;
		}
	}
}

Map::~Map() {
	delete grid;
}

bool Map::markBlock(int row, int col, int h, int w, int cost) {
	int minRow = row;
	int minCol = col;

	int maxRow = row + h - 1;
	int maxCol = col + w - 1; 
	for(int i = minRow; i <= maxRow; i++) {
		for(int j = minCol; j <= maxCol; j++) {
			if(!(mark(i,j,cost))) {
				return false;
			}
		}
	}
	return true;
}

void Map::writeToFile(char * filename) {
	ofstream myfile;
	myfile.open (filename);
	for(int i = 0; i < height; i++) {
		for(int j = 0; j < width; j++) {
			myfile << grid[i][j] ;
		}
		myfile << endl;
	}

	myfile.close();	
}

bool Map::mark(int row, int col, int cost) {
	if(grid[row][col] != 9) {
		grid[row][col] = cost;
		return true;
	} else {
		return false;
	}
}

int * Map::convertWorldCoordToMapCoord(int x, int y) {
	int cols = width;
	int rows = height;
	int col = ceil((double)(x / cellSide)) + cols / 2 - 1;
	int row = rows / 2 - ceil((double)(y / cellSide));

	int * arr = new int[2];
	arr[0] = row;
	arr[1] = col;

	return arr;
}

int ** Map::getMap() {
	return grid;
}

int Map::getValueAt(int row, int col) {
	if(row < height && col < width) {
		return grid[row][col];
	} else {
		return 9;
	}	
}

int Map::getWidth() {
	return width;
}

int Map::getHeight() {
	return height;
}