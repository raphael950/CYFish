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

Map* mapBuilder(int penguins, int width, int length) {
    Map* map = malloc(sizeof(Map));
    int nBoxes = length*width - (width - width / 2); // remove number of odd lines

    map->boxes = malloc(nBoxes*sizeof(Map));

    int spawnPoints = 0;
    for (int i = 0; i<nBoxes; i++) {
        Box* box = map->boxes + i;
        box->fishes = random(1, 3);
        for (int j = 0; j < box->fishes; j++) {
            box->fishValues[j] = random(1, 3);
        }
        if (box->fishes == 1 && box->fishValues[0] == 1) spawnPoints++;
    }

    while (spawnPoints < penguins) {
        Box* randomBox;
        do {
            randomBox = map->boxes + random(0, map->nBoxes-1);
        } while (randomBox->fishes == 1 && randomBox->fishValues[0] == 1);
        randomBox->fishes = 1;
        randomBox->fishValues[0] = 1;
        spawnPoints++;
    }

    map->length = length;
    map->width = width;
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

Box* getRelativeBox(Map* map, Coord coord, Direction direction) {
    switch (direction) {
        case WEST:
            coord.x--;
        case EAST:
            coord.x++;
        case NORTHWEST:
            if (coord.y % 2 == 1) coord.x--; 
            coord.y++;
        case NORTHEAST:
            if (coord.y % 2 == 0) coord.x++;
            coord.y++;
        case SOUTHWEST:
            if (coord.y % 2 == 1) coord.x--;
            coord.y--;
        case SOUTHEAST:
            if (coord.y % 2 == 0) coord.x++;
            coord.y--;
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

Box* getRecursiveRelativeBox(Map* map, Coord coord, Direction direction, int steps) {
    if (steps < 0) return NULL;
    if (steps == 0) return getBox(map, coord);
    Box* relative = getRelativeBox(map, coord, direction);
    if (relative == NULL) return NULL;
    return getRecursiveRelativeBox(map, relative->coord, direction, steps-1);
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