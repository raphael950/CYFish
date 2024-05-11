#ifndef MAP_H

#define MAP_H
#define MAP_WIDTH 3
#define MAP_LENGTH 9

typedef struct {
    int x;
    int y;
} Coord;

typedef struct {
    Coord coord; // Coordinate of the box on the map
    int fishes;
    int* fishValues;
    int playerId; // -1 for no player
} Box;

typedef struct {
    Box* boxes;
    int nBoxes;
    int width;
    int length; // length column for even lines but length-1 columns for odds lines
} Map;

typedef enum {
    NORTHEAST,
    NORTHWEST,
    SOUTHEAST,
    SOUTHWEST,
    EAST,
    WEST
} Direction;

Coord coordBuilder(int x, int y);
Map* mapBuilder(int penguins, int width, int length);
void showMap(Map* map);
int isValidCoord(Map map, Coord coord);
Box* getBox(Map* map, Coord coord);
int isSpawnpoint(Box* box);
Box* getBoxFromId(Map* map, int boxId);
Box* getRelativeBox(Map* map, Coord coord, Direction direction);
int getAvailableSteps(Map* map, Coord coord, Direction direction);
Box* getDistancedRelativeBox(Map* map, Coord coord, Direction direction, int distance);

#endif