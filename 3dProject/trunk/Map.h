/* 
 *   3dProject
 *   Geert d'Hoine
 *   (c) 2010
 */ 
#ifndef MAP_H
#define MAP_H

class Map
{
private: 
  static Map* _instance;
  static int width;
  static int height;
  static int cellSide;
  int ** grid;

  Map();
  ~Map();
 
public:
  static const int INACCESSIBLE_FIELD_VALUE = 9;
  static Map *getInstance();
  static void setParams(int width, int height, int cellSide);
  bool mark(int x, int y, int cost);
  bool debugMark(int x, int y, int cost);
  bool markBlock(int row, int col, int height, int width, int cost);
  int * convertWorldCoordToMapCoord(int xIn, int yIn);
  int * convertMapCoordToWorldCoord(int rowIn, int colIn);
  int ** getMap();
  int getValueAt(int row, int col) ;
  int getWidth();
  int getHeight();
  int getRealWidth();
  int getRealHeight();
  int getCellSide();
  void writeToFile(const char * filename);
};


#endif