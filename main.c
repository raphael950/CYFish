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

typedef struct {

} Penguin;

int main() {
    Box map[WIDTH][LENGTH];



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
