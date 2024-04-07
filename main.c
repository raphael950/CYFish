#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define BOX_LENGTH 9 // Length of chars that form Hexagon
#define BOX_WIDTH 5 // Width of chars that form Hexagon

#define BOX_Y 3 // Always supposed to be > 1 because of map scheme
#define BOX_X 9 // Always supposed to be > 0 and odd because of map scheme TODO: add odd support by fixing saveHexagons() first for loop x

#define SCREEN_WIDTH (BOX_Y * BOX_WIDTH - BOX_Y + 1) // Explained with a map scheme
#define SCREEN_LENGTH (BOX_LENGTH * (BOX_X / 2 + 1) + (BOX_LENGTH - 4) * (BOX_X / 2)) // Explained with a map scheme

typedef struct {
    bool isReachable;
    char fish;
    char playerId;
} Box;

Box boxBuilder(bool isReachable, char fish, char playerId) {
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


void showScreen(char screen[SCREEN_WIDTH][SCREEN_LENGTH]) {
    for (int y = 0; y < SCREEN_WIDTH; ++y) {
        for (int x = 0; x < SCREEN_LENGTH; ++x) {
            printf("%c", screen[y][x]);
        }
        printf("\n");
    }
}

void setupMap(Box map[BOX_Y][BOX_X]) {
    for (int y = 0; y < BOX_Y; ++y) {
        for (int x = 0; x < BOX_X; ++x) {

            map[y][x] = boxBuilder(1, 0, -1);

            if (y == BOX_Y - 1 && x % 2 == 1) {
                map[y][x].isReachable = false;
            }
        }
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

    for (int y = 0; y < SCREEN_WIDTH; y+=BOX_WIDTH/2) {
        for (int x = 0; x < SCREEN_LENGTH; x+=7) {
            if (y % 4 == 0 && x % 14 == 0 || y % 4 != 0 && x % 14 != 0) {
                saveOneHexagon(screen, x, y);
            }
        }
    }

}

int askPlayers(int min, int max) {
    int players, res = -1;
    do {
        printf("Combien de joueurs joueront à la partie ?");
        res = scanf("%d", &players);
        printf("\n");
    } while (res < 1 || players < min || players > max);
    return players;
}


int main() {

    int players = askPlayers(2, 6);


    char screen[SCREEN_WIDTH][SCREEN_LENGTH] = {0};

    for (int y = 0; y < SCREEN_WIDTH; ++y) {
        for (int x = 0; x < SCREEN_LENGTH; ++x) {
            screen[y][x] = ' ';
        }
    }

    saveHexagons(screen);
    showScreen(screen);




    return 0;
}
