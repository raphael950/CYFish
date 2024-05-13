#include "player.h"
#include "map.h"
#include "penguin.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <ncurses.h>

void printHexa(int xOffset, int yOffset) {
    mvaddch(yOffset, xOffset + 3, '/');
    mvaddch(yOffset, xOffset + 5, '\\');
    mvaddch(yOffset + 1, xOffset + 1, '/');
    mvaddch(yOffset + 1, xOffset + 7, '\\');
    mvaddch(yOffset + 2, xOffset, '|');
    mvaddch(yOffset + 2, xOffset + 8, '|');
    mvaddch(yOffset + 3, xOffset + 3, ' ');
    mvaddch(yOffset + 3, xOffset + 1, '\\');
    mvaddch(yOffset + 3, xOffset + 7, '/');
    mvaddch(yOffset + 4, xOffset + 3, '\\');
    mvaddch(yOffset + 4, xOffset + 5, '/');
}

void printBox(Box* box, int printBorder, int printFishes) {
    if (box == NULL) return;
    int x = box->coord.x, y = box->coord.y;
    int yOffset = y*3;
    int xOffset = x*8 + (y%2==0)*4;
    if (printBorder) {
        mvaddch(yOffset, xOffset + 3, '/');
        mvaddch(yOffset, xOffset + 5, '\\');
        mvaddch(yOffset + 1, xOffset + 1, '/');
        mvaddch(yOffset + 1, xOffset + 7, '\\');
        mvaddch(yOffset + 2, xOffset, '|');
        mvaddch(yOffset + 2, xOffset + 8, '|');
        mvaddch(yOffset + 3, xOffset + 3, ' ');
        mvaddch(yOffset + 3, xOffset + 1, '\\');
        mvaddch(yOffset + 3, xOffset + 7, '/');
        mvaddch(yOffset + 4, xOffset + 3, '\\');
        mvaddch(yOffset + 4, xOffset + 5, '/');
    }
    if (printFishes && box->fishes > 0) {

    }
    if (box->playerId >= 0) {
        mvprintw(yOffset + 2, xOffset + 3, "🐧");
    }
}

int main() {
    srand(time(NULL));
    int nbPlayers = 0;
    Player* players = askPlayers(&nbPlayers);
    int penguins = nbPenguin(nbPlayers);
    printf("Penguins: %d\n", penguins);
    Map* map = mapBuilder(penguins*nbPlayers, 9, 9);

    //showMap(map);

    /*
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
    }*/
    /*
    // Affichage temporaire des playerId pour les test
    for (int y = 0; y < map->width; y++) {
        for (int x = 0; x < map->length; x++) {
            Box* box = getBox(map, coordBuilder(x, y));
            if (box == NULL) continue;
            printf("%d ", box->playerId);
        }
        printf("\n");
    }*/

    int mapDisplayHeight, mapDisplayLength;
    mapDisplayHeight = (map->width / 2) * 5 + (map->width - map->width / 2) * 3;
    mapDisplayLength = map->width * 8 + 1;

    initscr();              // Initialise la structure WINDOW et autres paramètres

    for (int y = 0; y < map->width; ++y) {
        for (int x = 0; x < map->length; ++x) {
            Box* box = getBox(map, coordBuilder(x, y));
            if (box == NULL) continue;
            printBox(box, 1, 1);
        }
    }

    refresh();              // Rafraîchit la fenêtre courante afin de voir le message apparaître
    curs_set(0);
    getch();                // On attend que l'utilisateur appui sur une touche pour quitter
    endwin();


    for (int i = 0; i < map->nBoxes; i++) {
        free(map->boxes[i].fishValues);
        free(map->boxes + i);
    }
    free(map);
    free(players);
}