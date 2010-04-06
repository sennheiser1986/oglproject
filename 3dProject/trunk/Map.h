// Header file (.h)
//
// A thread safe Singleton. Version 2.
//
// This version looks deceptively simple, but it comes with caveats. First,
// if this singleton exists in some library. Users that use that library
// will have an instance of the singleton during application execution 
// whether it is used or not. 
//
// Second, is the case of file static dependency. For example, assume that
// the Singleton is some abstract factory for the type BaseType, and 
// implements a method create. As the order of initialization of file
// static variables across translation units is not defined, 
// this can lead to someone accessing the singleton before it has been
// constructed, aka undefined behavior, which is a bad thing. 
//
// namespace { const BaseType * const fileStaticVariable = Singleton::getInstance().create(); }
//
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
  Map(const Map &){};
  Map & operator=(const Map &){};
 
public:
  static Map *getInstance();
  static void setParams(int width, int height, int cellSide);
  bool mark(int x, int y, int cost);
  bool markBlock(int row, int col, int height, int width, int cost);
  int * convertWorldCoordToMapCoord(int xIn, int yIn);
  int ** getMap();
  int getValueAt(int row, int col) ;
  int getWidth();
  int getHeight();
  void Map::writeToFile(char * filename);
};


