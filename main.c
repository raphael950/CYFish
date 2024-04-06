#include <stdio.h>
#include <stdlib.h>


#define WIDTH 9
#define LENGTH 9


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
void generateFish(Box map[WIDTH][LENGTH], int minMonoPinguins) {
    int monoPinguinCount = 0;
    for (int y = 0; y < WIDTH; ++y) {
        for (int x = 0; x < LENGTH; ++x) {
            int random = rand() % 3 + 2;
            map[y][x].fish = random;
            if (random == 1) monoPinguinCount++;
        }
    }

    // Remove fishes if they are too many, disturbing pinguins spawn
    while (monoPinguinCount < minMonoPinguins) {
        int x = rand() % WIDTH, y = rand() % LENGTH;
        if (map[y][x].fish != 1) {
            map[y][x].fish = 1;
            monoPinguinCount++;
        }
    }
}


void showMap(char map) {
    // print les bordures et
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

void stockeUnHexagone(char screen[WIDTH*4-1][9*(LENGTH/2 + 1) + 5*(LENGTH / 2)], int startX, int startY) {

    int x, y;
    for (y = startY; y < startY+5; y+=4) {
        for (x = startX + 2; x < startX + 7; ++x)
            screen[y][x] = '_';
    }

    // les /
    screen[1 + startY][1 + startX] = '/';
    screen[2 + startY][0 + startX] = '/';
    screen[3 + startY][8 + startX] = '/';
    screen[4 + startY][7 + startX] = '/';

    screen[1 + startY][7 + startX] = '\\';
    screen[2 + startY][8 + startX] = '\\';
    screen[3 + startY][0 + startX] = '\\';
    screen[4 + startY][1 + startX] = '\\';

}

void stockHexagones(char screen[WIDTH*4-1][9*(LENGTH/2 + 1) + 5*(LENGTH / 2)]) {
    for (int y = 0; y < WIDTH*4-1; y+=5) {
        for (int x = 0; x < 9*(LENGTH/2 + 1) + 5*(LENGTH / 2); x+=5) {

        }
    }
    stockeUnHexagone(screen, 0, 0);
    stockeUnHexagone(screen, 0, 4);
    stockeUnHexagone(screen, 0, 8);

    stockeUnHexagone(screen, 7, 2);
    stockeUnHexagone(screen, 7, 6);

    stockeUnHexagone(screen, 14, 0);
    stockeUnHexagone(screen, 14, 4);
    stockeUnHexagone(screen, 14, 8);
}


int main() {
    Box map[WIDTH][LENGTH];




    char screen[WIDTH*4-1][9*(LENGTH/2 + 1) + 5*(LENGTH / 2)] = {0};

    for (int y = 0; y < WIDTH*4-1; ++y) {
        for (int x = 0; x < 9*(LENGTH/2 + 1) + 5*(LENGTH / 2); ++x) {
            screen[y][x] = ' ';
        }
    }

    stockHexagones(screen);

    for (int y = 0; y < WIDTH*4-1; ++y) {
        for (int x = 0; x < 9*(LENGTH/2 + 1) + 5*(LENGTH / 2); ++x) {
            printf("%c", screen[y][x]);
        }
        printf("\n");
    }

    // TODO: ask player number >= 2 && <= 6
    int players = 6;

    // Cases initialization
    for (int y = 0; y < LENGTH; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            map[y][x] = boxBuilder(0, -1);
        }
    }


    generateFish(map, penguinsNumber(players)*players);

    for (int y = 0; y < LENGTH; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            printf("%d", map[y][x].fish);
        }
        printf("\n");
    }


    return 0;
}
