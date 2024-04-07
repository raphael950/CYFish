#include <stdio.h>
#include <stdlib.h>

#define BOX_LENGTH 9 // Length of chars that form Hexagon
#define BOX_WIDTH 5 // Width of chars that form Hexagon

#define BOX_Y 3 // Always supposed to be > 1 because of map scheme
#define BOX_X 9 // Always supposed to be > 0

#define SCREEN_WIDTH (BOX_Y * BOX_WIDTH - BOX_Y + 1) // Explained with a map scheme
#define SCREEN_LENGTH (BOX_LENGTH * (BOX_X / 2 + 1) + (BOX_LENGTH - 4) * (BOX_X / 2)) // Explained with a map scheme

typedef struct {
    char fish;
    char playerId;
} Box;

Box boxBuilder(char fish, char playerId) {
    Box box;
    box.fish = fish;
    box.playerId = playerId;
    return box;
}

// utile ?
void generateFish(Box map[BOX_Y][BOX_X], int minMonoPinguins) {
    int monoPinguinCount = 0;
    for (int y = 0; y < BOX_Y; ++y) {
        for (int x = 0; x < BOX_X; ++x) {
            int random = rand() % 3 + 2;
            map[y][x].fish = random;
            if (random == 1) monoPinguinCount++;
        }
    }

    // Remove fishes if they are too many, disturbing pinguins spawn
    while (monoPinguinCount < minMonoPinguins) {
        int x = rand() % BOX_Y, y = rand() % BOX_X;
        if (map[y][x].fish != 1) {
            map[y][x].fish = 1;
            monoPinguinCount++;
        }
    }
}


void showMap(char screen[SCREEN_WIDTH][SCREEN_LENGTH]) {
    for (int y = 0; y < SCREEN_WIDTH; ++y) {
        for (int x = 0; x < SCREEN_LENGTH; ++x) {
            printf("%c", screen[y][x]);
        }
        printf("\n");
    }
}

char penguinsNumber(char playerNumber) {
    switch (playerNumber) {
        case 2:
            return 4;
        case 3:
            return 3;
        case 4:
            return 2;
        default:
            return 1;
    }
}

void saveOneHexagon(char screen[SCREEN_WIDTH][SCREEN_LENGTH], int xOffset, int yOffset) {

    if (xOffset + BOX_LENGTH > SCREEN_LENGTH || yOffset + BOX_WIDTH > SCREEN_WIDTH) return;

    int x, y;
    for (y = yOffset; y < yOffset+5; y+=4) {
        for (x = xOffset + 2; x < xOffset + 7; ++x)
            screen[y][x] = '_';
    }
    screen[1 + yOffset][1 + xOffset] = '/';
    screen[2 + yOffset][0 + xOffset] = '/';
    screen[3 + yOffset][8 + xOffset] = '/';
    screen[4 + yOffset][7 + xOffset] = '/';

    screen[1 + yOffset][7 + xOffset] = '\\';
    screen[2 + yOffset][8 + xOffset] = '\\';
    screen[3 + yOffset][0 + xOffset] = '\\';
    screen[4 + yOffset][1 + xOffset] = '\\';
}

void saveHexagons(char screen[SCREEN_WIDTH][SCREEN_LENGTH]) {

    for (int y = 0; y < SCREEN_WIDTH; y+=BOX_WIDTH-1)
        for (int x = 0; x < SCREEN_LENGTH; x+=14)
            saveOneHexagon(screen, x, y);

    for (int y = BOX_WIDTH / 2; y < SCREEN_WIDTH; y+=BOX_WIDTH-1)
        for (int x = BOX_LENGTH - BOX_WIDTH / 2; x < SCREEN_LENGTH; x+=14)
            saveOneHexagon(screen, x, y);
}


int main() {
    char screen[SCREEN_WIDTH][SCREEN_LENGTH] = {0};

    for (int y = 0; y < SCREEN_WIDTH; ++y) {
        for (int x = 0; x < SCREEN_LENGTH; ++x) {
            screen[y][x] = ' ';
        }
    }

    saveHexagons(screen);
    showMap(screen);

    // TODO: ask player number >= 2 && <= 6
    int players = 6;

    return 0;
}
