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
    for (int y = 0; y < WIDTH; ++y) {
        for (int x = 0; x < LENGTH; ++x) {

        }
    }
}

typedef struct {

} Penguin;

int main() {

    Box map[WIDTH][LENGTH];

    // demander nb joueur
    // remplir les poissons (en fonction nb pingouins)
    int players = 3;

    for (int y = 0; y < LENGTH; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            map[y][x] = boxBuilder(0, -1);
        }
    }

    for (int y = 0; y < LENGTH; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            printf("%d", map[y][x].fish);
        }
        printf("\n");
    }


    free(map);
    return 0;
}
