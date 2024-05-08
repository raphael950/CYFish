#include "player.h"
#include "map.h"
#include "penguin.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

int main() {
    srand(time(NULL));
    int nbPlayers = 0;
    Player* players = askPlayers(&nbPlayers);
    int penguins = nbPenguin(nbPlayers);
    printf("Penguins: %d\n", penguins);
    Map* map = mapBuilder(penguins*nbPlayers, 3, 9);

    showMap(map);

    // Placement des pingouins
    for (int i = 0; i < penguins; i++) {
            for (Player* p = players; p < players + nbPlayers; p++) {
                Box* box;
                do {
                    printf("Player %s, entrez le numero de case pour placer un pingouin (entre 1 et %d): ", p->name, map->nBoxes+1);
                    int boxId;
                    scanf("%d", &boxId);
                    box = getBoxFromId(map, boxId-1);
                } while (!isSpawnpoint(box));
                box->playerId = p - players;
            }
    }

    // Affichage temporaire des playerId pour les test
    for (int y = 0; y < map->width; y++) {
        for (int x = 0; x < map->length; x++) {
            Box* box = getBox(map, coordBuilder(x, y));
            if (box == NULL) continue;
            printf("%d ", box->playerId);
        }
        printf("\n");
    }


    for (int i = 0; i < map->nBoxes; i++) {
        free(map->boxes[i].fishValues);
        free(map->boxes + i);
    }
    free(map);
    free(players);
}