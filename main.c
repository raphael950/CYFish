#include "player.h"
#include "map.h"
#include "penguin.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <ncurses.h>
#include <locale.h>
#include "movement.h"
#include "window.h"




int main() {
    setlocale(LC_ALL, "");
    srand(time(NULL));
    int nbPlayers = 0;
    Player* players = askPlayers(&nbPlayers);
    int penguins = nbPenguin(nbPlayers);
    printf("Penguins: %d\n", penguins);

    initscr();
    curs_set(0);
    noecho();
    start_color();
    use_default_colors();


    Map* map = mapBuilder(penguins*nbPlayers, 9, 9);

    init_pair(1, COLOR_WHITE, COLOR_RED);
    init_pair(2, COLOR_WHITE, COLOR_GREEN);
    init_pair(3, COLOR_WHITE, COLOR_YELLOW);
    init_pair(4, COLOR_WHITE, COLOR_BLUE);
    init_pair(5, COLOR_WHITE, COLOR_MAGENTA);
    init_pair(6, COLOR_WHITE, COLOR_CYAN);
    init_pair(7, COLOR_WHITE, COLOR_WHITE);
    init_pair(8, COLOR_WHITE, COLOR_BLUE);
    init_pair(10, COLOR_BLACK, COLOR_WHITE);

    WINDOW* popUp = getMessageWindow();

    refresh();
    box(popUp, 0, 0);
    wrefresh(popUp);

    for (int i = 0; i < map->nBoxes; ++i) {
        printBox(map->boxes + i, map->mapWin, 1, 1);
    }


    // Placement des pingouins
    for (int i = 0; i < penguins; i++) {
            for (Player* p = players; p < players + nbPlayers; p++) {
                placeOnePenguin(map, p);
            }
    }

    int played = 0;
    while(1) {
        // for each players
        for (Player* p = players; p < players + nbPlayers; p++) {
            int res = turn(p, map);
            if (res == 0) continue;
            played++;
            mvwprintw(popUp, 1, 1, "C'est au tour de %s de jouer", p->name);
            wrefresh(popUp);
            int key;
            do {
                key = getch();
                if (key == KEY_RIGHT || key == KEY_LEFT || key == KEY_UP || key == KEY_DOWN || key == 67 || key == 68 || key == 65 || key == 66) {
                    Box* from = penguinSelection(map, p->playerId, map->boxes, key);
                    if (from == NULL) continue;
                    Box* to = getRelativeBox(map, from->coord, key);
                    movePenguin(from, to, p, map);
                }
            } while (key != KEY_ENTER && key != 10);
        }
        if (played == 0) break; // game finished
    }//

    wrefresh(popUp);
    wrefresh(map->mapWin);
    getch();
    endwin();


    for (int i = 0; i < map->nBoxes; i++) {
        free(map->boxes[i].fishValues);
        free(map->boxes + i);
    }
    free(map);
    free(players);
}