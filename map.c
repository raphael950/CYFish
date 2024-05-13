#include "map.h"
#include <stdlib.h>
#include <stdio.h>

#define random(min, max) (rand() % (max-min+1) + min)

Coord coordBuilder(int x, int y) {
    Coord coord;
    coord.x = x;
    coord.y = y;
    return coord;
}

int isSpawnpoint(Box* box) {
    return box != NULL && box->playerId == -1 && box->fishes == 1 && box->fishValues[0] == 1;
}

Map* mapBuilder(int penguins, int width, int length) {
    Map* map = malloc(sizeof(Map));
    if (map == NULL) exit(1);

    map->length = length;
    map->width = width;

    int nBoxes = length*width - (width - width / 2); // remove number of odd lines
    map->nBoxes = nBoxes;

    map->boxes = malloc(nBoxes*sizeof(Box));
    if (map->boxes == NULL) exit(1);

    int spawnPoints = 0;
    for (int y = 0; y < width; ++y) {
        for (int x = 0; x < length; ++x) {
            Coord coord = coordBuilder(x, y);
            Box* box = getBox(map, coord);
            if (box == NULL) continue;
            box->coord = coord;
            box->playerId = -1;
            box->fishes = random(1, 3);
            box->fishValues = malloc(sizeof(int)*box->fishes);
            if (box->fishValues == NULL) exit(1);
            for (int j = 0; j < box->fishes; j++) {
                box->fishValues[j] = random(1, 3);
            }
            if (isSpawnpoint(box)) spawnPoints++;
        }
    }

    while (spawnPoints < penguins) {
        Box* randomBox;
        do {
            randomBox = map->boxes + random(0, nBoxes-1);
        } while (isSpawnpoint(randomBox));
        randomBox->fishes = 1;
        randomBox->fishValues = realloc(randomBox->fishValues, sizeof(int));
        if (randomBox->fishValues == NULL) exit(1);
        randomBox->fishValues[0] = 1;
        spawnPoints++;
    }
    
    return map;
}

int isValidCoord(Map map, Coord coord) {
    if (coord.y >= map.width || coord.y < 0 || coord.x < 0) return 0;
    if (coord.y % 2 == 0) return coord.x < map.length - 1;
    return coord.x < map.length;
}

Box* getBox(Map* map, Coord coord) {
    if (!isValidCoord(*map, coord)) return NULL;
    int i = coord.x;
    i += (coord.y / 2) * map->length;
    i += (coord.y - coord.y/2) * (map->length-1);
    return map->boxes + i;
}

Box* getBoxFromId(Map* map, int boxId) {
    if (boxId < 0 || boxId >= map->nBoxes) return NULL;
    return map->boxes + boxId;
}

Box* getRelativeBox(Map* map, Coord coord, Direction direction) {
    int isColumnEven = coord.y % 2 == 0;
    switch (direction) {
        case WEST:
            coord.x--;
            break;
        case EAST:
            coord.x++;
            break;
        case NORTHWEST:
            if (!isColumnEven) coord.x--;
            coord.y++;
            break;
        case NORTHEAST:
            if (isColumnEven) coord.x++;
            coord.y++;
            break;
        case SOUTHWEST:
            if (!isColumnEven) coord.x--;
            coord.y--;
            break;
        case SOUTHEAST:
            if (isColumnEven) coord.x++;
            coord.y--;
            break;
        default:
            return NULL;
    }
    return getBox(map, coord);
}

int getAvailableSteps(Map* map, Coord coord, Direction direction) {
    int steps = 0;
    Box* relative = getRelativeBox(map, coord, direction);
    while (relative != NULL) {
        steps++;
        relative = getRelativeBox(map, coord, direction);
    }
    return steps;
}

Box* getDistancedRelativeBox(Map* map, Coord coord, Direction direction, int distance) {
    if (distance < 0) return NULL;
    if (distance == 0) return getBox(map, coord);
    Box* relative = getRelativeBox(map, coord, direction);
    if (relative == NULL) return NULL;
    return getDistancedRelativeBox(map, relative->coord, direction, distance-1);
}

void showBox(Box* box) {
    if (box == NULL) return;
    printf("%d ", box->fishes);
}

void showMap(Map* map) {
    for (int y = 0; y < map->width; y++) {
        for (int x = 0; x < map->length; x++) {
            showBox(getBox(map, coordBuilder(x, y)));
        }
        printf("\n");
    }
}