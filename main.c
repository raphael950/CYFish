#include "player.h"
#include "map.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

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

int main() {
    srand(time(NULL));
    int nbPlayers = 0;
    Player* players = askPlayers(&nbPlayers);
    int penguins = nbPlayers*penguinsNumber(nbPlayers);
    printf("Penguins: %d\n", penguins);
    Map* map = mapBuilder(penguins, 3, 9);
    showMap(map);

    free(map->boxes);
    free(map);
    free(players);
}