#include "player.h"
#include "map.h"
#include "penguin.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <ncurses.h>

void printBox(Box* box, WINDOW* mapWin, int printBorder, int printFishes) {
    if (box == NULL) return;
    int x = box->coord.x, y = box->coord.y;
    int yOffset = y*3;
    int xOffset = x*8 + (y%2==0)*4;
    if (printBorder) {
        mvwaddch(mapWin, yOffset, xOffset + 3, '/');
        mvwaddch(mapWin, yOffset, xOffset + 5, '\\');
        mvwaddch(mapWin, yOffset + 1, xOffset + 1, '/');
        mvwaddch(mapWin, yOffset + 1, xOffset + 7, '\\');
        mvwaddch(mapWin, yOffset + 2, xOffset, '|');
        mvwaddch(mapWin, yOffset + 2, xOffset + 8, '|');
        mvwaddch(mapWin, yOffset + 3, xOffset + 3, ' ');
        mvwaddch(mapWin, yOffset + 3, xOffset + 1, '\\');
        mvwaddch(mapWin, yOffset + 3, xOffset + 7, '/');
        mvwaddch(mapWin, yOffset + 4, xOffset + 3, '\\');
        mvwaddch(mapWin, yOffset + 4, xOffset + 5, '/');
    }
    if (printFishes && box->fishes > 0) {

    }
    if (box->playerId >= 0) {
        mvwprintw(mapWin, yOffset + 2, xOffset + 3, "🐧");
    }
}

int main() {
    srand(time(NULL));
    int nbPlayers = 0;
    Player* players = askPlayers(&nbPlayers);
    int penguins = nbPenguin(nbPlayers);
    printf("Penguins: %d\n", penguins);
    Map* map = mapBuilder(penguins*nbPlayers, 9, 9);

    initscr();
    int mapWinHeight = 5 + (map->width - 1) * 3;
    int mapWinLength = map->length * 8 + 1;

    WINDOW* mapWin = newwin(mapWinHeight, mapWinLength, 0, 0);
    refresh();
    box(mapWin, 0, 0);

    for (int i = 0; i < map->nBoxes; ++i) {
        printBox(map->boxes + i, mapWin, 1, 1);
    }

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

    wrefresh(mapWin);
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